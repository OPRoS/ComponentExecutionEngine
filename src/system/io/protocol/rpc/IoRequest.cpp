/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoRequest.cpp
 *
 */

#include "IoRequest.h"

#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

#include <climits>

#define OPROS_MAGIC_FIRST     O
#define OPROS_MAGIC_SECOND R


IoRequest::IoRequest(long timeout)
{
	m_reqVersion = REQUEST_VERSION;
	m_reqType = REQUEST_TYPE_SYNC;
	m_reqDir = REQUEST_DIR_REQUEST;
	m_reqId = -1;

	m_stream.reserve(REQUEST_HEADER_LEN);
	if (timeout <= 0) m_waitTimeout = REQUEST_TIMEOUT;
	else m_waitTimeout = timeout;
	m_returned = false;

	m_body = NULL;
	m_bodySize = 0;
}

IoRequest::~IoRequest()
{
	if (m_body != NULL) delete m_body;
}

void IoRequest::pack()
{
	m_stream.reset();
	m_stream << m_reqVersion;
	m_stream << m_reqType;
	m_stream << m_reqDir;
	m_stream << m_reqId;
	m_stream << m_bodySize;
	m_stream.putBytes(m_body, 0, m_bodySize);
	m_stream.flip();
}

void IoRequest::parseHeader()
{
	trace_enter_msg("------------- 1 , pos=" << m_stream.position() << ",lim=" << m_stream.limit());

	m_stream.reset();
	m_stream >> m_reqVersion;
	m_stream >> m_reqType;
	m_stream >> m_reqDir;
	m_stream >> m_reqId;
	m_stream >> m_bodySize;

	if (m_reqVersion != REQUEST_VERSION)
	{
		trace("Illegal Protocol");

		throw Exception("Illegal Protocol");
		return;
	}

	if (m_bodySize > 0) m_body = new byte[m_bodySize];
}


bool IoRequest::isReturned()
{
	return m_returned;
}

void IoRequest::setResponse(byte *body, int bodySize)
{
	trace_enter_msg("body_size = " << bodySize);

	if (m_body != NULL) {
		delete m_body;
	}

	m_body = body;
	m_bodySize = bodySize;

	m_returned = true;
	m_nf.notify();
}

bool  IoRequest::waitResponse()
{
	return waitResponse(m_waitTimeout);
}

bool IoRequest::waitResponse(long timeout)
{
	if (timeout <= 0) timeout = LONG_MAX; // infiite

	OSGuard guard(&m_nf);
	if (m_returned == true) {
		trace("Returned(m_returned == TRUE)");
		return true;
	}

	trace("Not Returned(m_returned == false). Wait until timeout=" << timeout);

	if (m_nf.wait(timeout)) {
		trace("WAIT OK... : returned=" << m_returned);

		return m_returned;
	}
	else {
		trace("WAIT FAILED....!!!!!");

		return false;
	}
}

void IoRequest::closeResponse()
{
	m_nf.notify();
}

void IoRequest::attatch(IoRequest *req)
{
	setRequestVersion(req->getRequestVersion());
	setRequestType(req->getRequestType());
	setRequestDirection(req->getRequestDirection());
	setRequestId(req->getRequestId());
}
