/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentPortConnector.h
 *
 */

#ifndef _COMPONENT_PORT_CONNECTOR_H_
#define _COMPONENT_PORT_CONNECTOR_H_

#include <string>
#include "ComponentAdapter.h"

#include "system/profile/PortConnectionInfo.h"

/** 
	컴포넌트 간의 포트를 연결하거나 해제하는 클래스 
*/
class ComponentPortConnector
{
public:
	ComponentPortConnector();
	virtual ~ComponentPortConnector();

	/** 
		PortConnectionInfo 리스트 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 컴포넌트들 간의 포트를 연결하는 함수.
		ComponentManager::connectPortList() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param infoList 포트 연결 정보 리스트

		@return bool 포트 연결 성공 여부

		@see ComponentManager::connectPortList()
	*/
	virtual bool connectPortList(ComponentAdapterList &caList, PortConnectionInfoList &infoList);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 포트를 연결하는 함수.
		ComponentManager::connectPort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentManager::connectPort()
	*/
	virtual bool connectPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 서비스 포트를 연결하는 함수.
		ComponentManager::connectServicePort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentManager::connectServicePort()
	*/
	virtual bool connectServicePort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 데이터 포트를 연결하는 함수.
		ComponentManager::connectDataPort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentManager::connectDataPort()
	*/
	virtual bool connectDataPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 이벤트 포트를 연결하는 함수.
		ComponentManager::connectEventPort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 성공 여부

		@see ComponentManager::connectEventPort()
	*/
	virtual bool connectEventPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo 리스트 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 컴포넌트들 간의 포트 연결을 해제하는 함수.
		ComponentManager::disconnectPortList() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param infoList 포트 연결 정보 리스트

		@return bool 포트 연결 해제 성공 여부

		@see ComponentManager::disconnectPortList()
	*/
	virtual bool disconnectPortList(ComponentAdapterList &caList, PortConnectionInfoList &list);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 포트 연결을 해제하는 함수.
		ComponentManager::disconnectPort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 해제 성공 여부

		@see ComponentManager::disconnectPort()
	*/
	virtual bool disconnectPort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 서비스 포트의 연결을 해제하는 함수.
		ComponentManager::disconnectServicePort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 해제 성공 여부

		@see ComponentManager::disconnectServicePort()
	*/
	virtual bool disconnectServicePort(ComponentAdapterList &caList, PortConnectionInfo &info);

	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 데이터 포트의 연결을 해제하는 함수.
		ComponentManager::disconnectDataPort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 해제 성공 여부

		@see ComponentManager::disconnectDataPort()
	*/
	virtual bool disconnectDataPort(ComponentAdapterList &caList, PortConnectionInfo &info);
	
	/** 
		PortConnectionInfo 정보를 이용하여 
		ComponentAdapterList 내에 존재하는 두 컴포넌트 간의 이벤트 포트의 연결을 해제하는 함수.
		ComponentManager::disconnectEventPort() 에서 호출됨.

		@param caList 포트 연결의 대상이 되는 컴포넌트의 집합
		@param info 포트 연결 정보

		@return bool 포트 연결 해제 성공 여부

		@see ComponentManager::disconnectEventPort()
	*/
	virtual bool disconnectEventPort(ComponentAdapterList &caList, PortConnectionInfo &info);

protected :

	/** 
		ComponentAdapterList 내의 특정 컴포넌트를 검색하는 함수.

		@param caList 컴포넌트의 집합
		@param compName 검색하고자 하는 컴포넌트 아이디(이름)

		@return ComponentAdapter * 포트 연결 성공 여부

		@see ComponentManager::connectPortList()
	*/
	ComponentAdapterPtr findComponentAdapter(ComponentAdapterList &caList, const std::string &compName);

};

#endif 
