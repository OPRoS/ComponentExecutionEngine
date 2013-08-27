/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 31
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentPackage.cpp
 *
 */

#include <string>




#include "system/Registry.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/cm/CoordinationUnit.h"
#include "ComponentPackage.h"
using namespace std;

ComponentPackage::ComponentPackage()
{
	m_type = PKG_ATOMIC;
	m_parent = DEFAUT_PARENT_NAME;
	m_ca= NULL;
}


ComponentPackage::~ComponentPackage()
{
	trace_enter();

	beforeComponentList.clear();
	afterComponentList.clear();
}

void ComponentPackage::setParentId(const std::string &parent)
{
	m_parent = parent;
}

std::string &ComponentPackage::getParentId()
{
	return m_parent;
}


ComponentAdapter *ComponentPackage::getComponentAdapter()
{
	return m_ca;
}

bool ComponentPackage::doLoad()
{
	trace_enter();

	ComponentManager *cmgr =
		portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL : pkg.name=" << getName());
		return false;
	}

	// 컴포넌트 관리자에게 컴포넌트 로딩을 요청함
	// 해당 컴포넌트는 atomic이거나 composite 컴포넌트 임
	m_ca = cmgr->loadComponent(getNodeId(), getParentId(), getName(), getPath(), getReference());
	if (m_ca == NULL) {
		// 컴포넌트 로딩 실패
		log_error("Component add FAIL : pkg.name=" << getName());
		return false;
	}
	else {
		// 컴포넌트 로딩 성공
		// 컴포넌트 아이디 세팅
		setName(m_ca->getId());	

		// 컴포넌트 어댑터의 Depend Setting

	/*	CoordinationUnit *t_coord = m_ca->getCoordinationUnit();

		std::list<std::string>::iterator bc_it; 

		for (bc_it = beforeComponentList.begin(); bc_it != beforeComponentList.end(); bc_it++)
		{			
			t_coord.addTrigger((*bc_it));
		}
		
		for (bc_it = afterComponentList.begin(); bc_it != afterComponentList.end(); bc_it++)
		{			
			t_coord.addFollower((*bc_it));
		} */

		trace("Component add OK : pkg.name=" << getName());
		return true;
	}
}

bool ComponentPackage::doUnload()
{
	trace_enter_msg("name=" + getName());

	ComponentManager *cmgr =
		portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL : pkg.name=" << getName());
		return false;
	}

	// 컴포넌트 관리자에게 컴포넌트 언로딩을 요청함
	if (m_ca != NULL && cmgr->unloadComponent(m_ca->getId())) {
		trace("Component remove OK : pkg.name=" << getName());

		//ComponentAdapterList &t_list = cmgr->getComponentAdapterList();
		// 컴포넌트 어댑터 중에서 m_ca 와 관계 있는 것들로부터 trigger를 제거
		// 이부분은 필요 컴포넌트가 없는 상황에서 trigger가 on 되도록 할것인가 아니면 
		// 계속 off가 되도록 할것인가의 문제임
		// 일단 컴포넌트가 사라지면 관계된 컴포넌트들의 trigger는 -1로 하고 무조건 true인것으로 가정


		return true;
	}
	else {
		log_error("Component unload FAIL : pkg.name=" << getName());
		return false;
	}
}

bool ComponentPackage::doConnectPort()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	// 컴포넌트 관리자에게 컴포넌트의 포트 연결을 요청함
	// composite일 경우에만 실제 포트 연결이 수행됨
	// atomic일 경우에는 포트 연결 필요 없음(컴포넌트가 하나이기 때문에)
	return cmgr->connectPort(m_ca);
}

bool ComponentPackage::doDisconnectPort()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->disconnectPort(m_ca);
}

bool ComponentPackage::doActivate()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->activate(m_ca);
}

bool ComponentPackage::doPassivate()
{
	trace_enter_msg("name = " + getName());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cmgr == NULL) {
		log_error("ComponentManager is NULL");
		return false;
	}

	return cmgr->passivate(m_ca);
}
