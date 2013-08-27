/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : EventPortConnector.h
 *
 *
 */

#ifndef EVENTPORTCONNECTOR_H_
#define EVENTPORTCONNECTOR_H_
#include "ReturnType.h"

class EventPortConnector {
public:
	EventPortConnector(){};
	virtual ~EventPortConnector(){};

	virtual ReturnType sendEvent(unsigned char *data, int len) = 0;
	virtual ReturnType recvEvent(unsigned char *data, int len) = 0;
};

#endif /* EVENTPORTCONNECTOR_H_ */
