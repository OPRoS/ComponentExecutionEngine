/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentManager.h
 *
 */

#ifndef _COMPONENT_MANAGER_H_
#define _COMPONENT_MANAGER_H_

#include <string>

#include "system/Manager.h"
#include "system/profile/ComponentProfile.h"
#include "system/profile/CompositeComponentProfile.h"
#include "system/os/OSMutex.h"

#include "ComponentAdapter.h"
#include "ComponentPortConnector.h"

#include "ComponentLocator.h"
#include "ComponentRequester.h"
#include "NodeLocator.h"

#ifdef	MOD_VERIFY_COMPONENT
#include "ComponentVerifier.h"
#endif

#include "ExecutorManager.h"

class Component;

/**
	 로딩된 모든 컴포넌트를 관리하는 클래스. 
 */
class ComponentManager : public Manager
{
	/** 로드된 컴포넌트를 저장하기 위한 std::vector */
	ComponentAdapterList m_caList;

	/** ComponentAdapterList m_caList 접근을 동기화하기 위한 뮤텍스 객체 */
	OSMutex m_caListLock;

	/** 컴포넌트가 존재하는 노드의 주소를 관리하는 객체 */
	ComponentLocator m_componentLocator;

	/** 컴포넌트 간의 포트를 연결하기 위한 객체 */
	ComponentPortConnector m_portConnector;

	/** 원격 노드에 존재하는 컴포넌트을 접근하거나
		원격 노드의 컴포넌트 접근 요청을 처리하기 위한 객체 */
	ComponentRequester m_requester;
	std::string m_cnnHandler;
	std::string m_evtProcessor;

	/** 노드의 실제 주소를 저장하고 있는 객체 */
	NodeLocator m_nodeLocator;

	/** 컴포넌트가 할당된 Executor를 관리하는 객체 */
	ExecutorManager *m_pExecMgr;

#ifdef	MOD_VERIFY_COMPONENT
	ComponentVerifier cvm;
#endif

protected :
	/*
	 *   m_caList로부터  adapter를 제거
	 */
	void removeComponentAdapter(const std::string &name);

	/*
	 *   m_caList에 adapter 추가
	 */
	void addComponentAdapter(ComponentAdapter *ca); 

public:
	ComponentManager();
	virtual ~ComponentManager();

	void lock() { m_caListLock.lock(); }
	void unlock() { m_caListLock.unlock(); }

    /** 컴포넌트 관리자 초기화 함수. */
	virtual bool onInit();

    /** 컴포넌트 관리자 시작 함수. */
	virtual bool onStart();

    /** 컴포넌트 관리자 중지 함수. */
	virtual bool onStop();

    /** 컴포넌트 관리자 삭제전에 호출되는 함수. */
	virtual bool onDestroy();

	/** NodeLocator 객체를 반환하는 함수 */
	NodeLocator *getNodeLocator() { return &m_nodeLocator; };

	/** ComponentLocator 객체를 반환하는 함수 */
	ComponentLocator *getComponentLocator() { return &m_componentLocator; };

	/** ComponentRequester 객체를 반환하는 함수 */
	ComponentRequester *getComponentRequester() { return &m_requester; };

	/** ComponentPortConnector 객체를 반환하는 함수 */
	ComponentPortConnector *getComponentPortConnector() { return &m_portConnector; };

	/** ExecutorManager 객체를 반환하는 함수 */
	ExecutorManager *getExecutorManager() { return m_pExecMgr; };

	/** 컴포넌트 리스트 ComponentAdapterList를 반환하는 함수 */
	ComponentAdapterList &getComponentAdapterList() { return m_caList; };

	/** 
		특정 컴포넌트를 검색하여 반환하는 함수.

		@param name 검색할 컴포넌트 아이디(이름)

		@return ComponentAdapter * 검색한 컴포넌트. 존재하지 않으면 NULL 반환.
	*/
	ComponentAdapterPtr getComponentAdapter(const std::string  &name);


	/** 
		특정 컴포넌트를 검색하여 반환하는 함수.

		@param parent 검색할 컴포넌트의 parent id
		@param name 검색할 컴포넌트 아이디(이름)

		@return ComponentAdapter * 검색한 컴포넌트. 존재하지 않으면 NULL 반환.
	*/
	ComponentAdapterPtr getComponentAdapter(const string &parent, const string &name);

	/** 
		컴포넌트를 로딩하는 함수. 동기화를 위해 뮤텍의 lock()을 호출.
		컴포넌트가 정상정으로 로딩되면 ComponentAdapterList에 저장

		@param nodeId 로딩할 컴포넌트가 존재하는 노드의 아이디
		&param parentid 로딩할 컴포넌트의 부모 컴포넌트(어플리케이션) ID
		@param name 로딩할 컴포넌트 아이디(이름)
		@param path 컴포넌트 프로파일 및 컴포넌트 DLL이 존재하는 디렉토리
		@param ref  컴포넌트 프로파일의 파일 이름

		@return ComponentAdapter * 로딩한 컴포넌트 객체 반환. 로딩에 실패하면 NULL 반환.
	*/
	ComponentAdapter *loadComponent(const std::string &nodeId, const string &parentid, const std::string &name, \
		const std::string &path, const std::string &ref);

	/** 
		로봇 응용의 최상위(root) 컴포넌트를 로딩하는 함수.
		- 해당 컴포넌트가 현재 노드에 이미 존재하는 경우 이미 존재하는 ComponentAdapter 객체 반환.		
		- 해당 컴포넌트가 원격 노드에 존재하는 경우 ComponentAdapterRemoteProxy 객체 생성. 
		- 컴포짓 컴포넌트인 경우 ComponentAdapterComposite 객체 생성.
		- 단일 컴포넌트인 경우ComponentAdapterAtomic 객체 생성.

		@param nodeId 로딩할 컴포넌트가 존재하는 노드의 아이디
		@param name 로딩할 컴포넌트 아이디(이름)
		@param path 컴포넌트 프로파일 및 컴포넌트 DLL이 존재하는 디렉토리
		@param ref  컴포넌트 프로파일의 파일 이름

		@return ComponentAdapter * 로딩한 컴포넌트 객체 반환. 로딩에 실패하면 NULL 반환.

		@see ComponentManager::loadComponent()
	*/
	ComponentAdapter *loadRootComponent(const std::string &nodeId, const std::string &name, \
		const std::string &path, const std::string &ref);

	/** 
		컴포짓 컴포넌트가 자신의 하위 컴포넌트를 로딩할 때 호출하는 함수.
		- 해당 컴포넌트가 현재 노드에 이미 존재하는 경우 ComponentAdapterLocalProxy 객체 생성.
		  (컴포짓 컴포넌트는 자신의 하위 컴포넌트를 내부에 저장하고 있으므로 
		   이미 존재하는 컴포넌트를 저장하기 위해 ComponentAdapterLocalProxy를 생성하여 저장함)
		- 해당 컴포넌트가 원격 노드에 존재하는 경우 ComponentAdapterRemoteProxy 객체 생성. 
		- 컴포짓 컴포넌트인 경우 ComponentAdapterComposite 객체 생성.
		- 단일 컴포넌트인 경우ComponentAdapterAtomic 객체 생성.

		@param nodeId 로딩할 컴포넌트가 존재하는 노드의 아이디
		@param name 로딩할 컴포넌트 아이디(이름)
		@param path 컴포넌트 프로파일 및 컴포넌트 DLL이 존재하는 디렉토리
		@param ref  컴포넌트 프로파일의 파일 이름

		@return ComponentAdapter * 로딩한 컴포넌트 객체 반환. 로딩에 실패하면 NULL 반환.

		@see ComponentManager::loadComponent()
	*/
	ComponentAdapter *loadSubComponent(const std::string &nodeId, const std::string &parent, const std::string &name, \
		const std::string &path, const std::string &ref);

	/** 
		현재 존재하지 않는 원격 노드가 아닌 현재 로컬 노드에 존재하는 컴포넌트를 로딩할 때 호출하는 함수.
		해당 컴포넌트가 원격 노드에 존재하는 경우 ComponentAdapterRemoteProxy 객체 생성. 
		- 컴포짓 컴포넌트인 경우 ComponentAdapterComposite 객체 생성.
		- 단일 컴포넌트인 경우ComponentAdapterAtomic 객체 생성.

		- 절차
		  1) 컴포넌트 프로파일 로딩
		  2) ComponentAdapter::load() 호출


		@param nodeId 로딩할 컴포넌트가 존재하는 노드의 아이디
		@param parent 로딩할 컴포넌트의 parent id
		@param name 로딩할 컴포넌트 아이디(이름)
		@param path 컴포넌트 프로파일 및 컴포넌트 DLL이 존재하는 디렉토리
		@param ref  컴포넌트 프로파일의 파일 이름

		@return ComponentAdapter * 로딩한 컴포넌트 객체 반환. 로딩에 실패하면 NULL 반환.

		@see ComponentManager::loadComponent()
	*/
	ComponentAdapter *loadLocalComponent(const std::string &nodeId, const std::string &parent, const std::string &name, \
		const std::string &path, const std::string &ref);

	ComponentAdapter *loadLocalComponent(const string &parent, CompositeComponentProfile *profile);
	ComponentAdapter *loadLocalComponent(const string &parent, ComponentProfile *profile);

	/** 
		컴포넌트를 언로딩하는 함수.

		@param name 언로딩할 컴포넌트 아이디(이름)

		@return bool 언로딩 성공 여부.
	*/
	bool unloadComponent(const std::string &name);

	/** 
		컴포넌트를 언로딩하는 함수.

		@param name 언로딩할 컴포넌트 Parent 아이디(이름)

		@return bool 언로딩 성공 여부.
	*/
	bool unloadComponent(const std::string &parent, const std::string &name);

	/** 
		로딩된 모든 컴포넌트를 언로딩하는 함수.

		@return bool 언로딩 성공 여부.
	*/
	bool unloadComponentAll();


	/** 
		컴포넌트 간의 포트를 연결하는 함수.
		ComponentAdapter::doConnectPort() 함수 호출.

		@param ca 컴포넌트 어댑터

		@return bool 포트 연결 성공 여부.

		@see ComponentAdapter::doConnectPort()
	*/
	bool connectPort(ComponentAdapter *ca);

	/** 
		컴포넌트 간의 포트를 연결하는 함수.
		ComponentAdapter::doConnectPort() 함수 호출.

		@param name 컴포넌트 아이디(이름)

		@return bool 포트 연결 성공 여부.

		@see ComponentAdapter::doConnectPort()
	*/
	bool connectPort(const std::string  &name);



	/** 
		컴포넌트 간의 포트 연결을 해제하는 함수.
		ComponentAdapter::doDisconnectPort() 함수 호출.

		@param ca 컴포넌트 어댑터

		@return bool 포트 연결 해제 성공 여부.

		@see ComponentAdapter::doDisconnectPort()
	*/
	bool disconnectPort(ComponentAdapter *ca);

	/** 
		컴포넌트 간의 포트 연결을 해제하는 함수.
		ComponentAdapter::doDisconnectPort() 함수 호출.

		@param name 컴포넌트 아이디(이름)

		@return bool 포트 연결 해제 성공 여부.

		@see ComponentAdapter::doDisconnectPort()
	*/
	bool disconnectPort(const std::string  &name);

	/** 
		PortConnectionInfo 리스트 내의 여러개의 포트를 연결하는 함수.
		ComponentPortConnector::connectPortList() 함수 호출.

		@param infoList 포트 연결 정보 리스트

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::connectPortList()
	*/
	bool connectPortList(PortConnectionInfoList &infoList);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 포트를 연결하는 함수.
		ComponentPortConnector::connectPort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::connectPort()
	*/
	bool connectPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 서비스 포트를 연결하는 함수.
		ComponentPortConnector::connectServicePort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::connectServicePort()
	*/
	bool connectServicePort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 데이터 포트를 연결하는 함수.
		ComponentPortConnector::connectDataPort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::connectDataPort()
	*/
	bool connectDataPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 이벤트 포트를 연결하는 함수.
		ComponentPortConnector::connectEventPort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::connectEventPort()
	*/
	bool connectEventPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo 리스트 내의 여러개의 포트 연결을 해제하는 함수.
		ComponentPortConnector::disconnectPortList() 함수 호출.

		@param infoList 포트 연결 정보 리스트

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::disconnectPortList()
	*/
	bool disconnectPortList(PortConnectionInfoList &list);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 포트 연결을 해제하는 함수.
		ComponentPortConnector::disconnectPort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::disconnectPort()
	*/
	bool disconnectPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 서비스 포트의 연결을 해제하는 함수.
		ComponentPortConnector::disconnectServicePort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::disconnectServicePort()
	*/
	bool disconnectServicePort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 데이터 포트의 연결을 해제하는 함수.
		ComponentPortConnector::disconnectDataPort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::disconnectDataPort()
	*/
	bool disconnectDataPort(PortConnectionInfo &info);

	/** 
		PortConnectionInfo의 정보를 보고 두 컴포넌트 간의 이벤트 포트의 연결을 해제하는 함수.
		ComponentPortConnector::disconnectEventPort() 함수 호출.

		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentPortConnector::disconnectEventPort()
	*/
	bool disconnectEventPort(PortConnectionInfo &info);

	/** 
		컴포넌트의 실행을 시작하는 함수.

		@param name 컴포넌트 아이디(이름)

		@return bool 성공 여부.
	*/
	bool activate(const std::string  &name);


	/** 
		컴포넌트의 실행을 시작하는 함수.

		@param ca 컴포넌트 어댑터

		@return bool 성공 여부.
	*/
	bool activate(ComponentAdapter *ca);


/** 
		컴포넌트의 실행을 중지하는 함수.

		@param ca 컴포넌트 어댑터

		@return bool 성공 여부.
	*/
	bool passivate(ComponentAdapter *ca);

	/** 
		컴포넌트의 실행을 중지하는 함수.

		@param name 컴포넌트 아이디(이름)

		@return bool 성공 여부.
	*/
	bool passivate(const std::string  &name);
};

#endif
