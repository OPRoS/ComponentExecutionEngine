/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpAcceptor.h
 *
 */

#ifndef _TCP_ACCEPTOR_H_
#define _TCP_ACCEPTOR_H_

#include "system/io/IoAcceptor.h"
#include "system/os/OSIncludes.h"

class TcpAcceptor : public IoAcceptor
{
protected:
	int m_mode;
	OSSocketTcp m_tcp;
	OSSocketHandle m_handle;

	virtual bool onOpen();
	virtual bool onClose();

public:
	TcpAcceptor();
	virtual ~TcpAcceptor();

	void setSocketHandle(OSSocketHandle h) { m_handle = h; };
	virtual OSSocketHandle getSocketHandle() { return m_handle; };

	virtual IoConnection *accept();
};

#endif  //_TCP_ACCEPTOR_H_
