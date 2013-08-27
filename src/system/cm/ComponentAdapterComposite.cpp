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

#include "ComponentAdapterComposite.h"

#include "ComponentManager.h"

#include "system/SystemTypes.h"
#include "system/config/ConfigLoaderFactory.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/profile/CoordinationInfo.h"

#include <list>

using namespace std;

ComponentAdapterComposite::ComponentAdapterComposite()
{	
	m_profile = NULL;
}

ComponentAdapterComposite::~ComponentAdapterComposite()
{
	trace_enter();

	OSGuard guard(&m_lock);

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL) {
			ca->unload();
		}
	}

	m_caList.clear();

	if (m_profile != NULL) delete m_profile;
}

ComponentAdapterPtr ComponentAdapterComposite::getChildComponent(const string &name)
{
	ComponentAdapterPtr ca(NULL);

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		if ((*iter)->getId() == name) return *iter;
	}
	return ca;
}

bool ComponentAdapterComposite::doLoad()
{
	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));

	std::string subparent = this->getParentID();
	subparent.append(".").append(this->getId());
	ComponentAdapterPtr temp_ca;


	ComponentInfoList::iterator iter = m_profile->componentList.begin();
	

	for (; iter != m_profile->componentList.end(); ++iter) {
		ComponentInfo &cinfo = *iter;
		ComponentAdapter *ca = cmgr->loadSubComponent(cinfo.node_id, subparent, cinfo.name, m_profile->getPath(), cinfo.reference);

		if (ca == NULL) {  
			//
				doUnload();

				return false;
		}

		
		CoordinationUnit *cu = ca->getCoordinationUnit();

		CoordinationInfoList::iterator coorditer = m_profile->coordinationInfoList.begin();

		// composite coordination setup
		for (; coorditer != m_profile->coordinationInfoList.end(); ++coorditer) {
			CoordinationInfo &coordinfo = *coorditer;

			if (!coordinfo.sourceComponentName.compare(cinfo.name))
			{
				temp_ca = getChildComponent(coordinfo.targetComponentName);
				if (!temp_ca.isNULL())
				{
					(*cu).addTrigger(temp_ca->getCoordinationUnit());
				}				
			}
			else
			if (!coordinfo.targetComponentName.compare(cinfo.name))
			{
				temp_ca = getChildComponent(coordinfo.sourceComponentName);
				if (!temp_ca.isNULL())
				{
					(*cu).addFollower(temp_ca->getCoordinationUnit());
				}						
			}
		}

		//

		(*cu).setParent(&this->m_coord);

		
		// remove local circle; trigger == follower
		std::map<CoordinationUnit *, int>::iterator xxiter = (*cu).getTriggerFirst();

		for (;xxiter != (*cu).getTriggerEnd(); ++xxiter)
		{
			CoordinationUnit *nstr = (*xxiter).first;			

			(*cu).removeFollower(nstr);
		}



		CoordinationUnit* lastAfter;
		int lastIndex = -1;			

			//
		std::map<CoordinationUnit *, int>::iterator ttiter = (*cu).getFollowerFirst();

		m_lock.lock();

		for (;ttiter != (*cu).getFollowerEnd(); ++ ttiter)
		{
			CoordinationUnit *nstr = (*ttiter).first;
			ComponentAdapterList::iterator pkgIter = m_caList.begin();
			int index = 0;

			for (;pkgIter != m_caList.end(); ++ pkgIter)
			{
				ComponentAdapter *ppkg = (*pkgIter).get();
				index++;					

				if (nstr == ppkg->getCoordinationUnit())
				{
					if (index > lastIndex)
					{
						lastAfter = nstr;
						lastIndex = index;
					}
					break;
				}
			}
		}
		
		
		if (lastIndex == -1)  // 
		{
			ComponentAdapterPtr s_ptr(ca);
			m_caList.push_front(s_ptr);
		}
		else
		{
			ComponentAdapterList::iterator pkgIter = m_caList.begin();
			for (;pkgIter != m_caList.end(); ++ pkgIter)
			{
				ComponentAdapter *ppkg = (*pkgIter).get();
					
				if (lastAfter==ppkg->getCoordinationUnit())
				{
					ComponentAdapterPtr s_ptr(ca);
					m_caList.insert(pkgIter,s_ptr);
					
					break;
				}
			}	
		}	 
		
		m_lock.unlock();
		
	}

	// 가상 export 포트를 만들어야 함

	ExportPortInfoList &list = m_profile->exportPortList;
	ExportPortInfoList::iterator exiter = list.begin();

	for (; exiter != list.end(); ++exiter) {
		ExportPortInfo &epinfo = *exiter;

		if (!epinfo.portType.compare("data"))
		{
			if (!epinfo.portDir.compare("input"))
			{
				CompositeInputDataPortList::iterator iter = m_dataport_list.find(epinfo.exportPortName);
				if (iter != m_dataport_list.end())
				{
					CompositeInputDataPort *mp = iter->second;


					ComponentAdapterPtr ca = getChildComponent(epinfo.componentName);
					if (!ca.isNULL())
					{
						DataPort *dport = ca->getInputDataPort(epinfo.portName);
						if (dport != NULL)
						{
							mp->addPort(dport);
						}
						else
						{
							log_error("Not exists Input DataPort : name=" << epinfo.componentName <<":"<< epinfo.portName);
						}
					}
					else
					{
						log_error("Not exists ChildComponent : name=" <<  epinfo.componentName <<":"<< epinfo.portName);
					}
				}
				else // not found CompositeInputDataPort
				{
					CompositeInputDataPort *mp = new CompositeInputDataPort(epinfo.exportPortName);
					if (mp != NULL)
					{
						ComponentAdapterPtr ca = getChildComponent(epinfo.componentName);
						if (!ca.isNULL())
						{
							DataPort *dport = ca->getInputDataPort(epinfo.portName);
							if (dport != NULL)
							{
								mp->addPort(dport);
							}
							else
							{
								delete mp;
								mp = NULL;
								log_error("Not exists Input DataPort : name=" << epinfo.componentName <<":"<< epinfo.portName);
							}
						}
						else
						{
							delete mp;
							mp = NULL;
							log_error("Not exists ChildComponent : name=" <<  epinfo.componentName <<":"<< epinfo.portName);
						}
						if (mp != NULL)
						{
							m_dataport_list[epinfo.exportPortName] = mp;
						}
					}

				}
			}
		}
		else
		if (!epinfo.portType.compare("event"))
		{
			if (!epinfo.portDir.compare("input"))
			{
				CompositeInputEventPortList::iterator iter = m_eventport_list.find(epinfo.exportPortName);
				if (iter != m_eventport_list.end())
				{
					CompositeInputEventPort *mp = iter->second;


					ComponentAdapterPtr ca = getChildComponent(epinfo.componentName);
					if (!ca.isNULL())
					{
						EventPort *dport = ca->getInputEventPort(epinfo.portName);
						if (dport != NULL)
						{
							mp->addPort(dport);
						}
						else
						{
							log_error("Not exists Input EventPort : name=" << epinfo.componentName <<":"<< epinfo.portName);
						}
					}
					else
					{
						log_error("Not exists ChildComponent : name=" <<  epinfo.componentName <<":"<< epinfo.portName);
					}
				}
				else // not found CompositeInputDataPort
				{
					CompositeInputEventPort *mp = new CompositeInputEventPort(epinfo.exportPortName);
					if (mp != NULL)
					{
						ComponentAdapterPtr ca = getChildComponent(epinfo.componentName);
						if (!ca.isNULL())
						{
							EventPort *dport = ca->getInputEventPort(epinfo.portName);
							if (dport != NULL)
							{
								mp->addPort(dport);
							}
							else
							{
								delete mp;
								mp = NULL;
								log_error("Not exists Input EventPort : name=" << epinfo.componentName <<":"<< epinfo.portName);
							}
						}
						else
						{
							delete mp;
							mp = NULL;
							log_error("Not exists ChildComponent : name=" <<  epinfo.componentName <<":"<< epinfo.portName);
						}
						if (mp != NULL)
						{
							m_eventport_list[epinfo.exportPortName] = mp;
						}
					}
				}
			}

		}
	}


	return true;
}

bool ComponentAdapterComposite::doUnload()
{
	trace_enter_msg("name=" << getId());

	OSGuard guard(&m_lock);

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->unload()) {
			isOk = false;
			
		}
	}

	if (isOk) m_caList.clear();


	CompositeInputDataPortList::iterator dataiter = m_dataport_list.begin();

	for (; dataiter != m_dataport_list.end(); ++dataiter) {
		CompositeInputDataPort *cip = dataiter->second;
		delete cip;
	}

	m_dataport_list.clear();




	CompositeInputEventPortList::iterator eventiter = m_eventport_list.begin();

	for (; eventiter != m_eventport_list.end(); ++eventiter) {
		CompositeInputEventPort *cep = eventiter->second;
		delete cep;
	}

	m_eventport_list.clear();


	return isOk;
}

//
// port connection
//
bool ComponentAdapterComposite::doConnectPort()
{
	trace_enter();

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->connectPort()) {
			return false;
		}
	}

	return connectPortThis();
}

bool ComponentAdapterComposite::doDisconnectPort()
{
	trace_enter();

	bool isOk;
	if (disconnectPortThis()) isOk = true;
	else isOk = false;

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->disconnectPort()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::connectPortThis()
{
	trace_enter_msg("name=" + getId());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	return cmgr->getComponentPortConnector()->connectPortList(m_caList, m_profile->portConnectionList);
}

bool ComponentAdapterComposite::disconnectPortThis()
{
	trace_enter_msg("name=" + getId());

	ComponentManager *cmgr = portable_dynamic_cast(ComponentManager *, Registry::getRegistry()->getManager(COMPONENT_MANAGER));
	return cmgr->getComponentPortConnector()->disconnectPortList(m_caList, m_profile->portConnectionList);
}

bool ComponentAdapterComposite::doActivate()
{
	trace_enter_msg("name=" << getId());

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->activate()) {
			return false;
		}
	}

	return true;
}

bool ComponentAdapterComposite::doPassivate()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->passivate()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doInitialize()
{
	trace_enter_msg("name=" + getId());

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doInitialize()) {
			return false;
		}
	}
	return true;
}

bool ComponentAdapterComposite::doStart()
{
	trace_enter_msg("name=" + getId());

	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doStart()) {
			return false;
		}
	}
	return true;
}

bool ComponentAdapterComposite::doStop()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doStop()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doReset()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doReset()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doStopOnError()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doStopOnError()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doRecover()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doRecover()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doDestroy()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doDestroy()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doExecute()
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doExecute()) {
			isOk = false;
		}
	}

	return isOk;
}

bool ComponentAdapterComposite::doEvent(Event *evt)
{
	trace_enter_msg("name=" << getId());

	bool isOk = true;
	ComponentAdapterList::iterator iter = m_caList.begin();
	for (; iter != m_caList.end(); ++iter) {
		ComponentAdapter *ca = (*iter).get();
		if (ca != NULL && !ca->doEvent(evt)) {
			isOk = false;
		}
	}

	return isOk;
}

ComponentAdapterPtr ComponentAdapterComposite::findComponentAdapter(const std::string &portName, \
																  ComponentPortType portType, ComponentPortDir portDir)
{
	trace_enter_msg("name=" + getId());

	ExportPortInfo *info = getExportPortInfo(portName, portType, portDir);
	if (info == NULL) {
		log_error("Not exists RequiredMethodPort : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	return getChildComponent(info->componentName);
}

std::list<ExportPortInfo *> ComponentAdapterComposite::getExportPortInfoList(const string &exportPortName, 
															 ComponentPortType portType,  ComponentPortDir portDir)
{
	std::list<ExportPortInfo *> results; 

 	trace_enter_msg("name=" + getId());
	string portTypeStr;
	string portDirStr;

	switch (portType) {
		case CA_PORT_SERVICE : portTypeStr = "service"; break;
		case CA_PORT_DATA   : portTypeStr = "data"; break;
		case CA_PORT_EVENT  : portTypeStr = "event"; break;
		default :
			log_error("Invalid port type : portType=" << portType);
			return results;
	}

	switch (portDir) {
		case CA_PORT_REQUIRED : portDirStr = "required"; break;
		case CA_PORT_PROVIDED : portDirStr = "provided"; break;
		case CA_PORT_INPUT    : portDirStr = "input"; break;
		case CA_PORT_OUTPUT   : portDirStr = "output"; break;
		default :
			log_error("Invalid port dir : portDir=" << portDir);
			return results;
	}


	trace("[] Input.exportPortName=" << exportPortName);
	trace("   - Input.portType=" << portTypeStr);
	trace("   - Input.portDir=" << portDirStr);

	ExportPortInfoList &list = m_profile->exportPortList;
	ExportPortInfoList::iterator iter = list.begin();

	

	for (; iter != list.end(); ++iter) {
		ExportPortInfo &epinfo = *iter;

		trace("   ---> exportPortName=" << epinfo.exportPortName);
		trace("        - portType=" << epinfo.portType);
		trace("        - portDir=" << epinfo.portDir);

		if (epinfo.exportPortName == exportPortName && epinfo.portType == portTypeStr && epinfo.portDir == portDirStr) {
			trace("   >>>>> Found : exportPortName=" << epinfo.exportPortName);

			results.push_back(&epinfo);			
		}
	}

	if (results.size()==0)
		trace("   >>>>> Not Exists : exportPortName=" << exportPortName);
	return results;
}


ExportPortInfo *ComponentAdapterComposite::getExportPortInfo(const string &exportPortName, \
															 ComponentPortType portType, \
															 ComponentPortDir portDir)
{
	trace_enter_msg("name=" + getId());
	string portTypeStr;
	string portDirStr;

	switch (portType) {
		case CA_PORT_SERVICE : portTypeStr = "service"; break;
		case CA_PORT_DATA   : portTypeStr = "data"; break;
		case CA_PORT_EVENT  : portTypeStr = "event"; break;
		default :
			log_error("Invalid port type : portType=" << portType);
			return NULL;
	}

	switch (portDir) {
		case CA_PORT_REQUIRED : portDirStr = "required"; break;
		case CA_PORT_PROVIDED : portDirStr = "provided"; break;
		case CA_PORT_INPUT    : portDirStr = "input"; break;
		case CA_PORT_OUTPUT   : portDirStr = "output"; break;
		default :
			log_error("Invalid port dir : portDir=" << portDir);
			return NULL;
	}


	trace("[] Input.exportPortName=" << exportPortName);
	trace("   - Input.portType=" << portTypeStr);
	trace("   - Input.portDir=" << portDirStr);

	ExportPortInfoList &list = m_profile->exportPortList;
	ExportPortInfoList::iterator iter = list.begin();

	for (; iter != list.end(); ++iter) {
		ExportPortInfo &epinfo = *iter;

		trace("   ---> exportPortName=" << epinfo.exportPortName);
		trace("        - portType=" << epinfo.portType);
		trace("        - portDir=" << epinfo.portDir);

		if (epinfo.exportPortName == exportPortName && epinfo.portType == portTypeStr && epinfo.portDir == portDirStr) {
			trace("   >>>>> Found : exportPortName=" << epinfo.exportPortName);
			return &epinfo;
		}
	}

	trace("   >>>>> Not Exists : exportPortName=" << exportPortName);
	return NULL;
}

RequiredServicePort *ComponentAdapterComposite::getRequiredServicePort(const string &portName)
{
	trace_enter_msg("name=" + getId());

	ExportPortInfo *info = getExportPortInfo(portName, CA_PORT_SERVICE, CA_PORT_REQUIRED);
	if (info == NULL) {
		log_error("Not exists RequiredMethodPort : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	ComponentAdapterPtr ca = getChildComponent(info->componentName);
	if (ca.isNULL()) {
		log_error("Not exists ChildComponent : name=" << info->componentName);
		return NULL;
	}

	return ca->getRequiredServicePort(info->portName);
}
ProvidedServicePort *ComponentAdapterComposite::getProvidedServicePort(const string &portName)
{
	trace_enter_msg("name=" + getId());

	ExportPortInfo *info = getExportPortInfo(portName, CA_PORT_SERVICE, CA_PORT_PROVIDED);
	if (info == NULL) {
		log_error("Not exists ProvidedMethodPort : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	ComponentAdapterPtr ca = getChildComponent(info->componentName);
	if (ca.isNULL()) {
		log_error("Not exists ChildComponent : name=" << info->componentName);
		return NULL;
	}

	return ca->getProvidedServicePort(info->portName);
}

DataPort *ComponentAdapterComposite::getOutputDataPort(const string &portName)
{
	trace_enter_msg("name=" + getId());

	ExportPortInfo *info = getExportPortInfo(portName, CA_PORT_DATA, CA_PORT_OUTPUT);
	if (info == NULL) {
		log_error("Not exists OutputDataPort : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	ComponentAdapterPtr ca = getChildComponent(info->componentName);
	if (ca.isNULL()) {
		log_error("Not exists ChildComponent : name=" << info->componentName);
		return NULL;
	}

	return ca->getOutputDataPort(info->portName);
}

DataPort *ComponentAdapterComposite::getInputDataPort(const string &portName)
{
	trace_enter_msg("name=" + getId());

	CompositeInputDataPortList::iterator iter = m_dataport_list.find(portName);
	if (iter != m_dataport_list.end())
		return iter->second;

	return NULL;
}


EventPort *ComponentAdapterComposite::getOutputEventPort(const string &portName)
{
	trace_enter_msg("name=" + getId());

	ExportPortInfo *info = getExportPortInfo(portName, CA_PORT_EVENT, CA_PORT_OUTPUT);
	if (info == NULL) {
		log_error("Not exists OutputEventPort : cname=" << getId() << ", pname=" << portName);
		return NULL;
	}

	ComponentAdapterPtr ca = getChildComponent(info->componentName);
	if (ca.isNULL()) {
		log_error("Not exists ChildComponent : name=" << info->componentName);
		return NULL;
	}

	return ca->getOutputEventPort(info->portName);
}

EventPort *ComponentAdapterComposite::getInputEventPort(const string &portName)
{
	trace_enter_msg("name=" + getId());

	CompositeInputEventPortList::iterator iter = m_eventport_list.find(portName);
	if (iter != m_eventport_list.end())
		return iter->second;

	return NULL;
}


//------------------
// port connection
//
bool ComponentAdapterComposite::connectServicePort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	std::list<ExportPortInfo *> results = getExportPortInfoList(pcInfo.sourcePortName, CA_PORT_SERVICE, CA_PORT_REQUIRED);
	if (results.size()==0) {
		log_error("Does not exist ExportPortInfo");
		return false;
	}

	std::list<ExportPortInfo *>::iterator it = results.begin();

	for (;it != results.end(); ++it)
	{
		ExportPortInfo *pinfo = *it;

		ComponentAdapterPtr ca = getChildComponent(pinfo->componentName);
		if (ca.isNULL()) {
			log_error("Not exists ChildComponent : name=" << pinfo->componentName);
			return false;
		}

		PortConnectionInfo tmp = pcInfo;
		tmp.sourceComponentName = pinfo->componentName;
		tmp.sourcePortName = pinfo->portName;
		ca->connectServicePort(target, tmp);
	}

	return true;
}

bool ComponentAdapterComposite::connectDataPort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	std::list<ExportPortInfo *> results = getExportPortInfoList(pcInfo.sourcePortName, CA_PORT_DATA, CA_PORT_OUTPUT);
	if (results.size()==0) {
		log_error("Does not exist ExportPortInfo");
		return false;
	}

	std::list<ExportPortInfo *>::iterator it = results.begin();

	for (;it != results.end(); ++it)
	{
		ExportPortInfo *pinfo = *it;

		ComponentAdapterPtr ca = getChildComponent(pinfo->componentName);
		if (ca.isNULL()) {
			log_error("Not exists ChildComponent : name=" << pinfo->componentName);
			return false;
		}

		PortConnectionInfo tmp = pcInfo;
		tmp.sourceComponentName = pinfo->componentName;
		tmp.sourcePortName = pinfo->portName;
		ca->connectDataPort(target, tmp);
	}

	return true;
}


bool ComponentAdapterComposite::connectEventPort(ComponentAdapterPtr target, PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName << ",targetPortName=" << pcInfo.targetPortName) ;

	std::list<ExportPortInfo *> results = getExportPortInfoList(pcInfo.sourcePortName, CA_PORT_EVENT, CA_PORT_OUTPUT);
	if (results.size()==0) {
		log_error("Does not exist ExportPortInfo");
		return false;
	}

	std::list<ExportPortInfo *>::iterator it = results.begin();

	for (;it != results.end(); ++it)
	{
		ExportPortInfo *pinfo = *it;

		ComponentAdapterPtr ca = getChildComponent(pinfo->componentName);
		if (ca.isNULL()) {
			log_error("Not exists ChildComponent : name=" << pinfo->componentName);
			return false;
		}

		PortConnectionInfo tmp = pcInfo;
		tmp.sourceComponentName = pinfo->componentName;
		tmp.sourcePortName = pinfo->portName;
		ca->connectEventPort(target, tmp);
	}

	return true;
}

bool ComponentAdapterComposite::disconnectServicePort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	std::list<ExportPortInfo *> results = getExportPortInfoList(pcInfo.sourcePortName, CA_PORT_SERVICE, CA_PORT_REQUIRED);
	if (results.size()==0) {
		log_error("Does not exist ExportPortInfo");
		return false;
	}

	std::list<ExportPortInfo *>::iterator it = results.begin();

	for (;it != results.end(); ++it)
	{
		ExportPortInfo *pinfo = *it;

		ComponentAdapterPtr ca = getChildComponent(pinfo->componentName);
		if (ca.isNULL()) {
			log_error("Not exists ChildComponent : name=" << pinfo->componentName);
			return false;
		}

		PortConnectionInfo tmp = pcInfo;
		tmp.sourceComponentName = pinfo->componentName;
		tmp.sourcePortName = pinfo->portName;
		ca->disconnectServicePort(tmp);
	}

	return true;
}




bool ComponentAdapterComposite::disconnectDataPort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	std::list<ExportPortInfo *> results = getExportPortInfoList(pcInfo.sourcePortName, CA_PORT_DATA, CA_PORT_OUTPUT);
	if (results.size()==0) {
		log_error("Does not exist ExportPortInfo");
		return false;
	}

	std::list<ExportPortInfo *>::iterator it = results.begin();

	for (;it != results.end(); ++it)
	{
		ExportPortInfo *pinfo = *it;

		ComponentAdapterPtr ca = getChildComponent(pinfo->componentName);
		if (ca.isNULL()) {
			log_error("Not exists ChildComponent : name=" << pinfo->componentName);
			return false;
		}

		PortConnectionInfo tmp = pcInfo;
		tmp.sourceComponentName = pinfo->componentName;
		tmp.sourcePortName = pinfo->portName;
		ca->disconnectDataPort(tmp);
	}

	return true;
}


bool ComponentAdapterComposite::disconnectEventPort(PortConnectionInfo &pcInfo)
{
	trace_enter_msg("sourcePortName=" << pcInfo.sourcePortName);

	std::list<ExportPortInfo *> results = getExportPortInfoList(pcInfo.sourcePortName, CA_PORT_EVENT, CA_PORT_OUTPUT);
	if (results.size()==0) {
		log_error("Does not exist ExportPortInfo");
		return false;
	}

	std::list<ExportPortInfo *>::iterator it = results.begin();

	for (;it != results.end(); ++it)
	{
		ExportPortInfo *pinfo = *it;

		ComponentAdapterPtr ca = getChildComponent(pinfo->componentName);
		if (ca.isNULL()) {
			log_error("Not exists ChildComponent : name=" << pinfo->componentName);
			return false;
		}

		PortConnectionInfo tmp = pcInfo;
		tmp.sourceComponentName = pinfo->componentName;
		tmp.sourcePortName = pinfo->portName;
		ca->disconnectEventPort(tmp);
	}

	return true;
}
