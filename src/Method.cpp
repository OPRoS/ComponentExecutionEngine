/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : Service.cpp
 *
 *
 */

#include "Method.h"
#include <sstream>

Method::Method() {
	m_arc = NULL;
}

Method::Method(const std::string &name) :
	m_name(name) {
	m_arc = NULL;
}

Method::~Method() {
}

std::string &Method::getName()
{
	return m_name;
}

void Method::setArchiveFactory(ArchiveFactory *darc) {
	m_arc = darc;
}

ReturnType Method::call(std::string &invalue, std::string &outvalue) {
	if (m_arc == NULL) {
		return OPROS_UNSUPPORTED_METHOD;
	}
	std::istringstream instr(invalue);
	std::ostringstream outstr;

	opros::archive::iarchive *inarc = m_arc->getInputArchive(&instr);

	opros::archive::oarchive *outarc = m_arc->getOutputArchive(&outstr);

	ReturnType ret = call(*inarc, *outarc);

	outvalue = outstr.str();

	m_arc->release(inarc);
	m_arc->release(outarc);

	return ret;
}
