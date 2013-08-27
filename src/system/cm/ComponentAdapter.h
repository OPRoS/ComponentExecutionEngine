/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentAdapter.cpp
 *
 */

#ifndef _COMPONENT_ADAPTER_H_
#define _COMPONENT_ADAPTER_H_

#include <map>
#include <opros_smart_ptr.h>

#include "ServicePort.h"
#include "RequiredServicePort.h"
#include "ProvidedServicePort.h"

#include "DataPort.h"
#include "InputDataPort.h"
#include "OutputDataPort.h"
#include "DataPortLink.h"

#include "EventPort.h"
#include "InputEventPort.h"
#include "OutputEventPort.h"
#include "EventPortLink.h"
#include "Event.h"

#include "system/profile/PortConnectionInfo.h"
#include "system/SystemTypes.h"
#include "system/os/OSMutex.h"
#include "system/cm/CoordinationUnit.h"

class Component;
class ComponentAdapter;

typedef opros::smart_ptr<ComponentAdapter> ComponentAdapterPtr;

/**
	 컴포넌트 및 관련 정보를 저장하고 있는 클래스.
 */
class ComponentAdapter
{
	ComponentAdapterStatus m_caStatus;

	int m_activeCount;
	int m_refCount;
	int m_useCount;

	std::string m_parent_id;
	std::string m_unique_id;
	


	/** 컴포넌트에서 Follower와 Trigger는 같은 레벨만 가능함. 같은 깊이에서만 가능 */


	OSMutex m_lock;

protected:
	CoordinationUnit m_coord;
	bool m_execute_state;

public:
	ComponentAdapter();
	virtual ~ComponentAdapter();


	virtual bool getExecuteState() { return m_execute_state; }
	virtual void setExecuteState( bool v) { m_execute_state = v; }

	virtual CoordinationUnit *getCoordinationUnit();

	virtual void setParentID(const std::string &id);

	virtual std::string &getParentID();

	/** 컴포넌트 상태를 스트링 형태로 변환하여 반환 */
	virtual std::string stateToStr() { return "Unknown"; };

	/** 컴포넌트 어댑터의 타입 반환 : INVALID, ATOMIC, COMPOSITE, REMOTE_PROXY, LOCAL_PROXY */
	virtual ComponentAdapterType getType() = 0;

	/** 컴포넌트 id 반환 */
	virtual std::string  getId() = 0;

	virtual std::string getUniqueID();

	virtual void reviseUniqueID();

	/** 컴포넌트 반환 */
	virtual Component *getComponent() { return NULL; };

	/** 컴포넌트 어댑터의 상태 반환 : CA_CREATED, CA_LOADED, CA_CONNECTED, CA_ACTIVATED */
	ComponentAdapterStatus getComponentAdapterStatus() { return m_caStatus; };

	/** 컴포넌트 어댑터 상태 세팅 : CA_CREATED, CA_LOADED, CA_CONNECTED, CA_ACTIVATED */
	void setComponentAdapterStatus(ComponentAdapterStatus status) { m_caStatus = status; };

	/** 컴포넌트 어댑터의 상태를 스트링 형태로 반환: CREATED, LOADED, CONNECTED, ACTIVATED */
	std::string statusToStr();

	/**
		Active 카운트를 증가 시키는 함수
			- Active 카운드 : 해당 컴포넌트가 여러 응용에 의해 공유되어 실행이 시작된 경우
			  해당 컴포넌트를 시작시킨 횟수를 나타냄
			- Active 카운트가 0이 되었을 때문 해당 컴포넌트의 실행을 중지 시킴
	*/
	void incActiveCount() { ++m_activeCount; };

	/**
		Active 카운트를 감소 시키는 함수
			- Active 카운드 : 해당 컴포넌트가 여러 응용에 의해 공유되어 실행이 시작된 경우
			  해당 컴포넌트를 시작시킨 횟수를 나타냄
			- Active 카운트가 0이 되었을 때문 해당 컴포넌트의 실행을 중지 시킴
	*/
	void decActiveCount() { --m_activeCount; };

	int getActiveCount() { return m_activeCount; };

	/**
		컴포넌트가 Active인지를 나타내는 함수.
		Active 카운트가 0보다 크면 Active상태임.
	*/
	bool isActive() { return m_activeCount > 0; };

	/**
		Reference 카운트를 증가 시키는 함수
			- Reference 카운드 : 해당 컴포넌트가 여러 응용에 의해 공유되는 경우
			  해당 컴포넌트를 레퍼런스하는 횟수를 나타냄
			- Reference 카운트가 0이 되었을 때문 해당 컴포넌트를 메모리에서 삭제(unload)함
	*/
	void incRefCount() { ++m_refCount; };

	/**
		Reference 카운트를 감소 시키는 함수
			- Reference 카운드 : 해당 컴포넌트가 여러 응용에 의해 공유되는 경우
			  해당 컴포넌트를 레퍼런스하는 횟수를 나타냄
			- Reference 카운트가 0이 되었을 때문 해당 컴포넌트를 메모리에서 삭제(unload)함
	*/
	void decRefCount() { --m_refCount; };

	int getRefCount() { return m_refCount; };

	/**
		해당 컴포넌트가 여러 응용에 의해 공유되는지 여부를 나타내는 함수.
		 Reference 카운트가 0보다 크면 아직 레퍼런스되고 있음
	*/
	bool isRef() { return m_refCount > 0; };

	/** 컴포넌트 접근 시 동시성 제어를 위한 락 */
	OSMutex *getLock() { return &m_lock; };
	void lock() { m_lock.lock(); };
	void unlock() {m_lock.unlock(); };
	void IncUseCount() { ++m_useCount; }
	void decUseCount() { --m_useCount; }
	int getUseCount() { return m_useCount; }
	bool isUsed() { return m_useCount>0; }

	/**
		DLL(SO)을 로드하고 컴포넌트를 생성하는 함수.
		상태 체크를 통해 로드가 가능한 상태인 경우에만 수행.
		컴포넌트의 initialize() 함수 호출
	*/
	virtual bool load();

	/**
		컴포넌트 간의 포트를 연결하기 위한 함수.
		상태 체크를 통해 포트 연결이 가능한 상태인 경우에만 수행.
		컴포짓 컴포넌트만 해당됨.
	*/
	virtual bool connectPort();

	/**
		컴포넌트의 실행을 시작시키는 함수.
		컴포넌트 프로파일에 <lifecycle_type>이 "start" 인 경우만
		컴포넌트의 start() 함수를 호출함.
		상태 체크를 통해 실행 시작이 가능한 상태인 경우에만 수행.
	*/
	virtual bool activate();

	/**
		컴포넌트를 메모리에서 삭제하고 DLL(SO)을 언로드하는 함수.
		컴포넌트의 destroy() 함수 호출.
		상태 체크를 통해 언로드가 가능한 상태인 경우에만 수행.
	*/
	virtual bool unload();

	/**
		컴포넌트 간의 포트 연결을 해제하는 함수.
		컴포짓 컴포넌트만 해당됨.
		상태 체크를 통해 포트 연결 해제가 가능한 상태인 경우에만 수행.
	*/
	virtual bool disconnectPort();

	/**
		컴포넌트의 실행을 중지시키는 함수.
		상태 체크를 통해 실행 중지가 가능한 상태인 경우에만 수행.
	*/
	virtual bool passivate();

	/**
		컴포넌트의 initialize() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 initialize() 함수 호출.
		passive 컴포넌트인 경우만 호출됨.
		그외는 Executor에 의해 호출됨.
	*/
	virtual bool doInitialize() = 0;

	/**
		컴포넌트의 start() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 start() 함수 호출
		passive 컴포넌트인 경우만 호출됨.
		그외는 Executor에 의해 호출됨.
	*/
	virtual bool doStart() = 0;

	/**
		컴포넌트의 stop() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 stop() 함수 호출
		passive 컴포넌트인 경우만 호출됨.
		그외는 Executor에 의해 호출됨.
	*/
	virtual bool doStop() = 0;

	/**
		컴포넌트의 reset() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 reset() 함수 호출
		passive 컴포넌트인 경우만 호출됨.
		그외는 Executor에 의해 호출됨.
	*/
	virtual bool doReset() = 0;

	/**
		컴포넌트의 stopOnError() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 stopOnError() 함수 호출
	*/
	virtual bool doStopOnError() = 0;

	/**
		컴포넌트의 recover() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 recover() 함수 호출
	*/
	virtual bool doRecover() = 0;

	/**
		컴포넌트의 destroy() 함수 호출.
		컴포짓 컴포넌트인 경우 해당 컴포짓 컴포넌트에 속한
		모든 컴포넌트의 destroy() 함수 호출
		passive 컴포넌트인 경우만 호출됨.
		그외는 Executor에 의해 호출됨.
	*/
	virtual bool doDestroy() = 0;

	/**
		컴포넌트의 execute() 함수 호출. (현재 사용되지 않음)
		컴포넌트의 execute() 함수는 Executor에 의해 호출됨.
	*/
	virtual bool doExecute() = 0;

	/**
		컴포넌트의 event() 함수 호출.
	*/
	virtual bool doEvent(Event *evt) = 0;

	/**
		컴포짓 컴포넌트에서  export된 포트와 연결된 컴포넌트를 찾는 함수.
		단일 컴포넌트의 경우 자기 자신을 반환

		@param portName export된 포트의 이름
		@param portType export된 포트의 타입 (Service, Data, Event)
		@param portDir  export된 포트의 방향 (required/provided, input/output)

		@return ComponentAdapter * 찾은  컴포넌트
	*/
	virtual ComponentAdapterPtr findComponentAdapter(const std::string &portName, \
		ComponentPortType portType, ComponentPortDir portDir) { return this; };

	/**
		컴포넌트에 속한 Required Service Port를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName 찾고자 하는 Service Port 이름

		@return RequiredServicePort * 찾은  Required Service Port
	*/
	virtual RequiredServicePort *getRequiredServicePort(const std::string  &portName) { return NULL; };

	/**
		컴포넌트에 속한 Provided Service Port를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName 찾고자 하는 Service Port 이름

		@return RequiredServicePort * 찾은  Provided Service Port
	*/
	virtual ProvidedServicePort *getProvidedServicePort(const std::string  &portName) { return NULL; };

	/**
		컴포넌트에 속한 Output DataPort를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName 찾고자 하는DataPort 이름

		@return DataPort * 찾은  Output DataPort
	*/
	virtual DataPort *getOutputDataPort(const std::string  &portName) { return NULL; };

	/**
		컴포넌트에 속한 Input DataPort를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName 찾고자 하는 DataPort 이름

		@return DataPort * 찾은  Input DataPort
	*/
	virtual DataPort *getInputDataPort(const std::string  &portName) { return NULL; };

	/**
		DataPort에 속한 DataPortLink 객체를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName DataPortLink가 속해 있는DataPort의 이름

		@return DataPortLink * 찾은  DataPortLink 객체
	*/
	virtual DataPortLink *getDataPortLink(const std::string  &portName) { return NULL; };

	/**
		Data Port에 속한 DataPortLink를 추가하는 함수.
		컴포짓 컴포넌트인 경우 아무일도 하지 않음.

		@param portName DataPortLink가 추가되어야 할 Data Port의 이름
		@param link     추가할 DataPortLink 객체
	*/
	virtual void addDataPortLink(const std::string  &portName, DataPortLink *link) {};

	/**
		Data Port에 속한 DataPortLink를 삭제하는  함수.
		컴포짓 컴포넌트인 경우 아무일도 하지 않음.

		@param portName DataPortLink를 삭제해야할 Data Port의 이름
	*/
	virtual void removeDataPortLink(const std::string  &portName) {};

	/**
		컴포넌트에 속한 Output EventPort 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName 찾고자 EventPort 이름

		@return DataPort * 찾은  Output EventPort
	*/
	virtual EventPort *getOutputEventPort(const std::string  &portName) { return NULL; };

	/**
		컴포넌트에 속한 Input EventPort를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName 찾고자 하는 EventPort 이름

		@return EventPort * 찾은  Input EventPort
	*/
	virtual EventPort *getInputEventPort(const std::string  &portName) { return NULL; };

	/**
		Event Port에 속한 EventPortLink 객체를 리턴하는 함수.
		컴포짓 컴포넌트인 경우 NULL을 리턴.

		@param portName EventPortLink 속해 있는 Event Port의 이름

		@return EventPortLink * 찾은  EventPortLink 객체
	*/
	virtual EventPortLink *getEventPortLink(const std::string  &portName) { return NULL; };

	/**
		Event Port에 속한 EventPortLink를 추가하는 함수.
		컴포짓 컴포넌트인 경우 아무일도 하지 않음.

		@param portName EventPortLink가 추가되어야 할 Event Port의 이름
		@param link     추가할 EventPortLink 객체
	*/
	virtual void addEventPortLink(const std::string  &portName, EventPortLink *link) {};

	/**
		Event Port에 속한EventPortLink를 삭제하는  함수.
		컴포짓 컴포넌트인 경우 아무일도 하지 않음.

		@param portName EventPortLink를 삭제해야할  Event Port의 이름
	*/
	virtual void removeEventPortLink(const std::string  &portName) {};

	/**
		이 컴포넌트의 Required Service Port와  다른 컴포넌트의 Proviced Service Port간을 연결하는 함수.
		컴포짓 컴포넌트인 경우에만 수행됨.

		@param target 이 컴포넌트와 연결될 다른 컴포넌트
		@param info   포트 연결 정보

		@return bool 포트 연결 성공 여부
	*/
	virtual bool connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info) = 0;

	/**
		이 컴포넌트의 Output DataPort와  다른 컴포넌트의 Input DataPort간을 연결하는 함수.
		컴포짓 컴포넌트인 경우에만 수행됨.

		@param target 이 컴포넌트와 연결될 다른 컴포넌트
		@param info   포트 연결 정보

		@return bool 포트 연결 성공 여부
	*/
	virtual bool connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info) = 0;

	/**
		이 컴포넌트의 Output EventPort와  다른 컴포넌트의 Input EventPort간을 연결하는 함수.
		컴포짓 컴포넌트인 경우에만 수행됨.

		@param target 이 컴포넌트와 연결될 다른 컴포넌트
		@param info   포트 연결 정보

		@return bool 포트 연결 성공 여부
	*/
	virtual bool connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info) = 0;

	/**
		이 컴포넌트와 다른 컴포넌트 간의 ServicePort 연결을 해제하는 함수
		컴포짓 컴포넌트인 경우에만 수행됨.

		@param info  해제할 포트 연결 정보

		@return bool 포트 해제  성공 여부
	*/
	virtual bool disconnectServicePort(PortConnectionInfo &info) = 0;

	/**
		이 컴포넌트와 다른 컴포넌트 간의 DataPort 연결을 해제하는 함수
		컴포짓 컴포넌트인 경우에만 수행됨.

		@param info  해제할 포트 연결 정보

		@return bool 포트 해제  성공 여부
	*/
	virtual bool disconnectDataPort(PortConnectionInfo &info) = 0;

	/**
		이 컴포넌트와 다른 컴포넌트 간의 EventPort 연결을 해제하는 함수
		컴포짓 컴포넌트인 경우에만 수행됨.

		@param info  해제할 포트 연결 정보

		@return bool 포트 해제  성공 여부
	*/
	virtual bool disconnectEventPort(PortConnectionInfo &info) = 0;


protected :

	/**	컴포넌트를 로드하는 함수로 상태 체크 없이 바로 수행 */
	virtual bool doLoad() = 0;

	/**	컴포넌트를 언로드하는 함수로 상태 체크 없이 바로 수행 */
	virtual bool doUnload() = 0;

	/**	컴포넌트 간의 포트를 연결하는 함수로 상태 체크 없이 바로 수행 */
	virtual bool doConnectPort() = 0;

	/**	컴포넌트 간의 포트 연결을 해제하는 함수로 상태 체크 없이 바로 수행 */
	virtual bool doDisconnectPort() = 0;

	/**	컴포넌트의 실행을 시작시키는 함수로 상태 체크 없이 바로 수행 */
	virtual bool doActivate() = 0;

	/**	컴포넌트의 실행을 중지시키는 함수로 상태 체크 없이 바로 수행 */
	virtual bool doPassivate() = 0;
};

//typedef std::list<ComponentAdapter *> ComponentAdapterList;

typedef std::list<ComponentAdapterPtr> ComponentAdapterList;

#endif
