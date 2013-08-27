/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentAdapterRemoteProxy.cpp
 *
 */

#include "ComponentAdapterRemoteProxy.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentAdapterRemoteProxy::ComponentAdapterRemoteProxy()
{
}

ComponentAdapterRemoteProxy::ComponentAdapterRemoteProxy(const string &nid, const string &name)
{
	m_nodeId = nid;
	m_name = name;
}

ComponentAdapterRemoteProxy::~ComponentAdapterRemoteProxy()
{
}

bool ComponentAdapterRemoteProxy::connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	// 리모트 노드에 있는 컴포넌트인 경우 포트 연결 불필요
	// 리모트 노드에 있는 컴포넌트에 의해서 리모트 포트 연결이 이루어짐
	return true;
}

bool ComponentAdapterRemoteProxy::connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	// 리모트 노드에 있는 컴포넌트인 경우 포트 연결 불필요
	// 리모트 노드에 있는 컴포넌트에 의해서 리모트 포트 연결이 이루어짐
	return true;
}

bool ComponentAdapterRemoteProxy::connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	// 리모트 노드에 있는 컴포넌트인 경우 포트 연결 불필요
	// 리모트 노드에 있는 컴포넌트에 의해서 리모트 포트 연결이 이루어짐
	return true;
}


bool ComponentAdapterRemoteProxy::disconnectServicePort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	// 리모트 노드에 있는 컴포넌트인 경우 포트 연결 불필요
	// 리모트 노드에 있는 컴포넌트에 의해서 리모트 포트 연결이 이루어짐
	return true;
}

bool ComponentAdapterRemoteProxy::disconnectDataPort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	// 리모트 노드에 있는 컴포넌트인 경우 포트 연결 해제 불필요
	// 리모트 노드에 있는 컴포넌트에 의해서 리모트 포트 연결 해제가 이루어짐
	return true;
}

bool ComponentAdapterRemoteProxy::disconnectEventPort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	// 리모트 노드에 있는 컴포넌트인 경우 포트 연결 해제 불필요
	// 리모트 노드에 있는 컴포넌트에 의해서 리모트 포트 연결 해제가 이루어짐
	return true;
}
