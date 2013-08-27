/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentAdapterLocalProxy.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_LOCAL_PROXY_H_
#define _COMPONENT_ADAPTER_LOCAL_PROXY_H_

#include "ComponentAdapter.h"

/**
	 다른 응용에 의해 생성된 컴포넌트를 사용하고자 할 때 생성되는 컴포넌트 Local Proxy임
	 대부분의 함수 호출은 원래 컴포넌트로 전달되어 수행됨.
	 응용에서 해당 컴포넌트를 삭제하면 다른 응용에서 생성된 컴포넌트가 삭제되는 것이 
	 아니라 Local Proxy가 삭제됨
 */
class ComponentAdapterLocalProxy : public ComponentAdapter
{
	ComponentAdapter *m_ca;

public:
	ComponentAdapterLocalProxy(ComponentAdapter *ca) : m_ca(ca) {};
	virtual ~ComponentAdapterLocalProxy() {};

	virtual ComponentAdapterType getType() { return CA_LOCAL_PROXY; };
	virtual std::string  getId() { return m_ca->getId(); };
	virtual Component *getComponent() { return m_ca->getComponent(); };

	ComponentAdapterStatus getComponentAdapterStatus() { return m_ca->getComponentAdapterStatus(); };
	void setComponentAdapterStatus(ComponentAdapterStatus status) { m_ca->setComponentAdapterStatus(status); };
	std::string statusToStr() { return m_ca->statusToStr(); };

	void incActiveCount() { m_ca->incActiveCount(); };
	void decActiveCount() { m_ca->decActiveCount(); };
	bool isActive() { return m_ca->isActive(); };

	void incRefCount() { m_ca->incRefCount(); };
	void decRefCount() { m_ca->decRefCount(); };
	bool isRef() { return m_ca->isRef(); };

	OSMutex *getLock() { return m_ca->getLock(); };
	void lock() { m_ca->lock(); };
	void unlock() { m_ca->unlock(); };

	virtual bool load() { incRefCount(); return true; };
	virtual bool connectPort() { return m_ca->connectPort(); };
	virtual bool activate() { incActiveCount(); return true; };

	virtual bool unload() { return m_ca->unload(); };
	virtual bool disconnectPort() { return m_ca->disconnectPort(); };
	virtual bool passivate() { return m_ca->passivate(); };

	virtual bool doInitialize() { return m_ca->doInitialize(); };
	virtual bool doStart() { return m_ca->doStart(); };
	virtual bool doStop() { return m_ca->doStop(); };
	virtual bool doReset() { return m_ca->doReset(); };
	virtual bool doStopOnError() { return m_ca->doStopOnError(); };
	virtual bool doRecover() { return m_ca->doRecover(); };
	virtual bool doDestroy() { return m_ca->doDestroy(); };

	virtual bool doExecute() { return m_ca->doExecute(); };
	virtual bool doEvent(Event *evt) { return m_ca->doEvent(evt); };

	virtual ComponentAdapterPtr findComponentAdapter(const std::string &portName, ComponentPortType portType, ComponentPortDir portDir) {
			return m_ca->findComponentAdapter(portName, portType, portDir);
	};

	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName) { return m_ca->getRequiredServicePort(portName); };
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName) { return m_ca->getProvidedServicePort(portName); };

	virtual DataPort *getOutputDataPort(const std::string  &portName) { return m_ca->getOutputDataPort(portName); };
	virtual DataPort *getInputDataPort(const std::string  &portName) { return m_ca->getInputDataPort(portName); };
	virtual DataPortLink *getDataPortLink(const std::string  &portName) { return m_ca->getDataPortLink(portName); };
	virtual void addDataPortLink(const std::string  &portName, DataPortLink *link) {};
	virtual void removeDataPortLink(const std::string  &portName) {};

	virtual EventPort *getOutputEventPort(const std::string  &portName) { return m_ca->getOutputEventPort(portName); };
	virtual EventPort *getInputEventPort(const std::string  &portName) { return m_ca->getInputEventPort(portName); };
	virtual EventPortLink *getEventPortLink(const std::string  &portName)  { return m_ca->getEventPortLink(portName); };
	virtual void addEventPortLink(const std::string  &portName, EventPortLink *link) {};
	virtual void removeEventPortLink(const std::string  &portName) {};

	virtual bool connectServicePort(ComponentAdapter *target, PortConnectionInfo &info) {
		return m_ca->connectServicePort(target, info);
	};
	virtual bool connectDataPort(ComponentAdapter *target, PortConnectionInfo &info) {
		return m_ca->connectDataPort(target, info);
	};
	virtual bool connectEventPort(ComponentAdapter *target, PortConnectionInfo &info) {
		return m_ca->connectEventPort(target, info);
	};
	virtual bool disconnectServicePort(PortConnectionInfo &info) { return m_ca->disconnectServicePort(info); };
	virtual bool disconnectDataPort(PortConnectionInfo &info)   { return m_ca->disconnectDataPort(info); };
	virtual bool disconnectEventPort(PortConnectionInfo &info)  { return m_ca->disconnectEventPort(info); };

protected :
	virtual bool doLoad() { return true; };
	virtual bool doUnload() { return true; };

	virtual bool doConnectPort() { return true; };
	virtual bool doDisconnectPort() { return true; };

	virtual bool doActivate() { return true; };
	virtual bool doPassivate() { return true; };
};

#endif
