/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoRequestTranceiver.cpp
 *
 */

#include <opros_smart_ptr.h>
#include "IoRequestTranceiver.h"

#include "system/io/IoEventProcessor.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/io/protocol/IoByteUtil.h"


IoRequestTranceiver::IoRequestTranceiver(IoRequestProtocol *p, IoConnection *cnn)
{
	trace_enter();

	m_protocol = p;
	m_cnn = cnn;
	m_recvState = RECV_INIT;

	m_closed = false;
}

IoRequestTranceiver::~IoRequestTranceiver()
{
	if (m_closed == false) onClose();
}

void IoRequestTranceiver::onClose()
{
	m_closed = true;
	m_nf.notifyAll();
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void IoRequestTranceiver::endRequest()
{
	m_nf.lock();
	
	if (m_req->getRequestType() & REQUEST_TYPE_ONEWAY)
		m_reqMap.erase(m_req->getRequestId());

	m_req = m_nullReqPtr;
	m_nf.unlock();

	m_nf.notify();
}

void IoRequestTranceiver::errorRequest()
{
	m_nf.lock();
	m_reqMap.erase(m_res.getRequestId());
	m_req = m_nullReqPtr;
	m_nf.unlock();

	m_nf.notify();
}

bool IoRequestTranceiver::sendRequest(IoRequestPtr req)
{
	trace_enter();

	m_nf.lock();

	while (m_req.get() != NULL) {
		m_nf.wait();
		if (m_closed || m_cnn->isOpen() == false) {
			m_nf.unlock();
			return false;
		}
	}

	if (!(req->getRequestType() & REQUEST_TYPE_ONEWAY))
		m_reqMap[req->getRequestId()] = req;


	m_req = req;
	m_writePos = 0;

	m_nf.unlock();

	byte *buf = m_req->getStream()->getBuffer();
	if (buf == NULL)
	{
		trace("[sendRequest] stream buffer NULL");
		return false;
	}
	int   size = m_req->getStream()->limit();

	IoByteUtil util;
	// util.PrintsBytes(buf, 0, size);

	trace("***** RequestVersion =" << util.byte2char(m_req->getRequestVersion()));
	trace("      RequestType =" << util.byte2char(m_req->getRequestType()));
	trace("      RequestDirection =" << util.byte2char(m_req->getRequestDirection()));
	trace("      RequestId =" << m_req->getRequestId());
	trace("      BodySize =" << m_req->getBodySize());

	int wc = m_cnn->write( buf + m_writePos,  size - m_writePos);
	if (wc < 0) {
		errorRequest();

		log_error("Connection write error : cnn.id=" << m_cnn->getId());
//		m_cnn->getEventProcessor()->unregisterEventTarget(m_cnn);
		return false;
	}

	m_writePos += wc;
	if (m_writePos < size) {
		trace("Write CONTINUE------------");
		m_cnn->addEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);
	}
	else {
		trace("Write DONE------------");
		endRequest();
	}

	return true;
}

bool IoRequestTranceiver::onSend()
{
	if (m_closed || m_req.get()== NULL || m_cnn->isOpen() == false) {
		return false;
	}

	byte *buf = m_req->getStream()->getBuffer();
	if (buf == NULL)
	{
		trace("[onSend] stream buffer NULL");
		return false;
	}

	int   size = m_req->getStream()->limit();

	int wc = m_cnn->write(buf + m_writePos, size - m_writePos);
	if (wc < 0) {
		errorRequest();

		log_error("Connection write error : cnn.id=" << m_cnn->getId());
//		m_cnn->getEventProcessor()->unregisterEventTarget(m_cnn);
		return false;
	}

	m_writePos += wc;
	if (m_writePos == size) {
		m_cnn->clearEvent(IoEventUtil::writeEvent());
		m_cnn->getEventProcessor()->registerEventTarget(m_cnn);

		endRequest();
	}

	return true;
}

bool IoRequestTranceiver::onReceive()
{
	trace_enter();

	bool readEnd = false;
	while (!readEnd) {
		trace("cnn.id=" << m_cnn->getId() << ", readState=" << m_recvState);
		if (m_closed || m_cnn->isOpen() == false) {
			return false;
		}

		switch (m_recvState) {
			case RECV_INIT:
			{
				trace("[onReceive] RECV_INIT");
				m_res.getStream()->reset();
				m_readPos = 0;
				m_recvState = RECV_HEADER;
			}
			break;

			/*
			case RECV_MAGIC:
			{
				trace("[onReceive] RECV_MAGIC");

				byte magic;
				int readLen = m_cnn->read(&magic, 1);

				IoByteUtil util;
				trace("***** MAGIC Read Len = " << readLen);

				if (readLen <= 0) {
					m_recvState = RECV_INIT;
					log_error("Connection read error : cnn.id=" << m_cnn->getId());
					return false;
				}

				if (magic == REQUEST_MAGIC) {
					m_res.getStream()->reset();
					m_readPos = 0;
					m_recvState = RECV_HEADER;
				}
				else {
					m_recvState = RECV_INIT;
					return true;
				}
			}
			break;
			*/

			case RECV_HEADER:
			{
				trace("[onReceive] RECV_HEADER : readPos=" << m_readPos << "; cnn.id=" << m_cnn->getId());
				byte *buf = m_res.getStream()->getBuffer();

				if (buf == NULL)
				{
					m_recvState = RECV_INIT;
					trace("[RECV_HEADER] stream buffer NULL");
					return false;
				}

				int size = m_res.getStream()->getSize();
				int len = size - m_readPos;

				int readLen = m_cnn->read(buf + m_readPos, len);
				trace("***** HEADER Read Len = " << readLen << "; size=" << size << "; len=" << len);
				if (readLen <= 0) {
					m_recvState = RECV_INIT;
					trace("Connection read error(REQ_HEADER) : cnn.id=" << m_cnn->getId());
					return false;
				}

				m_readPos += readLen;
				if (m_readPos < size) {
					return true;
				}

				IoByteUtil util;
				// util.PrintsBytes(buf, 0, size);
				try {
					m_res.parseHeader();
				}
				catch (Exception &ex) {
					log_error(ex);
					m_recvState = RECV_INIT;
					return true;
				}

				trace("print header-----");
				trace("***** RequestVersion =" << util.byte2char(m_res.getRequestVersion()));
				trace("      RequestType =" << util.byte2char(m_res.getRequestType()));
				trace("      RequestDirection =" << util.byte2char(m_res.getRequestDirection()));
				trace("      RequestId =" << m_res.getRequestId());
				trace("      BodySize =" << m_res.getBodySize());

				m_readPos = 0;
				m_recvState = RECV_BODY;
				readEnd = true;
			}
			break;

			case RECV_BODY:
			{
				trace("[onReceive] RECV_BODY : bodySize=" << m_res.getBodySize());

				int size = m_res.getBodySize();

				if (size <= 0) return true;

				byte *buf = m_res.getBody();
				if (buf == NULL)
				{
					m_recvState = RECV_INIT;
					trace("[RECV_BODY] stream buffer NULL");
					return false;
				}

				int len = size - m_readPos;
				int readLen = m_cnn->read(buf + m_readPos, len);
				trace("***** BODY Read Len = " << readLen);
				if (readLen <= 0) {
					m_recvState = RECV_INIT;
					log_error("Connection read error(REQ_BODY) : cnn.id=" << m_cnn->getId());
					return false;
				}

				m_readPos += readLen;
				if (m_readPos < size) {
					return true;
				}

				m_recvState = RECV_INIT;
				readEnd = true;

				processReceive();
			}
			break;

		} // end of switch
	} // end of while

	return true;
}

void IoRequestTranceiver::processReceive()
{
	trace_enter();

	m_nf.lock();

	if (m_res.getRequestType() & REQUEST_TYPE_ONEWAY)
	{
		IoRequestMap::iterator it = m_reqMap.find(m_res.getRequestId());

		if (it != m_reqMap.end())
		{
			IoRequestPtr req = it->second;

	if (req.get() != NULL) {
		m_reqMap.erase(m_res.getRequestId());
		m_nf.unlock();

		trace("******* find request : req_id = " << req->getRequestId());

		req->attatch(&m_res);

		byte *body = m_res.getBody();
		int   bodySize = m_res.getBodySize();
		m_res.setBody(NULL, 0);
		req->setResponse(body, bodySize);

		return;
	}
		}
	}

	m_nf.unlock();

	m_protocol->rpc_receive(m_cnn, m_res);
}
