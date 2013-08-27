/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : EventPort.cpp
 *
 *
 */

#include "EventPort.h"
#include <CTrace.h>
EventPort::EventPort() {
	m_type = OPROS_EVENT;
}

EventPort::EventPort(const std::string &name) :
	Port(name, OPROS_EVENT) {
	// TODO Auto-generated constructor stub

}

EventPort::~EventPort() {
	ctrace("eventport delete:"<<getName()<<std::endl);
}
