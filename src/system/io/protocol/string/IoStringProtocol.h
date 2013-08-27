/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStringProtocol.h
 *
 */

#ifndef _IO_STRING_PROTOCOL_H_
#define _IO_STRING_PROTOCOL_H_

#include <map>
#include <string>

#include "system/os/OSMutex.h"
#include "system/io/protocol/IoProtocol.h"

class IoStringProtocol : public IoProtocol
{
protected :
	virtual IoTranceiver *createTransceiver(IoConnection *cnn);

public :
	IoStringProtocol();
	virtual ~IoStringProtocol();

	virtual bool string_send(const std::string &cnnAddr, const std::string &str);
	virtual bool string_receive(IoConnection *cnn, const std::string &str);
};

#endif
