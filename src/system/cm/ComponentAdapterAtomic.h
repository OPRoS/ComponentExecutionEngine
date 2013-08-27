/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentAdapterAtomic.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_ATOMIC_H_
#define _COMPONENT_ADAPTER_ATOMIC_H_

#include <vector>

#include "system/profile/ComponentProfile.h"
#include "executor/Executor.h"
#include "ComponentLoader.h"
#include "ComponentAdapter.h"

#include "system/os/OSMutex.h"

class Component;

/**
	단일(Atomic) 컴포넌트 및 관련 정보를 저장하고 있는 클래스.
 */
class ComponentAdapterAtomic : public ComponentAdapter
{
	typedef std::map<std::string , DataPortLink *> DataPortLinkMap;
	typedef std::map<std::string , EventPortLink *> EventPortLinkMap;

#ifdef COORD
#error depend list
	typedef std::list<std::string> DependListType;

	/** 해당 컴포넌트에 앞서 실행되어야 할 컴포넌트 리스트 **/
	DependListType m_depends;
#endif

	/** 데이터 포트와 이에 속한 데이터 포트 링크 정보를 저장하고 있는 Map */
	DataPortLinkMap m_dataPortMap;

	/**이벤트 포트와 이에 속한 이벤트 포트 링크 정보를 저장하고 있는 Map */
	EventPortLinkMap m_eventPortMap;

	/** 컴포넌트 로더 객체 : DLL, SO 파일을 관리 */
	ComponentLoader *m_loader;

	/** 컴포넌트 프로파일내의 정보를 저장하고 있는 객체 */
	ComponentProfile *m_profile;

	/** OPRoS 컴포넌트 객체 */
	Component *m_component;

	int m_executorId;

public:
	ComponentAdapterAtomic();
	virtual ~ComponentAdapterAtomic();

	/** 컴포넌트 id를 반환함 */
	std::string  getId() {
		if (m_profile != NULL) return m_profile->getName();
		else return "";
	};

	/** 컴포넌트의 상태를 스트링 형태로 반환함 */
	virtual std::string stateToStr();

	void setExecutorId(int eid) { m_executorId = eid; };
	int getExecutorId() { return m_executorId; };

	/**
		 컴포넌트의 타입을 반환 함.
		  - 단일 컴포넌트 : CA_ATOMIC
		  - 컴포짓 컴포넌트 : CA_COMPOSITE
	 */
	virtual ComponentAdapterType getType() { return CA_ATOMIC; };

	/** 컴포넌트 로더 객체 반환 */
	ComponentLoader *getComponentLoader() { return m_loader; };

	/** 컴포넌트 로더 객체 세팅  */
	void setComponentLoader(ComponentLoader *loader) { m_loader = loader; };

	/** OPRoS 컴포넌트  객체 반환 */
	Component *getComponent() { return m_component; };

	/** OPRoS 컴포넌트  객체 세팅 */
	void setComponent(Component *component) { m_component = component;	};

	/** 컴포넌트 프로파일 반환 */
	ComponentProfile *getAtomicComponentProfile() { return m_profile;	};

	/** 컴포넌트 프로파일 세팅 */
	void setAtomicComponentProfile(ComponentProfile *profile) { m_profile = profile; };

	virtual bool doInitialize();
	virtual bool doStart();
	virtual bool doStop();
	virtual bool doReset();
	virtual bool doStopOnError();
	virtual bool doRecover();
	virtual bool doDestroy();

	virtual bool doExecute();
	virtual bool doEvent(Event *evt);

	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName);
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName);

	virtual DataPort *getOutputDataPort(const std::string  &portName);
	virtual DataPort *getInputDataPort(const std::string  &portName);
	virtual DataPortLink *getDataPortLink(const std::string  &portName);
	virtual void addDataPortLink(const std::string  &portName, DataPortLink *link);
	virtual void removeDataPortLink(const std::string  &portName);

	virtual EventPort *getOutputEventPort(const std::string  &portName);
	virtual EventPort *getInputEventPort(const std::string  &portName);
	virtual EventPortLink *getEventPortLink(const std::string  &portName);
	virtual void addEventPortLink(const std::string  &portName, EventPortLink *link);
	virtual void removeEventPortLink(const std::string  &portName);

	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info);
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info);

	virtual bool disconnectServicePort(PortConnectionInfo &info);
	virtual bool disconnectDataPort(PortConnectionInfo &info);
	virtual bool disconnectEventPort(PortConnectionInfo &info);

protected :
	virtual bool doLoad();
	virtual bool doUnload();

	virtual bool doConnectPort() { return true; };
	virtual bool doDisconnectPort() { return true; };

	virtual bool doActivate();
	virtual bool doPassivate();
};

#endif
