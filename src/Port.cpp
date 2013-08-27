/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : Port.cpp
 *
 *
 */

#include "Port.h"

Port::Port() {
	// TODO Auto-generated constructor stub

}

Port::Port(const std::string &name, const PortType &type) {
	m_name = name;
	m_type = type;
}

Port::~Port() {
	// TODO Auto-generated destructor stub
}

PortType Port::getType() {
	return m_type;
}
std::string &Port::getName() {
	return m_name;
}
