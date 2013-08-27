/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 25
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoTransport.h
 *
 */

#ifndef _IO_TRANSPORT_H_
#define _IO_TRANSPORT_H_

#include <string>
#include <map>

#include "IoAcceptor.h"
#include "IoConnector.h"

class IoTransport
{
	std::string m_type;

public:
	IoTransport();
	virtual ~IoTransport();

	void setType(const std::string &type) { m_type = type; };
	std::string getType() { return m_type; };

	virtual IoAcceptor *getAcceptor() = 0;
	virtual IoConnector *getConnector() = 0;
};

typedef std::map<std::string, IoTransport *> IoTransportMap;

#endif // _IO_TRANSPORT_H_
