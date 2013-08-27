/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpAcceptor.cpp
 *
 */

#include "TcpAcceptor.h"
#include "TcpConnection.h"

#include "system/util/BasicTypes.h"

#include "system/Log.h"
#include "system/Trace.h"

TcpAcceptor::TcpAcceptor()
{
	m_handle = INVALID_SOCKET;
}

TcpAcceptor::~TcpAcceptor()
{
	m_tcp.close();
}

bool TcpAcceptor::onOpen()
{
	trace_enter();

	Properties *props = getProperties();
	if (m_tcp.open(*props) && m_tcp.bind() && m_tcp.listen()) {
		setId(m_tcp.getId());
		setSocketHandle(m_tcp.getSocketHandle());
		return true;
	}
	return false;
}

bool TcpAcceptor::onClose()
{
	trace_enter();

	return m_tcp.close();
}

IoConnection *TcpAcceptor::accept()
{
	trace_enter();

	OSSocketTcp *tcp = portable_dynamic_cast(OSSocketTcp *, m_tcp.accept());
	if (tcp == NULL) return NULL;

	TcpConnection *cnn = new TcpConnection();
	cnn->setParent(this);
	cnn->setSocket(tcp);

	return cnn;
}
