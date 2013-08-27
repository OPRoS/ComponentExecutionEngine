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

#ifndef _COMPONENT_ADAPTER_REMOTE_PROXY_H_
#define _COMPONENT_ADAPTER_REMOTE_PROXY_H_

#include <string>

#include "ComponentAdapter.h"

#include "system/Trace.h"

/**
	 원격 노드에 존재하는 컴포넌트를 참조하고자 할 때 생성되는 컴포넌트 Remote Proxy임.
 */
class ComponentAdapterRemoteProxy : public ComponentAdapter
{
	/** 해당 컴포넌트가 존재하는 원격 노드의 아이디 */
	std::string m_nodeId;

	/** 해당 컴포넌트의 아이디(이름) */
	std::string m_name;

public:
	ComponentAdapterRemoteProxy();
	ComponentAdapterRemoteProxy(const std::string &nid, const std::string &name);
	virtual ~ComponentAdapterRemoteProxy();

	virtual void setId(const std::string &name) { m_name = name; };
	virtual std::string  getId() { return m_name; };

	virtual void setNodeId(const std::string &id) { m_nodeId = id; };
	virtual std::string getNodeId() { return m_nodeId; };

	virtual ComponentAdapterType getType() { return CA_REMOTE_PROXY; };
	virtual Component *getComponent(const std::string &name) { return NULL; };

	virtual bool load() { return true; };
	virtual bool connectPort() { return true; };
	virtual bool activate() { return true; };

	virtual bool unload() { return true; };
	virtual bool disconnectPort() { return true; };
	virtual bool passivate() { trace_enter(); return true; };

	virtual bool doInitialize() { return true; };
	virtual bool doStart() { return true; };
	virtual bool doStop() { return true; };
	virtual bool doReset() { return true; };
	virtual bool doStopOnError() { return true; };
	virtual bool doRecover() { return true; };
	virtual bool doDestroy() { return true; };

	virtual bool doExecute() { return true; };
	virtual bool doEvent(Event *evt) { return true; };

	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool disconnectServicePort(PortConnectionInfo &info);
	virtual bool disconnectDataPort(PortConnectionInfo &info);
	virtual bool disconnectEventPort(PortConnectionInfo &info);

protected :
	virtual bool doLoad() { return true; };
	virtual bool doUnload() { return true; };

	virtual bool doConnectPort() { return true; };
	virtual bool doDisconnectPort()  { return true; };

	virtual bool doActivate()  { return true; };
	virtual bool doPassivate()  { return true; };
};

#endif
