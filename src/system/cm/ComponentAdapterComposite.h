/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentAdapterComposite.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_COMPOSITE_H_
#define _COMPONENT_ADAPTER_COMPOSITE_H_

#include <vector>

#include "CompositeInputDataPort.h"
#include "CompositeInputEventPort.h"
#include "system/profile/CompositeComponentProfile.h"
#include "ComponentAdapter.h"

#include "system/os/OSMutex.h"

class Component;

typedef std::map<std::string, CompositeInputDataPort *> CompositeInputDataPortList;
typedef std::map<std::string, CompositeInputEventPort *> CompositeInputEventPortList;

/**
	컴포짓(Composite) 컴포넌트 및 관련 정보를 저장하고 있는 클래스.
 */
class ComponentAdapterComposite : public ComponentAdapter
{
	/** 컴포짓 컴포넌트에 속한 하위 컴포넌트 리스트 */
	ComponentAdapterList m_caList;

	CompositeInputDataPortList m_dataport_list;
	CompositeInputEventPortList m_eventport_list;


	/** 컴포짓 컴포넌트 프로파일 정보 */
	CompositeComponentProfile *m_profile;

	/** 컴포넌트 추가/삭제 시 락킹을 위한 뮤텍스 */
	OSMutex m_lock;

public:
	ComponentAdapterComposite();
	virtual ~ComponentAdapterComposite();

	/**  컴포짓 컴포넌트 아이디(이름) 반환 */
	std::string  getId() {
		if (m_profile != NULL) return m_profile->getName();
		else return "";
	};

	/**
		컴포짓  컴포넌트의 타입을 반환 함.
		  - 단일 컴포넌트 : CA_ATOMIC
		  - 컴포짓 컴포넌트 : CA_COMPOSITE
	 */
	virtual ComponentAdapterType getType() { return CA_COMPOSITE; };

	/** 컴포짓 컴포넌트 프로파일 반환 */
	CompositeComponentProfile *getCompositeComponentProfile() { return m_profile;	};

	/** 컴포짓 컴포넌트 프로파일 세팅 */
	void setCompositeComponentProfile(CompositeComponentProfile *profile) { m_profile = profile; };

	/** 컴포짓 컴포넌트에 속한 하위 컴포넌트 리스트 반환  */
	ComponentAdapterList &getComponentAdapterList() { return m_caList; };

	/**
		 컴포짓 컴포넌트에 속한 하나의 컴포넌트를 찾아서 반환.

		 @param name 찾을 하위 컴포넌트 아이디(이름)

		 @return ComponentAdapter *찾은 하위 컴포넌트
	*/
	ComponentAdapterPtr getChildComponent(const std::string  &name);

	virtual bool doInitialize();
	virtual bool doStart();
	virtual bool doStop();
	virtual bool doReset();
	virtual bool doStopOnError();
	virtual bool doRecover();
	virtual bool doDestroy();

	virtual bool doExecute();
	virtual bool doEvent(Event *evt);

	/** @see ComponentAdapter::findComponentAdapter() */
	virtual ComponentAdapterPtr findComponentAdapter(const std::string &portName, \
		ComponentPortType portType, ComponentPortDir portDir);

	/** @see ComponentAdapter::getRequiredServicePort() */
	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName);

	/** @see ComponentAdapter::getProvidedServicePort() */
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName);

	/** @see ComponentAdapter::getOutputDataPort() */
	virtual DataPort *getOutputDataPort(const std::string  &portName);

	/** @see ComponentAdapter::getInputDataPort() */
	virtual DataPort *getInputDataPort(const std::string  &portName);

	/** @see ComponentAdapter::getOutputEventPort() */
	virtual EventPort *getOutputEventPort(const std::string  &portName);

	/** @see ComponentAdapter::getInputEventPort() */
	virtual EventPort *getInputEventPort(const std::string  &portName);

	/** @see ComponentAdapter::connectServicePort() */
	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info);

	/** @see ComponentAdapter::connectDataPort() */
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info);

	/** @see ComponentAdapter::connectEventPort() */
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info);

	/** @see ComponentAdapter::disconnectServicePort() */
	virtual bool disconnectServicePort(PortConnectionInfo &info);

	/** @see ComponentAdapter::disconnectDataPort() */
	virtual bool disconnectDataPort(PortConnectionInfo &info);

	/** @see ComponentAdapter::disconnectEventPort() */
	virtual bool disconnectEventPort(PortConnectionInfo &info);

protected :
	/** @see ComponentAdapter::doLoad() */
	virtual bool doLoad();

	/** @see ComponentAdapter::doUnload() */
	virtual bool doUnload();

	/** @see ComponentAdapter::doConnectPort() */
	virtual bool doConnectPort();

	/** @see ComponentAdapter::doDisconnectPort() */
	virtual bool doDisconnectPort();

	/** @see ComponentAdapter::doActivate() */
	virtual bool doActivate();

	/** @see ComponentAdapter::doPassivate() */
	virtual bool doPassivate();

	/**
		 컴포짓 컴포넌트에서 export된 포트 정보를 반환하는 함수.

		 @param exportPortName export된 포트의 이름
		 @param portType export된 포트 타입
		 @param portDir export된 포트의 반향 (Required/Provided, Input/Output)

		 @return ExportPortInfo *찾은 포트 연결 정보
	*/
	ExportPortInfo *getExportPortInfo(const std::string &exportPortName, ComponentPortType portType, ComponentPortDir portDir);

	std::list<ExportPortInfo *> getExportPortInfoList(const std::string &exportPortName, ComponentPortType portType, ComponentPortDir portDir);															 

	/**
		 자기 자신에게 export된 포트 간의 연결을 수행하는 함수.
		 doConnectPort 함수에서는 자기 자신 뿐만 아니라,
		 자신이 포함하고 있는  하위 컴포넌트의 doConnectPort()를 재귀적으로 호출함.
	*/
	bool connectPortThis();

	/** 
		자기 자신에게 export된 포트 간의 연결을 해제하는  함수 
		 doDisconnectPort 함수에서는 자기 자신 뿐만 아니라,
		 자신이 포함하고 있는  하위 컴포넌트의 doDisconnectPort()를 재귀적으로 호출함.
	*/
	bool disconnectPortThis();
};

#endif
