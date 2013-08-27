/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpConnector.h
 *
 */

#if !defined(_TCPCONNECTOR_H)
#define _TCPCONNECTOR_H

#include "system/io/IoConnector.h"
#include "TcpConnection.h"

class TcpConnector : public IoConnector
{
	TcpConnection *m_cnn;
	int m_mode;

protected:
	virtual bool onOpen();
	virtual bool onClose();

public:
	TcpConnector();
	virtual ~TcpConnector();

	virtual OSSocketHandle getSocketHandle();

	virtual IoConnection *connect();

	virtual IoConnection *detachConnection();
	virtual bool attachConnection(IoConnection *cnn);
};

#endif  //_TCPCONNECTOR_H
