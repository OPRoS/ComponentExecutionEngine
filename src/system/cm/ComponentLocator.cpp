/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentLocator.cpp
 *
 */


#include "ComponentLocator.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentLocator::ComponentLocator()
{
}

ComponentLocator::~ComponentLocator()
{
}

void ComponentLocator::addComponentLocation(const string &compId, const string &nodeId)
{
	trace_enter_msg("compId=" << compId << ", nodeId = " << nodeId);

	m_compMap[compId] = nodeId;
}

string ComponentLocator::getComponentLocation(const string &compId)
{
	trace_enter_msg("compId=" << compId);

	StringMap::const_iterator pos = m_compMap.find(compId);
	if (pos != m_compMap.end()) {
		return pos->second;
	}
	else {
		return "";
	}
}

void ComponentLocator::removeComponentLocation(const string &compId)
{
	m_compMap.erase(compId);
}

