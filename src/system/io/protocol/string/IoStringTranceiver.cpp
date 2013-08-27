/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStringTranceiver.cpp
 *
 */

#include "IoStringTranceiver.h"

#include <cstring>

#include "system/io/IoEventProcessor.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoStringTranceiver::IoStringTranceiver(IoStringProtocol *p, IoConnection *cnn)
{
	trace_enter();

	m_protocol = p;
	m_cnn = cnn;

	m_recvState = STR_RECV_INIT;

	m_closed = false;

	m_sendBuf = NULL;
	m_sendBufSize = 0;

	m_recvBufSize = STR_RECV_BUF_SIZE;
	m_readPos = 0;
	m_readLen = 0;
}

IoStringTranceiver::~IoStringTranceiver()
{
	if (m_closed == false) onClose();
}

void IoStringTranceiver::onClose()
{
	m_closed = true;
	m_nf.notifyAll();
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void IoStringTranceiver::endWrite()
{
	m_nf.lock();
	if (m_sendBuf != NULL) delete m_sendBuf;
	m_sendBuf = NULL;
	m_sendBufSize = 0;
	m_nf.unlock();

	m_nf.notify();
}

void IoStringTranceiver::errorWrite()
{
	m_nf.lock();
	if (m_sendBuf != NULL) delete m_sendBuf;
	m_sendBuf = NULL;
	m_sendBufSize = 0;
	m_nf.unlock();

	m_nf.notify();
}

bool IoStringTranceiver::sendString(const string &m_sendStr)
{
	trace_enter();

	m_nf.lock();

	while (m_sendBuf == NULL) {
		m_nf.wait();
		if (m_closed || m_cnn->isOpen() == false) {
			m_nf.unlock();
			return false;
		}
	}
	m_writePos = 0;

	m_nf.unlock();

	//m_sendStr = str;

	m_sendBufSize = m_sendStr.size();
	m_sendBuf = new byte[m_sendBufSize];
	if (m_sendBuf==NULL)
	{
		trace("sendString internal error------------");
		return false;
	}
	memcpy((void *) m_sendBuf, (void *) m_sendStr.c_str(), m_sendBufSize);

	int wc = m_cnn->write( m_sendBuf + m_writePos,  m_sendBufSize - m_writePos);
	if (wc < 0) {
		errorWrite();

		log_error("Connection write error : cnn.id=" << m_cnn->getId());
		return false;
	}

	m_writePos += wc;
	if (m_writePos < m_sendBufSize) {
		trace("Write CONTINUE------------");
		endWrite();

		m_cnn->addEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);
	}
	else {
		trace("Write DONE------------");
	}

	return true;
}

bool IoStringTranceiver::onSend()
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
bool IoStringTranceiver::onReceive()
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
			case STR_RECV_INIT:
			{
				trace("[onReceive] STR_RECV_INIT");
				m_recvState = STR_RECV_BODY;
			}
			break;

			// 메시지 바디를 읽음
			case STR_RECV_BODY:
			{
				trace("[onReceive] STR_RECV_BODY");

				m_readLen = m_cnn->read((byte *) m_recvBuf, m_recvBufSize);
				if (m_readLen <= 0) {
					m_recvState = STR_RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				// 메시지 바디를 다 읽은후 메시지의 끝을 체크함
				m_readPos = 0;
				m_recvStr.clear();
				m_recvState = STR_RECV_CHECK_EOL;
			}
			break;

			// 메시지  끝을 체크함
			case STR_RECV_CHECK_EOL:
			{
				trace("[onReceive] STR_RECV_CHECK_EOL: readPos=" << m_readPos << ";readLen=" << m_readLen);

				bool isEOL = false;
				while (m_readPos <= m_readLen){
					char ch = (char) m_recvBuf[m_readPos++];

				trace("[onReceive] STR_RECV_CHECK_EOL: readPos=" << m_readPos << ";ch=" << ch);

					if (ch == '\r' || ch == '\n') {
						isEOL = true;
						break;
					}
					else {
						m_recvStr.push_back(ch);
					}
				}

				if (isEOL) {
					// 다 읽은 메시지 처리
					processReceive();
				}

				// 버퍼에 있는 데이터를 모두 처리했으면 다시 읽기
				if (m_readPos > m_readLen) {
					m_recvState = STR_RECV_BODY;
				}
				readEnd = true;
			}
			break;

		} // end of switch
	} // end of while

	return true;
}

void IoStringTranceiver::processReceive()
{
	trace_enter();

	m_protocol->string_receive(m_cnn, m_recvStr);
}
