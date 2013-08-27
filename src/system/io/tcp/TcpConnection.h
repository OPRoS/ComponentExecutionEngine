/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpConnection.h
 *
 */

#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include "system/io/IoConnection.h"
#include "system/os/OSIncludes.h"


class TcpConnection : public IoConnection
{
	OSSocketTcp *m_tcp;
	OSSocketHandle m_handle;

public:
	TcpConnection();
	virtual ~TcpConnection();

	void setSocketHandle(OSSocketHandle h) { m_handle = h; };
	virtual OSSocketHandle getSocketHandle() { return m_handle; };

	void setSocket(OSSocketTcp *tcp);
	OSSocketTcp *getSocket() { return m_tcp; };

	virtual bool onClose();

	virtual int write(const byte *buf, size_t len);
	virtual int read(byte *buf, size_t len);
};

#endif  //_TCP_CONNECTION_H_
