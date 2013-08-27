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

#include "ComponentAdapterAtomic.h"

#include "ServicePort.h"
#include "DataPort.h"
#include "EventPort.h"
#include "DataPortLink.h"
#include "ReturnType.h"

#include "executor/Executor.h"

#include "ComponentManager.h"
#include "ComponentLoader.h"
#include "ComponentLoaderDll.h"

#include "system/config/ConfigLoaderFactory.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "DataPortConnector.h"
#include "DataPortConnectorImpl.h"

#include "EventPortConnector.h"
#include "EventPortConnectorImpl.h"
#include "ServicePortConnectorImpl.h"

#include "ArchiveManager.h"
#include "ArchiveFactory.h"
#include "OutputDataPortInterface.h"
#include "OutputEventPortInterface.h"

#include <MethodInvokeException.h>
#include <ArgumentNumberException.h>

using namespace std;

ComponentAdapterAtomic::ComponentAdapterAtomic()
{
	m_component = NULL;
	m_profile = NULL;
	m_loader = NULL;
	m_executorId = 0;
}

ComponentAdapterAtomic::~ComponentAdapterAtomic()
{
	if (m_loader != NULL) {
		m_loader->close();
		delete m_loader;
	}

	if (m_profile != NULL)
	{
		delete m_profile;
	}
}


std::string ComponentAdapterAtomic::stateToStr()
{
	switch (getComponent()->getStatus()) {
	case OPROS_CS_CREATED 	: return "CREATED";
	case OPROS_CS_READY 	: return "READY";
	case OPROS_CS_ACTIVE 	: return "ACTIVE";
	case OPROS_CS_INACTIVE 	: return "INACTIVE";
	case OPROS_CS_ERROR 	: return "ERROR";
	case OPROS_CS_DESTROYED : return "DESTROYED";
	default : return "Invalid";
	}
}

bool ComponentAdapterAtomic::doLoad()
{
	// 컴포넌트 로더 생성
	if (m_profile->execEnv.libraryType == "dll" || m_profile->execEnv.libraryType == "so") {
		m_loader = new ComponentLoaderDll();
	}
	if (m_loader == NULL) {
		m_loader = new ComponentLoaderDll();
	}

	// 컴포넌트 로더 open
	if (!m_loader->open(m_profile)) {
		delete m_loader;
		m_loader = NULL;

		log_error("ComponentLoader open FAIL : name=" << getId());
		return false;
	}

	// 컴포넌트 로더를 통해 컴포넌트 인스턴스 생성
	m_component = m_loader->loadComponent();
	if (m_component == NULL) {
		// 컴포넌트 생성이 실패하면 컴포넌트 로더 삭제
		delete m_loader;
		m_loader = NULL;

		log_error("Component loading FAIL : name=" << getId());
		return false;
	}

	// 컴포넌트에 id 세팅
	string id = getId();
	m_component->setId(id);

	// 컴포넌트 load시 컴포넌트의 initialize() 함수를 호출함
	// 포트가 연결되기 전에 initialize() 함수가 호출되므로
	// initialize()함수에서 포트를 사용하면 안됨
	// initialize를 포트 연결 후에 하기 위해 activate로 이동함
	if (!doInitialize()) {
		log_error("ComponentAdapter init FAIL : name=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doUnload()
{
	trace_enter_msg("name=" << getId());

	// 컴포넌트가 메모리에서 삭제되기 바로 전에  doDestroy() 함수를 호출함
	// doDestroy() 가 실패하면 메모리에서 삭제하지 않음 !!!
	if (!doDestroy()) {
		return false;
	}

	if (m_loader != NULL) {
		// 컴포넌트 로더을 통해 컴포넌트를 메모리에서 삭제함
		if (m_loader->unloadComponent(getComponent())) {

			trace("remove OK : name=" << getId());
			m_component = NULL;
			return true;
		}
		else {
			log_error("remove FAIL : name=" << getId());
			return false;
		}
	}

	return true;
}

bool ComponentAdapterAtomic::doActivate()
{
	trace_enter_msg("name=" << getId());

	// 프로파일의 실행 시멘틱스 정보 중 lifecycleType이 start인 경우에만
	// 컴포넌트의 start() 함수를 호출함
	if (m_profile->execSemantics.lifecycleType == "start" && !doStart()) {
		log_error("ComponentAdapter start FAIL : name=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doPassivate()
{
	trace_enter_msg("name=" << getId());

	// 컴포넌트의 실행을 중지 함
	if (!doStop()) {
		return false;
	}

	return true;
}

// 컴포넌트 초기화 함수
bool ComponentAdapterAtomic::doInitialize()
{
	trace_enter_msg("name=" + getId());

	log_info("doInit() : name=" << getId());

	// 컴포넌트 프로파일이 NULL이면 초기화 하지 않음
	if (m_profile == NULL) {
		log_error("ComponentProfile is NULL...");
		return false;
	}

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component loading FAIL : ComponentAdapterAtomic.name=" << getId());
		return false;
	}

	// 컴포넌트 프로파일에 존재하는 프로퍼티를 컴포넌트에 세팅함
	vector<string> propList;
	m_profile->props.getNames(propList);
	for (size_t i = 0; i < propList.size(); ++i) {
		string name = propList[i];
		string value = m_profile->props.getProperty(propList[i]);
		comp->setProperty(name, value);
	}

	// component init
	trace("Try to init...");
	try {
		if (comp->initialize() != OPROS_SUCCESS) {
			log_error("Component init FAIL : comp.name=" << getId());
			//return true;
			return false;
		}

		// executor 생성
		trace("Executor allocating : executor.type=" << m_profile->execSemantics.type);
		ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cm != NULL) {
			cm->getExecutorManager()->addComponent(this);
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		return false;
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
		return false;
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
		return false;
	}

	return true;
}

bool ComponentAdapterAtomic::doStart()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->startComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}
	return false;
}

bool ComponentAdapterAtomic::doStop()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->stopComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException ) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}
	return false;}

bool ComponentAdapterAtomic::doReset()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	if (cm != NULL) {
		try {
			return cm->getExecutorManager()->resetComponent(this);
		}
		catch (MethodInvokeException &mie) {
			log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
		}
		catch (ArgumentNumberException ) {
			log_error("ArgumentNumberException : id=" << getId());
		}
		catch (...) {
			log_error("Unknown Exception : id=" << getId());
		}
	}
	return false;
}

bool ComponentAdapterAtomic::doStopOnError()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->stopOnError() != OPROS_SUCCESS) {
			log_error("Component error FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException ) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doRecover()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->recover() != OPROS_SUCCESS) {
			log_error("Component recover FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException ) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doDestroy()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->getStatus() != OPROS_CS_CREATED)
		{
			if (comp->destroy() != OPROS_SUCCESS) {
				log_info("Component destroy FAIL : name=" << getId());
				return false;
			}
		}

		ComponentManager *cm = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cm != NULL) {
			cm->getExecutorManager()->removeComponent(this);
		}
		return true;
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doExecute()
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->onExecute() != OPROS_SUCCESS) {
			log_error("Component execute FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

bool ComponentAdapterAtomic::doEvent(Event *evt)
{
	trace_enter_msg("ComponentAdapterAtomic.name=" + getId());

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is null : name=" << getId());
		return false;
	}

	try {
		if (comp->onEvent(evt) != OPROS_SUCCESS) {
			log_error("Component event FAIL : name=" << getId());
			return false;
		}
		else {
			return true;
		}
	}
	catch (MethodInvokeException &mie) {
		log_error("MethodInvokeException : id=" << getId() << ";msg=" << mie.getMessage());
	}
	catch (ArgumentNumberException) {
		log_error("ArgumentNumberException : id=" << getId());
	}
	catch (...) {
		log_error("Unknown Exception : id=" << getId());
	}

	return false;
}

//-----------------------------
// 데이터 포트 링크 리턴
// @param portName 데이터 포트 이름
// @ret   데이터 포트에 할당된 데이터 포트 링크
DataPortLink *ComponentAdapterAtomic::getDataPortLink(const string &portName)
{
	// 데이터 포트 링크를 찾음
	DataPortLinkMap::iterator pos = m_dataPortMap.find(portName);
	if (pos != m_dataPortMap.end()) {
		return pos->second;
	}

	// 데이터 포트 링크가 없으면 세로 생성한 후 리턴
	DataPortLink *link = new DataPortLink();
	addDataPortLink(portName, link);

	return link;
}

void ComponentAdapterAtomic::addDataPortLink(const string &portName, DataPortLink *link)
{
	m_dataPortMap.insert(make_pair(portName, link));
}

void ComponentAdapterAtomic::removeDataPortLink(const string &portName)
{
	trace_enter();

	// portName에 해당하는 데이터 포트 링크를 찾는다.
	DataPortLinkMap::iterator pos = m_dataPortMap.find(portName);
	if (pos != m_dataPortMap.end()) {
		// 데이터 포트 링크를 메모리에서 삭제한다.
		delete pos->second;

		// 데이터 포트 링크를 리스트에서 삭제한다.
		m_dataPortMap.erase(pos);
	}
}

EventPortLink *ComponentAdapterAtomic::getEventPortLink(const string &portName)
{
	EventPortLinkMap::iterator pos = m_eventPortMap.find(portName);
	if (pos != m_eventPortMap.end()) {
		// 이미  존재하면 그것을 리턴함
		return pos->second;
	}

	// 존재하지 않으면 하나 생성하여 리턴
	EventPortLink *link = new EventPortLink();
	addEventPortLink(portName, link);

	return link;
}

void ComponentAdapterAtomic::addEventPortLink(const string &portName, EventPortLink *link)
{
	m_eventPortMap.insert(make_pair(portName, link));
}

void ComponentAdapterAtomic::removeEventPortLink(const string &portName)
{
	trace_enter();

	// portName에 해당하는 이벤트 포트 링크를 찾는다.
	EventPortLinkMap::iterator pos = m_eventPortMap.find(portName);
	if (pos != m_eventPortMap.end()) {
		// 이벤트 포트 링크를 메모리에서 삭제한다.
		delete pos->second;

		// 이벤트 포트 링크를 리스트에서 삭제한다.
		m_eventPortMap.erase(pos);
	}
}

RequiredServicePort *ComponentAdapterAtomic::getRequiredServicePort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("RequiredServicePort not exists :: cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_SERVICE)
	{
		log_error("The Port is not ServicePort : name=" << portName);
		return NULL;
	}

	RequiredServicePort *tport = portable_dynamic_cast(RequiredServicePort *,port);
	if (tport == NULL) {
		log_error("The Port is not RequiredServicePort : name=" << portName);
		return NULL;
	}

	return tport;
}

ProvidedServicePort *ComponentAdapterAtomic::getProvidedServicePort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("ProvidedMethodPort is not exists :: cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_SERVICE)
	{
		log_error("The Port is not ServicePort : name=" << portName);
		return NULL;
	}

	ProvidedServicePort *tport = portable_dynamic_cast(ProvidedServicePort *,port);
	if (tport == NULL) {
		log_error("The Port is not ProvidedServicePort : name=" << portName);
		return NULL;
	}

	return tport;
}

DataPort *ComponentAdapterAtomic::getOutputDataPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("DataPort is not exists :: cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_DATA)
	{
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}

	DataPort *tport = portable_dynamic_cast(DataPort *,port);
	if (tport == NULL) {
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}

	return tport;
}

DataPort *ComponentAdapterAtomic::getInputDataPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("DataPort is not exists : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	
	if (port->getType() != OPROS_DATA)
	{
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}


	DataPort *tport = portable_dynamic_cast(DataPort *,port);
	if (tport == NULL) {
		log_error("The Port is not DataPort : name=" << portName);
		return NULL;
	}

	return tport;
}

EventPort *ComponentAdapterAtomic::getOutputEventPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("EventPort is not exists : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_EVENT)
	{
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}


	EventPort *tport = portable_dynamic_cast(EventPort *,port);
	if (tport == NULL) {
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}

	

	return tport;
}

EventPort *ComponentAdapterAtomic::getInputEventPort(const string &portName)
{
	trace_enter_msg("portName = " + portName);

	Component *comp = getComponent();
	if (comp == NULL) {
		log_error("Component is NULL : name=" << getId());
		return NULL;
	}

	Port *port = comp->getPort(portName);
	if (port == NULL) {
		log_error("EventPort is not exists : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	if (port->getType() != OPROS_EVENT)
	{
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}

	EventPort *tport = portable_dynamic_cast(EventPort *,port);
	if (tport == NULL) {
		log_error("The Port is not EventPort : name=" << portName);
		return NULL;
	}

	return tport;
}

bool ComponentAdapterAtomic::connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName << ",targetPortName=" << info.targetPortName) ;

	RequiredServicePort *rmport = getRequiredServicePort(info.sourcePortName);
	if (rmport == NULL) {
		log_error("Not exists RequiredMethodPort : name=" << info.sourcePortName);
		return false;
	}

	if (target->getType() == CA_REMOTE_PROXY) {
		ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			return false;
		}

		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}

		ServicePortConnectorImpl *t_con = new ServicePortConnectorImpl(cmgr->getComponentRequester());
		t_con->setTargetComponentName(target->getId());
		t_con->setTargetPortName(info.targetPortName);


		ArchiveFactory *pfac = armgr->getDefaultArchiveFactory();
		rmport->setArchiveFactory(pfac);
		rmport->setConnector(t_con);
		
		// 이 단계에서 Connect의 종류에 따르 Archive를 바꾸어 줌으로써 binary, xml등이 가능하다.
		// info에 message type 이 binary 이면 binary archive를 xml이면 xml archive를 넣도록 한다.


	}
	else {
		// target
		ProvidedServicePort *pmport = target->getProvidedServicePort(info.targetPortName);
		if (pmport == NULL) {
			log_error("Not exists ProvidedMethodPort : name=" << info.targetPortName);
			return false;
		}

		trace("----------------------------------- setPeer before !!!!!");
		// connecting
		rmport->setPeer(pmport);
		trace("----------------------------------- setPeer After");
	}

	return true;
}

bool ComponentAdapterAtomic::connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName << ",targetPortName=" << info.targetPortName) ;

	DataPort *odport = getOutputDataPort(info.sourcePortName);
	if (odport == NULL) {
		log_error("Not exists DataPort : name=" << info.sourcePortName);
		return false;
	}

	if (target->getType() == CA_REMOTE_PROXY) {
//		log_error("Not Implemented yet !!!!!");


		ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			return false;
		}

		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}
/*
		ConnectionImpl *cnnImpl = new ConnectionImpl();
		cnnImpl->setComponentRequester(cmgr->getComponentRequester());
		DataPortConnectionImpl *mpImpl = new DataPortConnectionImpl(cnnImpl);

		mpImpl->setTargetComponentName(info.targetComponentName);
		mpImpl->setTargetPortName(info.targetPortName);

		rmport->setPeer(mpImpl);
		*/

		//
		//   아래와 같이 DataPortLink를 사용해야 합니다.
		//
		//


		DataPortConnectorImpl *d_con = new DataPortConnectorImpl(cmgr->getComponentRequester()); // DataPortConnector는 Factory에서 만들어져야 함.
		RemoteDataPort *t_port = portable_dynamic_cast(RemoteDataPort *, d_con);


		d_con->setTargetComponentName(info.targetComponentName);
		d_con->setTargetPortName(info.targetPortName);


		DataPortLink *link = getDataPortLink(info.sourcePortName);
		link->setSource(odport);
		link->addDestination(t_port);

		OutputDataPortInterface *g_odport = portable_dynamic_cast(OutputDataPortInterface *, odport);
		g_odport->setArchiveFactory(armgr->getDefaultArchiveFactory());
	}
	else {		
		// target
		DataPort *idport = target->getInputDataPort(info.targetPortName);
		if (idport == NULL) {
			log_error("Not exists DataPort : name=" << info.targetPortName);
			return false;
		}

		// connecting
		DataPortLink *link = getDataPortLink(info.sourcePortName);
		link->setSource(odport);
		link->addDestination(idport);		
	}

	return true;
}

bool ComponentAdapterAtomic::connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName << ",targetPortName=" << info.targetPortName) ;

	EventPort *oeport = getOutputEventPort(info.sourcePortName);
	if (oeport == NULL) {
		log_error("Not exists EventPort : name=" << info.sourcePortName);
		return false;
	}

	if (target->getType() == CA_REMOTE_PROXY) {		


		ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			return false;
		}

		ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
		if (armgr == NULL) {
			log_error("ArchiveManager is NULL");
			return false;
		}
/*
		ConnectionImpl *cnnImpl = new ConnectionImpl();
		cnnImpl->setComponentRequester(cmgr->getComponentRequester());
		EventPortConnectionImpl *mpImpl = new EventPortConnectionImpl(cnnImpl);

		mpImpl->setTargetComponentName(info.targetComponentName);
		mpImpl->setTargetPortName(info.targetPortName);

		rmport->setPeer(mpImpl);
		*/
		//
				//   아래와 같이EventPortLink를 사용해야 합니다.
				//
				//

		EventPortConnectorImpl *d_con = new EventPortConnectorImpl(cmgr->getComponentRequester()); // DataPortConnector는 Factory에서 만들어져야 함.
		RemoteEventPort *t_port = portable_dynamic_cast(RemoteEventPort *, d_con);

		d_con->setTargetComponentName(info.targetComponentName);
		d_con->setTargetPortName(info.targetPortName);

		EventPortLink *link = getEventPortLink(info.sourcePortName);
		link->setSource(oeport);
		link->addDestination(t_port);

		OutputEventPortInterface *g_oeport = portable_dynamic_cast(OutputEventPortInterface *, oeport);
		g_oeport->setArchiveFactory(armgr->getDefaultArchiveFactory());
	}
	else {
		// target
		EventPort *ieport = target->getInputEventPort(info.targetPortName);
		if (ieport == NULL) {
			log_error("Not exists EventPort : name=" << info.targetPortName);
			return false;
		}

		// connecting
		EventPortLink *link = getEventPortLink(info.sourcePortName);
		link->setSource(oeport);
		link->addDestination(ieport);
	}

	return true;
}

bool ComponentAdapterAtomic::disconnectServicePort(PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName);

	// disconnecting
	RequiredServicePort *rsport = getRequiredServicePort(info.sourcePortName);
	if (rsport != NULL) rsport->Disconnect();

	return true;
}

bool ComponentAdapterAtomic::disconnectDataPort(PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName);

	// 데이터 포트 링크를 지운다.
	removeDataPortLink(info.sourcePortName);
	return true;
}

bool ComponentAdapterAtomic::disconnectEventPort(PortConnectionInfo &info)
{
	trace_enter_msg("sourcePortName=" << info.sourcePortName);

	// 이벤트 포트 링크를 지운다.
	removeEventPortLink(info.sourcePortName);
	return true;
}
