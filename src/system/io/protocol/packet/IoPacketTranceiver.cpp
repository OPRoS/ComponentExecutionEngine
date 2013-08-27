/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoPacketTranceiver.cpp
 *
 */

#include "IoPacketTranceiver.h"

#include "system/io/IoEventProcessor.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/io/protocol/IoByteUtil.h"


IoPacketTranceiver::IoPacketTranceiver(IoPacketProtocol *p, IoConnection *cnn)
{
	trace_enter();

	m_protocol = p;
	m_cnn = cnn;

	m_sendBuf = NULL;
	m_recvBuf = NULL;

	m_recvState = MSG_RECV_INIT;

	m_header.reserve(IO_MSG_HEADER_SIZE);

	m_closed = false;
}

IoPacketTranceiver::~IoPacketTranceiver()
{
	if (m_closed == false) onClose();
}

void IoPacketTranceiver::onClose()
{
	m_closed = true;
	m_nf.notifyAll();
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void IoPacketTranceiver::endWrite()
{
	m_nf.lock();
	if (m_sendBuf != NULL) delete m_sendBuf;
	m_sendBuf = NULL;
	m_sendBufSize = 0;
	m_nf.unlock();

	m_nf.notify();
}

void IoPacketTranceiver::errorWrite()
{
	m_nf.lock();
	if (m_sendBuf != NULL) delete m_sendBuf;
	m_sendBuf = NULL;
	m_sendBufSize = 0;
	m_nf.unlock();

	m_nf.notify();
}

bool IoPacketTranceiver::sendPacket(byte *buf, int bufSize)
{
	trace_enter();

	m_nf.lock();

	while (m_sendBuf != NULL) {
		m_nf.wait();
		if (m_closed || m_cnn->isOpen() == false) {
			return false;
		}
	}
	m_sendBuf = buf;
	m_sendBufSize = bufSize;
	m_writePos = 0;

	m_nf.unlock();

	IoByteUtil util;
	trace("***** Sending Msg" );
	trace("      BodySize =" << m_sendBufSize);

	int wc = m_cnn->write( m_sendBuf + m_writePos,  m_sendBufSize - m_writePos);
	if (wc < 0) {
		errorWrite();
		log_error("Connection write error : cnn.id=" << m_cnn->getId());
		return false;
	}

	m_writePos += wc;
	if (m_writePos < m_sendBufSize) {
		trace("Write CONTINUE------------");
		m_cnn->addEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);
	}
	else {
		endWrite();
		trace("Write DONE------------");
	}

	return true;
}

bool IoPacketTranceiver::onSend()
{
	if (m_closed || m_sendBuf == NULL || m_sendBufSize <= 0 || m_cnn->isOpen() == false) {
		return false;
	}

	int wc = m_cnn->write(m_sendBuf + m_writePos, m_sendBufSize - m_writePos);
	if (wc < 0) {
		errorWrite();
		log_error("Connection write error : cnn.id=" << m_cnn->getId());
		return false;
	}

	m_writePos += wc;
	if (m_writePos == m_sendBufSize) {
		endWrite();
		m_cnn->clearEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);
	}

	return true;
}

//--------------------------------------
// IoConnection에서 데이터를 읽는 함수
// - 한번에 데이터를 다 못 읽을 수 있음.
// - 데이터를 다 못 읽을 경우 상태를 기록하고 추후 계속 읽음
//
// @ret 읽기 실패시 false 리턴
//
bool IoPacketTranceiver::onReceive()
{
	trace_enter();

	bool readEnd = false;
	while (!readEnd) {
		trace("cnn.id=" << m_cnn->getId() << ", readState=" << m_recvState);
		if (m_closed || m_cnn->isOpen() == false) {
			return false;
		}

		switch (m_recvState) {
			// 초기 읽기 상태는 MSG_RECV_INIT (초기화 수행)
			case MSG_RECV_INIT:
			{
				trace("[onReceive] MSG_RECV_INIT");
				m_recvState = MSG_RECV_MAGIC;
			}
			break;

			// magic 값을 읽음.
			// 데이터를 읽기 전에 magic값 체크 (데이터가 깨질 경우를 대비해서)
			case MSG_RECV_MAGIC:
			{
				trace("[onReceive] MSG_RECV_MAGIC");

				byte magic;
				int readLen = m_cnn->read(&magic, 1);

				IoByteUtil util;
				trace("***** MAGIC Read Len = " << readLen);
				// util.PrintsBytes(buf, 0, 1);

				if (readLen <= 0) {
					m_recvState = MSG_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				if (magic == IO_MSG_MAGIC) {
					m_header.reset();
					m_readPos = 0;
					m_recvState = MSG_RECV_HEADER;
				}
				else {
					m_recvState = MSG_RECV_INIT;
					return true;
				}
			}
			break;

			// 데이터의 헤더를 읽음
			// 헤더에는 메시지 바디의 크기(int)가 들어 있음
			case MSG_RECV_HEADER:
			{
				trace("[onReceive] MSG_RECV_HEADER");

				byte *buf = m_header.getBuffer();
				int size = m_header.getSize();
				int len = size - m_readPos;

				int readLen = m_cnn->read(buf + m_readPos, len);
				trace("***** HEADER Read Len = " << readLen);
				if (readLen <= 0) {
					m_recvState = MSG_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				m_readPos += readLen;
				if (m_readPos < size) {
					return true;
				}

				IoByteUtil util;
				// util.PrintsBytes(buf, 0, size);
				try {
					m_header >> m_recvBufSize;
				}
				catch (Exception &ex) {
					log_error(ex);
					m_recvState = MSG_RECV_INIT;
					return true;
				}

				trace("print header-----");
				trace("      BodySize =" << m_recvBufSize);

				if (m_recvBufSize > 0) {
					m_recvBuf = new byte[m_recvBufSize];
				}
				else {
					m_recvBuf = NULL;
				}
				m_readPos = 0;
				m_recvState = MSG_RECV_BODY;
				readEnd = true;
			}
			break;

			// 메시지 바디를 읽음
			case MSG_RECV_BODY:
			{
				trace("[onReceive] MSG_RECV_BODY : bodySize=" << m_recvBufSize);
				byte *buf = m_recvBuf;
				int size = m_recvBufSize;

				if (size <= 0) return true;

				int len = size - m_readPos;
				int readLen = m_cnn->read(buf + m_readPos, len);
				trace("***** BODY Read Len = " << readLen);
				if (readLen <= 0) {
					m_recvState = MSG_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				m_readPos += readLen;
				if (m_readPos < size) {
					return true;
				}

				// 메시지 바디를 다 읽었을 경우 상태를 MSG_RECV_INIT으로 함
				m_recvState = MSG_RECV_INIT;
				readEnd = true;

				// 다 읽은 메시지 처리
				processReceive();
			}
			break;

		} // end of switch
	} // end of while

	return true;
}

void IoPacketTranceiver::processReceive()
{
	trace_enter();

	byte *body;
	int bodySize;
	m_nf.lock();
	body = m_recvBuf;
	bodySize = m_recvBufSize;

	m_recvBuf = NULL;
	m_recvBufSize = 0;
	m_nf.unlock();

	m_protocol->packet_receive(m_cnn, body, bodySize);
}
