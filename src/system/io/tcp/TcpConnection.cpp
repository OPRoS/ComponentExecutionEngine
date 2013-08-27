/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpConnection.cpp
 *
 */

#include "TcpConnection.h"

#include "system/Log.h"
#include "system/Trace.h"

TcpConnection::TcpConnection()
{
	m_tcp = NULL;
	m_handle = INVALID_SOCKET;
}

TcpConnection::~TcpConnection()
{
	if (m_tcp != NULL) {
		m_tcp->close();
		delete m_tcp;
	}
}

void TcpConnection::setSocket(OSSocketTcp *tcp)
{
	if (tcp != NULL) {
		m_tcp = tcp;
		setSocketHandle(tcp->getSocketHandle());
		setId(tcp->getId());
		setOpen(true);
	}
}

bool TcpConnection::onClose()
{
	trace_enter();

	if (m_tcp == NULL) {
		log_error("TCP socket is NULL : cnn.id=" << getId());
		return true;
	}

	return m_tcp->close();
}

int TcpConnection::write(const byte *buf, size_t len)
{
	if (m_tcp == NULL) {
		log_error("TCP socket is NULL : cnn.id=" << getId());
		return -1;
	}

	return m_tcp->write((const char *) buf, len);
}

int TcpConnection::read(byte *buf, size_t len)
{
	if (m_tcp == NULL) {
		log_error("TCP socket is NULL : cnn.id=" << getId());
		return -1;
	}

	return m_tcp->read((char *) buf, len);
}

