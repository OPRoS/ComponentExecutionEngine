/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpTransport.h
 *
 */

#ifndef _TCP_TRANSPORT_H_
#define _TCP_TRANSPORT_H_

#include "system/io/IoTransport.h"

// TCPTransport
class TcpTransport : public IoTransport
{
public:
	TcpTransport();
	virtual ~TcpTransport();

	virtual IoAcceptor *getAcceptor();
	virtual IoConnector *getConnector();
};

#endif // _TCP_TRANSPORT_H_
