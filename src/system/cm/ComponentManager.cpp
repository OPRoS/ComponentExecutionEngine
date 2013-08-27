/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentManager.cpp
 *
 */

#include "ComponentVerifier.h"
#include "ComponentManager.h"

#include "ComponentLoader.h"
#include "ComponentLoaderDll.h"

#include "ComponentAdapterAtomic.h"
#include "ComponentAdapterComposite.h"
#include "ComponentAdapterRemoteProxy.h"
#include "ComponentAdapterLocalProxy.h"

// #include "ft/FTExecutorManager.h"
#include "ExecutorManagerShared.h"

#include "system/SystemTypes.h"
#include "system/config/ConfigLoaderFactory.h"
#include "system/os/OSGuard.h"
#include "system/io/IoManager.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/util/Exceptions.h"

class Component;

using namespace std;

ComponentManager::ComponentManager()
{
	m_pExecMgr = NULL;
}

ComponentManager::~ComponentManager()
{
	trace_enter();

	// 실행기를 중지시킴
	if (m_pExecMgr != NULL) {
		delete m_pExecMgr;
	}
}

bool ComponentManager::onInit()
{
	trace_enter();

	Config *cfg = getConfig();
	if (cfg == NULL) return true;

	//-----------------------------------
	// io_connector loading
	//
	Config *ioccfg = cfg->getChild("io_connector");
	if (ioccfg != NULL ) {
		m_cnnHandler = ioccfg->getChildValue("connection_handler", "component_io");
		m_evtProcessor = ioccfg->getChildValue("event_processor", "system");
	}
	else {
		m_cnnHandler = "component_io";
		m_evtProcessor = "system";
	}
	trace("connection_handler=" << m_cnnHandler << "; event_processor=" << m_evtProcessor);

	//-------------------------------------
	// local node id setting
	//
	NodeInfo *localNode = new NodeInfo();
	localNode->node_id = cfg->getChildValue("local_node_id", "__local_node__");
	localNode->io.protocol = "";
	localNode->io.props.setProperty("cnn.handler", m_cnnHandler);
	localNode->io.props.setProperty("evt.processor", m_evtProcessor);

	m_nodeLocator.addNode(localNode);
	m_nodeLocator.setLocalNodeId(localNode->node_id);
	Registry::getRegistry()->setNodeId(localNode->node_id);

	trace("local_node_id=" << m_nodeLocator.getLocalNodeId());

	// ---------------------
	// node_map setting
	//
	Config *nmcfg = cfg->getChild("node_map");
	if (nmcfg != NULL) {
		ConfigList list = nmcfg->getChildren("node");
		ConfigList::iterator pos = list.begin();
		for(; pos != list.end(); ++pos) {
			NodeInfo *info = new NodeInfo();
			Config *child = *pos;
			if (info->load(child)) {
				info->io.props.setProperty("cnn.handler", m_cnnHandler);
				info->io.props.setProperty("evt.processor", m_evtProcessor);
				m_nodeLocator.addNode(info);

				trace("node_id=" << info->node_id << ", protcol=" << info->io.protocol);
			}
			else {
				delete info;
			}
		}
	}

	m_requester.setComponentLocator(&m_componentLocator);
	m_requester.setNodeLocator(&m_nodeLocator);
	m_requester.setup(cfg->getChildValueAsInt("workers", 10));

	IoManager *iom = portable_dynamic_cast(IoManager *, Registry::getRegistry()->getManager(IO_MGNAGER));
	if (iom != NULL) {
		m_requester.setConnectionHandlerId(m_cnnHandler);

		if (!iom->addConnectionHandler(&m_requester))
		{
			// add connectionHandler error
			// 현재로서는 별다른 처리 방법이 없음.
		}
	}


	// ExecutorManager 생성
	m_pExecMgr = (ExecutorManager *) new ExecutorManagerShared();

	// Verifier Setting
#ifdef	MOD_VERIFY_COMPONENT
	cvm.init();
#endif

	return true;
}

bool ComponentManager::onStart()
{
	trace_enter();

	return true;
}

bool ComponentManager::onStop()
{
	trace_enter();

	unloadComponentAll();
	m_requester.close();

	return true;
}

bool ComponentManager::onDestroy()
{
	trace_enter();

	return true;
}



ComponentAdapterPtr ComponentManager::getComponentAdapter(const string &name)
{
	trace_enter_msg("name=" + name);

	OSGuard guard(&m_caListLock);

	ComponentAdapterPtr ca(NULL);

	ComponentAdapterList::iterator pos = m_caList.begin();
	for (; pos != m_caList.end(); ++pos) {
		if ((*pos)->getId() == name) {
			ca = (*pos);
			break;
		}
	}
	return ca;
}

void ComponentManager::addComponentAdapter(ComponentAdapter *ca)
{
	trace_enter();

	OSGuard guard(&m_caListLock);

	ComponentAdapterPtr t_ptr(ca);

	m_caList.push_back(t_ptr);
}

void ComponentManager::removeComponentAdapter(const std::string &name)
{
	trace_enter();

	OSGuard guard(&m_caListLock);

	ComponentAdapterList::iterator pos = m_caList.begin();
	for (; pos != m_caList.end(); ++pos) {
		if ((*pos)->getId() == name) {
			m_caList.erase(pos);
			break;
		}
	}
}

ComponentAdapterPtr ComponentManager::getComponentAdapter(const string &parent, const string &name)
{
	trace_enter_msg("name=" + name);

	OSGuard guard(&m_caListLock);

	ComponentAdapterPtr ca(NULL);
	ComponentAdapterList::iterator pos = m_caList.begin();
	for (; pos != m_caList.end(); ++pos) {
		if ((*pos)->getId() == name && (*pos)->getParentID() == parent) {
			ca = (*pos);
			break;
		}
	}
	return ca;
}


ComponentAdapter *ComponentManager::loadComponent(const string &nodeId, const string &parentid , const string &name, const string &path, const string &ref)
{
	trace_enter_msg("nodeId=" << nodeId << ",name=" << name << ",parentid=" << parentid << ",path=" << path << ",ref=" << ref);
	ComponentAdapter *ca = NULL;

	string nid;
	if (nodeId == "") nid = getNodeLocator()->getLocalNodeId();
	else nid = nodeId;

	if (name=="")
	{
		string profilePath = path + ref;

		ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();
		if (clf==NULL)
		{
			log_error("ConfigLoadFactory Loading Error");
			return NULL;
		}

		ConfigLoader *loader = clf->getConfigLoader(ref);
		if (loader == NULL)
		{
			log_error("getConfigLoader Loading Error");
			return NULL;
		}

		Config *cfg = loader->loadFromFile(profilePath);
		if (cfg == NULL) {
			log_error("PackageProfile Config Loading Error");
			return NULL;
		}

		if (cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG) != NULL) {
			log_debug("*** [Composite Component] *************************");
			log_debug("    - path=" << path);
			log_debug("    - ref=" << ref);

			CompositeComponentProfile *profile = new  CompositeComponentProfile();
			if (!profile->parse(cfg)) {
				delete profile;
				delete cfg;
				log_error("CompositeComponentProfile Parsing Error");
				return NULL;
			}
			string dirName = OS::splitExt(ref);
			profile->setPath(OS::toDir(path + dirName));

			ComponentAdapterPtr t_ca = getComponentAdapter( profile->getName()); //parentid, name);
			if (!t_ca.isNULL()) {
				//m_caListLock.unlock();

				log_debug("Component already exists : name=" << ca->getId());

				//if (ca->load() == false) {
				//	log_error("Existed Component load failed : name=" << ca->getId());
				//	ca = NULL;
				//}
				//return ca;
				return NULL;
			}

			ca = loadLocalComponent(parentid, profile);

			delete cfg;


		}else 	if (cfg->getChild(COMPONENT_PROFILE_TAG) != NULL) {
			log_debug("*** [Atomic Component] *************************");
			log_debug("    - path=" << path);
			log_debug("    - ref=" << ref);

			// secure package testing.
	#ifdef	MOD_VERIFY_COMPONENT
			if (cvm.verifyComponent(path, cfg)>0)
			{
				delete cfg;
				// secure mismatch
				log_error("Component Verification Error [component binary is damaged]");

				string mesg = "Component Verification Fail : component.name ->";
				mesg.append(name);

				throw ComponentException(mesg);
				//return NULL;
			}	
	#endif

			ComponentProfile *profile = new  ComponentProfile();
			if (!profile->parse(cfg)) {
				delete profile;
				delete cfg;
				log_error("ComponentProfile Parsing Error");
				return NULL;
			}
			profile->setPath(path);
			ComponentAdapterPtr t_ca = getComponentAdapter( profile->getName()); //parentid, name);
			if (!t_ca.isNULL()){
				//m_caListLock.unlock();

				log_debug("Component already exists : name=" << ca->getId());

				//if (ca->load() == false) {
				//	log_error("Existed Component load failed : name=" << ca->getId());
				//	ca = NULL;
				//}
				//return ca;
				return NULL;
			}

			ca = loadLocalComponent(parentid, profile);

			delete cfg;

		}
		else {
			delete cfg;
			log_error("Unknown Package Type");
			return NULL;
		}

	}
	else
	{

		//m_caListLock.lock();
		ComponentAdapterPtr t_ca = getComponentAdapter(name); //parentid, name);
		if (!t_ca.isNULL()){
		//	m_caListLock.unlock();

			log_debug("Component already exists : name=" << ca->getId());

			//if (ca->load() == false) {
			//	log_error("Existed Component load failed : name=" << ca->getId());
			//	ca = NULL;
			//}
			//return ca;
			return NULL;
		}
		//	m_caListLock.unlock();

		if (nid != getNodeLocator()->getLocalNodeId()) {
			log_debug("ComponentAdapterRemoteProxy created : nodeId= " << nodeId);
			ComponentAdapterRemoteProxy *proxy = new ComponentAdapterRemoteProxy(nid, name);
			m_componentLocator.addComponentLocation(name, nid);

			ca = proxy;
		}
		else {
			ca = loadLocalComponent(nodeId,parentid, name, path, ref);
		}
	}

	if (ca != NULL) {
		ca->setParentID(parentid);

		addComponentAdapter(ca);
	}

	return ca;
}

ComponentAdapter *ComponentManager::loadSubComponent(const string &nodeId,const string &parent, const string &name, const string &path, const string &ref)
{
	trace_enter_msg("nodeId=" << nodeId << ", name=" << name << ", path=" << path << ", ref=" << ref);

	string nid;
	if (nodeId == "") nid = getNodeLocator()->getLocalNodeId();
	else nid = nodeId;

	return loadLocalComponent(nid, parent, name, path, ref);
}

ComponentAdapter *ComponentManager::loadLocalComponent(const string &parent, CompositeComponentProfile *profile)
{
		ComponentAdapterComposite *composite = new ComponentAdapterComposite();
		composite->setCompositeComponentProfile(profile);
		
		composite->setParentID(parent);

		if (!composite->load()) {
			delete composite;
			return NULL;		
		}

		return composite;
}

ComponentAdapter *ComponentManager::loadLocalComponent(const string &parent, ComponentProfile *profile)
{
		ComponentAdapterAtomic *atomic = new ComponentAdapterAtomic();
		atomic->setAtomicComponentProfile(profile);

		atomic->setParentID(parent);

		if (!atomic->load()) {
			delete atomic;
			return NULL;
		}

		return atomic;
}

ComponentAdapter *ComponentManager::loadLocalComponent(const string &nodeId, const string &parent, const string &name, const string &path, const string &ref)
{
	trace_enter_msg("nodeId=" << nodeId << ", name=" << name << ", path=" << path << ", ref=" << ref);

	// profile loading
	string profilePath = path + ref;

	trace("profilePath : " << profilePath);

	ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();
	if (clf==NULL)
	{
		log_error("ConfigLoadFactory Loading Error");
		return NULL;
	}

	ConfigLoader *loader = clf->getConfigLoader(ref);
	Config *cfg = loader->loadFromFile(profilePath);
	if (cfg == NULL) {
		log_error("PackageProfile Config Loading Error");
		return NULL;
	}

	ComponentAdapter *ca = NULL;
	if (cfg->getChild(COMPOSITE_COMPONENT_PROFILE_TAG) != NULL) {
		log_debug("*** [Composite Component] *************************");
		log_debug("    - path=" << path);
		log_debug("    - ref=" << ref);

		CompositeComponentProfile *profile = new  CompositeComponentProfile();
		if (!profile->parse(cfg)) {
			delete profile;
			delete cfg;
			log_error("CompositeComponentProfile Parsing Error");
			return NULL;
		}
		string dirName = OS::splitExt(ref);
		profile->setPath(OS::toDir(path + dirName));

		ComponentAdapterComposite *composite = new ComponentAdapterComposite();
		composite->setCompositeComponentProfile(profile);
		ca = composite;
	}
	else if (cfg->getChild(COMPONENT_PROFILE_TAG) != NULL) {
		log_debug("*** [Atomic Component] *************************");
		log_debug("    - path=" << path);
		log_debug("    - ref=" << ref);

		// secure package testing.
#ifdef	MOD_VERIFY_COMPONENT
		if (cvm.verifyComponent(path, cfg)>0)
		{
			delete cfg;
			// secure mismatch
			log_error("Component Verification Error [component binary is damaged]");

			string mesg = "Component Verification Fail : component.name ->";
			mesg.append(name);

			throw ComponentException(mesg);
			//return NULL;
		}	
#endif

		ComponentProfile *profile = new  ComponentProfile();
		if (!profile->parse(cfg)) {
			delete profile;
			delete cfg;
			log_error("ComponentProfile Parsing Error");
			return NULL;
		}
		profile->setPath(path);

		ComponentAdapterAtomic *atomic = new ComponentAdapterAtomic();
		atomic->setAtomicComponentProfile(profile);
		ca = atomic;
	}
	else {
		delete cfg;
		log_error("Unknown Package Type");
		return NULL;
	}

	ca->setParentID(parent);

	if (!ca->load()) {
		delete ca;
		ca = NULL;
	}

	delete cfg;



	return ca;
}

bool ComponentManager::unloadComponent(const string &name)
{
	trace_enter();


	ComponentAdapterPtr ca = getComponentAdapter(name);

	if (ca.isNULL()) return true;

	if (ca->unload()) {
		if (ca->isRef() == false) {
			removeComponentAdapter(name);
		}
		return true;
	}

		return false;
	}

bool ComponentManager::unloadComponent(const string &parent, const string &name)
{
	trace_enter();

	ComponentAdapterPtr t_ca = getComponentAdapter(parent, name);

	if (t_ca.isNULL()) return true;

	if (t_ca->unload()) {
		if (t_ca->isRef() == false) {
			removeComponentAdapter(name);
		}
		return true;
	}

		return false;
	}


bool ComponentManager::unloadComponentAll()
{
	trace_enter();

	OSGuard guard(&m_caListLock);

	ComponentAdapter *ca = NULL;
	ComponentAdapterList::iterator pos = m_caList.begin();
	for (; pos != m_caList.end(); ++pos) {
		ca = (*pos).get();
		if (ca != NULL)
		{
			ca->unload();
		}
	}
	m_caList.clear();

	return true;
}

bool ComponentManager::activate(const string &name)
{
	trace_enter_msg("name=" + name);

	ComponentAdapterPtr t_ca = getComponentAdapter(name);
	if (t_ca.isNULL()){
		log_error("ComponentAdapter is null : name=" << name);
		return false;
	}

	return t_ca->activate();
}

bool ComponentManager::activate(ComponentAdapter *ca)
{
	if (ca == NULL) {
		log_error("ComponentAdapter is null : ");
		return false;
	}

	return ca->activate();
}


bool ComponentManager::passivate(const string &name)
{
	trace_enter_msg("name=" + name);

	ComponentAdapterPtr t_ca = getComponentAdapter(name);
	if (t_ca.isNULL()){
		log_error("ComponentAdapter is null : name=" << name);
		return false;
	}

	return t_ca->passivate();
}

bool ComponentManager::passivate(ComponentAdapter *ca)
{
	if (ca == NULL) {
		log_error("ComponentAdapter is null : ");
		return false;
	}

	return ca->passivate();
}


bool ComponentManager::connectPort(ComponentAdapter *ca)
{
	if (ca == NULL) {
		log_error("ComponentAdapter is null : " );
		return false;
	}

	return ca->connectPort();
}


//
// PORT connection & disconnection
//
bool ComponentManager::connectPort(const std::string  &name)
{
	trace_enter_msg("name=" + name);

	ComponentAdapterPtr ca = getComponentAdapter(name);
	if (ca.isNULL()){
		log_error("ComponentAdapter is null : name=" << name);
		return false;
	}

	return ca->connectPort();
}

bool ComponentManager::disconnectPort(ComponentAdapter *ca)
{
	if (ca == NULL) {
		log_error("ComponentAdapter is null : ");
		return false;
	}

	return ca->disconnectPort();
}


bool ComponentManager::disconnectPort(const std::string  &name)
{
	trace_enter_msg("name=" + name);

	ComponentAdapterPtr ca = getComponentAdapter(name);
	if (ca.isNULL()){
		log_error("ComponentAdapter is null : name=" << name);
		return false;
	}

	return ca->disconnectPort();
}

bool ComponentManager::connectPortList(PortConnectionInfoList &infoList)
{
	trace_enter();
	return m_portConnector.connectPortList(m_caList, infoList);
}

bool ComponentManager::connectPort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.connectPort(m_caList, info);
}

bool ComponentManager::connectServicePort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.connectServicePort(m_caList, info);
}

bool ComponentManager::connectDataPort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.connectDataPort(m_caList, info);
}

bool ComponentManager::connectEventPort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.connectEventPort(m_caList, info);
}

bool ComponentManager::disconnectPortList(PortConnectionInfoList &list)
{
	trace_enter();
	return m_portConnector.disconnectPortList(m_caList, list);
}

bool ComponentManager::disconnectPort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.disconnectPort(m_caList, info);
}

bool ComponentManager::disconnectServicePort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.disconnectServicePort(m_caList, info);
}

bool ComponentManager::disconnectDataPort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.disconnectDataPort(m_caList, info);
}


bool ComponentManager::disconnectEventPort(PortConnectionInfo &info)
{
	trace_enter();
	return m_portConnector.disconnectEventPort(m_caList, info);
}
