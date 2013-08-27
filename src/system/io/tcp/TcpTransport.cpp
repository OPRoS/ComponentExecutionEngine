/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TcpTransport.cpp
 *
 */

#include "TcpTransport.h"

#include "TcpAcceptor.h"
#include "TcpConnector.h"

#include "system/Log.h"
#include "system/Trace.h"


TcpTransport::TcpTransport()
{
	setType("tcp");
}

TcpTransport::~TcpTransport()
{
}


IoAcceptor *TcpTransport::getAcceptor()
{
	trace_enter();
	TcpAcceptor *ta;

	if (getType()=="tcp")
	{

		ta = new TcpAcceptor();
		if (ta == NULL) {
			log_error("acceptor creation FAIL");
			return NULL;
		}
	}
	


	return ta; 
}

IoConnector *TcpTransport::getConnector()
{
	trace_enter();

	TcpConnector *tc;
	if (getType()=="tcp")
	{
		tc = new TcpConnector();
		if (tc == NULL) {
			log_error("connector creation FAIL");
			return NULL;
		}
	}


	return tc;
}
