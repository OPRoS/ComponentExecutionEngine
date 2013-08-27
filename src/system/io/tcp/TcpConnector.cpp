/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpConnector.cpp
 *
 */

#include "TcpConnector.h"

#include "system/util/BasicTypes.h"

#include "system/Log.h"
#include "system/Trace.h"

TcpConnector::TcpConnector()
{
	m_cnn = NULL;
}

TcpConnector::~TcpConnector()
{
	if (m_cnn != NULL) {
		m_cnn->close();
		delete m_cnn;
		m_cnn = NULL;
	}
}

OSSocketHandle TcpConnector::getSocketHandle()
{
	if (m_cnn != NULL) {
		return m_cnn->getSocketHandle();
	}
	else {
		return INVALID_SOCKET;
	}
}

bool TcpConnector::onOpen()
{
	trace_enter();

	OSSocketTcp *tcp = new OSSocketTcp();
	Properties *props = getProperties();
	if (!tcp->open(*props)) {
		delete tcp;
		log_error("tcp open error : cnn=" << *this);
		return false;
	}
	m_cnn = new TcpConnection();
	m_cnn->setParent(this);
	m_cnn->setSocket(tcp);
	setId(tcp->getId());

	return true;
}

bool TcpConnector::onClose()
{
	trace_enter();

	if (m_cnn == NULL) {
		log_error("tcp cnn is NULL : cnn=" << *this);
		return true;
	}

	if (m_cnn->close()) {
		delete m_cnn;
		m_cnn = NULL;
		return true;
	}
	else {
		log_error("tcpCnn close error : cnn=" << *this);
		return false;
	}
}

IoConnection *TcpConnector::connect()
{
	trace_enter();

	if (m_cnn == NULL) {
		log_error("tcp cnn is NULL : cnn=" << *this);
		return NULL;
	}

	OSSocketTcp *tcp = m_cnn->getSocket();
	if (!tcp->connect()) {
		log_error("tcp.socket connecting FAIL : cnn=" << *this);
		return NULL;
	}

	log_info("tcp.Connect success : tcp.id=" << tcp->getId() << ", cnn.node_id=" << m_cnn->getId());

	return m_cnn;
}

IoConnection *TcpConnector::detachConnection()
{
	trace_enter();

	IoConnection *tmp = m_cnn;
	m_cnn = NULL;
	return tmp;
}

bool TcpConnector::attachConnection(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("tcp cnn is NULL");
		return false;
	}

	TcpConnection *tmp = portable_dynamic_cast(TcpConnection *, cnn);
	if (tmp == NULL) {
		log_error("given cnn is not TcpConnection : cnn.id=" << cnn);
		return false;
	}

	// close previous connecion
	close();

	m_cnn = tmp;
	setOpen(true);

	return true;
}
