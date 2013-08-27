/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : DataPort.cpp
 *
 *
 */

#include "DataPort.h"
#include <iostream>
#include <CTrace.h>
DataPort::DataPort() {
	m_type = OPROS_DATA;
}

DataPort::DataPort(const std::string &name) :
	Port(name, OPROS_DATA) {
	// TODO Auto-generated constructor stub

}

DataPort::~DataPort() {
	ctrace("dataport delete:"<<getName()<<std::endl);
}
