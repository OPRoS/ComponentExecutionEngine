/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentEngine.h
 *
 */

#ifndef _COMPONENT_ENGINE_H_
#define _COMPONENT_ENGINE_H_

#include <string>
#include <map>

#include "system/util/Properties.h"
#include "system/os/OSNotify.h"

#include "Manager.h"

/**
	컴포넌트 실행 엔진 내의 각종 관리자를 관리하는 클래스. 
 */
class ComponentEngine : public Manager
{
	typedef std::map<std::string , Manager *> ManagerMap;

	/** 컴포넌트 실행 엔진내의 관리자(Manager) 객체를 보관하기 위한 맵 */
	ManagerMap m_managers;

	/** 
		시스템 프로퍼티(system properties)를 저장하기 위한 객체.
		config/system.xml내에 아래와 같이 시스템 프로퍼티를 추가할 수 있음.
		<system_props>
			<prop name="app.default">hello</prop>
			<prop name="sys.console">on</prop>
		</system_props>
	*/
	Properties m_props; 

	/** 컴포넌트 실행 엔진이 종료할때 이를 알려주기 위한 객체 */
	OSNotify m_finished; 

public:
	ComponentEngine();
	ComponentEngine(const ComponentEngine&);
	ComponentEngine& operator=(const ComponentEngine&);
	virtual ~ComponentEngine();

	/** 
		컴포넌트 실행엔진 초기화 함수.
		컴포넌트 실행엔진 내의 모든 관리자 객체를 생성하고 초기화 함.

		@return bool 성공 여부
	*/
	virtual bool onInit();

	/** 
		컴포넌트 실행엔진 시작 함수.
		컴포넌트 실행엔진 내의 모든 관리자 객체를 시작시킴.

		@return bool 성공 여부
	*/
	virtual bool onStart();

	/** 
		컴포넌트 실행엔진 종료 함수.
		컴포넌트 실행엔진 내의 모든 관리자 객체를 종료시킴.

		@return bool 성공 여부
	*/
	virtual bool onStop();

	/** 
		컴포넌트 실행엔진 객체를 메모리에서 삭제하기 전에 호출되는 함수.
		컴포넌트 실행엔진 내의 모든 관리자 객체의 onDestroy() 함수를 호출함.

		@return bool 성공 여부
	*/
	virtual bool onDestroy();

	/** 
		config/system.xml 의 정보를 담고 있는 시스템 Config 객체를 반환하는 함수.

		@return Config * 시스템 Config 객체에 대한 포인터
	*/
	Config *getSystemConfig() { return getConfig()->getChild("system_config"); };

	/** 
		관리자(Manager) 객체를 검색하는 함수.

		@return Manager * 검색한 관리자 객체에 대한 포인터
	*/
	Manager *getManager(const std::string  &mgrName);

	/** 
		컴포넌트 실행엔진이 종료될 때까지 기다리는 함수.
	*/
	void waitFinished();
};

#endif  
