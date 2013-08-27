/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : Component.cpp
 *
 *
 */

#include "Component.h"

Component::Component() {
	// TODO Auto-generated constructor stub

}

Component::Component(const std::string &name) :m_compId(name)
{

}

Component::~Component() {
	// TODO Auto-generated destructor stub
}

void Component::setId(std::string id)
{
	m_compId = id;
}

std::string Component::getId()
{
	return m_compId;
}

ReturnType Component::onInitialize()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onStart()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onStop()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onReset()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onError()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onRecover()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onDestroy()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onEvent(Event *evt)
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onExecute()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onUpdated()
{
	return OPROS_UNSUPPORTED_METHOD;
}

ReturnType Component::onPeriodChanged()
{
	return OPROS_UNSUPPORTED_METHOD;
}
