/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : PropertyInterface.cpp
 *
 *
 */

#include "PropertyInterface.h"
#include <map>
#include <CTrace.h>

static std::string NULLString="";

class PropertyInterfaceImpl {
	typedef std::map<std::string, std::string> property_base;

	property_base m_properties;


public:
	PropertyInterfaceImpl() {
		NULLString="";
		;
	}
	virtual ~PropertyInterfaceImpl() {
		;
	}

	virtual void setProperty(const std::string &name, const std::string &value) {
		m_properties[name] = value;
	}

	virtual std::string &getProperty(const std::string &name) {
		property_base::iterator d_it = m_properties.find(name);
		if (d_it != m_properties.end()) {
			return d_it->second;
		}
		return NULLString;
	}

	virtual std::map<std::string, std::string> &getPropertyMap() {
		return m_properties;
	}
};

PropertyInterface::PropertyInterface() :
	m_impl(new PropertyInterfaceImpl) {
	// TODO Auto-generated constructor stub

}

PropertyInterface::~PropertyInterface() {
	delete m_impl;
}

void PropertyInterface::setProperty(const std::string &name,
		const std::string &value) {
	ctrace_enter();
	m_impl->setProperty(name, value);
}

std::string &PropertyInterface::getProperty(const std::string &name) {
	ctrace_enter();
	return m_impl->getProperty(name);
}

std::map<std::string, std::string> &PropertyInterface::getPropertyMap()
{
	ctrace_enter();
	return m_impl->getPropertyMap();
}

