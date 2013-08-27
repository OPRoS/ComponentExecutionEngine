/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoManager.h
 *
 */

#include <string>

#include "IoManager.h"
#include "IoConnectionHandler.h"
#include "IoEventTarget.h"
#include "IoSelector.h"
// #include "IoSelectorWSAEvent.h"
#include "IoAcceptor.h"
#include "IoConnector.h"
#include "IoConnection.h"
#include "IoInfo.h"

#include "tcp/TcpTransport.h"

#include "system/Registry.h"
#include "system/config/Config.h"
#include "system/util/BasicTypes.h"
#include "system/util/Exceptions.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoManager::IoManager()
{
	trace_enter();
	setEventTargetManagerId("CommunicationManager");
}

IoManager::~IoManager()
{
	trace_enter();
	releaseEventProcessorAll();
	releaseTransportAll();
}

bool IoManager::onInit()
{
	trace_enter();

	Config *cfg = getConfig();
	if (cfg == NULL) return true;

	//--------------------
	// event processor
	//
	ConfigList list = cfg->getChildren("event_processor");
	ConfigList::iterator pos = list.begin();
	for(; pos != list.end(); ++pos) {
		registerEventProcessor(*pos);
	}

	//-----------------
	// io
	//
	list = cfg->getChildren("io");
	pos = list.begin();
	for(; pos != list.end(); ++pos) {
		registerIo(*pos);
	}

	return true;
}

bool IoManager::onStart()
{
	trace_enter();

	startIo();
	startEventProcessor();

	return true;
}

bool IoManager::onStop()
{
	trace_enter();

	stopIo();
	stopEventProcessor();

	return true;
}

bool IoManager::onDestroy()
{
	trace_enter();

	return true;
}

IoEventProcessor *IoManager::createEventProcessor(const string &type)
{
	trace_enter_msg("IoEventProcessor.type=" + type);

	if (type == "wsaevent") {
//		return new IoSelectorWSAEvent();
	}

	// type == "selector"
	return new IoSelector();
}

IoTransport *IoManager::createTransport(const string &type)
{
	trace_enter_msg("IoTransport.type=" + type);

	if (type == "tcp") {
		return new TcpTransport();
	}
	

	// type == "tcp"
	return NULL;
}

bool IoManager::addConnectionHandler(IoConnectionHandler *cnnHandler)
{
	trace_enter();

	if (cnnHandler == NULL) {
		log_error("IoConnectionHandler is NULL");
		return false;
	}

	trace_info("IoConnectionHandler.id=" << cnnHandler->getConnectionHandlerId());
	IoConnectionHandlerMap::iterator pos = m_dpMap.find(cnnHandler->getConnectionHandlerId());
	if (pos != m_dpMap.end()) {
		log_error("IoConnectionHandler is already exist : IoConnectionHandler.id=" << cnnHandler->getConnectionHandlerId());
		return false;
	}
	m_dpMap.insert(make_pair(cnnHandler->getConnectionHandlerId(), cnnHandler));

	return true;
}

IoConnectionHandler *IoManager::removeConnectionHandler(const string &id)
{
	trace_enter_msg("IoConnectionHandler.id=" + id);

	IoConnectionHandler *cnnHandler = NULL;

	IoConnectionHandlerMap::iterator pos = m_dpMap.find(id);
	if (pos != m_dpMap.end()) {
		cnnHandler = pos->second;
		m_dpMap.erase(pos);
	}
	else {
		log_info("IoConnectionHandler is not exist : IoConnectionHandler.id=" << id);
	}

	return cnnHandler;
}

IoConnectionHandler *IoManager::getConnectionHandler(const string &id)
{
	trace_enter_msg("IoConnectionHandler.id=" + id);

	IoConnectionHandler *cnnHandler = NULL;
	IoConnectionHandlerMap::iterator pos = m_dpMap.find(id);
	if (pos != m_dpMap.end()) {
		cnnHandler = pos->second;
	}
	else {
		log_info("IoConnectionHandler is not exist : IoConnectionHandler.id=" << id);
	}

	return cnnHandler;
}

bool IoManager::attachEventProcessor(IoEventProcessor *processor)
{
	trace_enter();

	if (processor == NULL) {
		log_error("IoEventProcessor is NULL");
		return false;
	}

	trace_info("IoEventProcessor.id=" << processor->getId());
	IoEventProcessorMap::iterator pos = m_epMap.find(processor->getId());
	if (pos != m_epMap.end()) {
		log_error("IoEventProcessor is already exist : IoEventProcessor.id=" << processor->getId());
		return false;
	}
	m_epMap.insert(make_pair(processor->getId(), processor));

	return true;
}

IoEventProcessor *IoManager::detachEventProcessor(const string &id)
{
	trace_enter_msg("IoEventProcessor.id=" + id);

	IoEventProcessor *processor = NULL;
	IoEventProcessorMap::iterator pos = m_epMap.find(id);
	if (pos != m_epMap.end()) {
		processor = pos->second;
		m_epMap.erase(pos);
	}
	else {
		log_info("IoEventProcessor is not exist : IoEventProcessor.id=" << id);
	}

	return processor;
}

IoEventProcessor *IoManager::getEventProcessor(const string &id)
{
	trace_enter_msg("IoEventProcessor.id=" + id);

	IoEventProcessor *processor = NULL;
	IoEventProcessorMap::iterator pos = m_epMap.find(id);
	if (pos != m_epMap.end()) {
		processor = pos->second;
	}
	else {
		log_info("IoEventProcessor is not exist : IoEventProcessor.id=" << id);
		return NULL;
	}

	return processor;
}

void IoManager::releaseEventProcessor(const string &id)
{
	trace_enter_msg("IoEventProcessor.id=" + id);

	IoEventProcessor *processor = detachEventProcessor(id);
	if (processor != NULL) delete processor;
}

void IoManager::releaseEventProcessorAll()
{
	trace_enter();

	IoEventProcessor *processor = NULL;
	IoEventProcessorMap::iterator pos = m_epMap.begin();
	for (; pos != m_epMap.end(); ++pos) {
		processor = pos->second;
		if (processor != NULL) delete processor;
	}
	m_epMap.clear();
}

bool IoManager::attachTransport(IoTransport *transport)
{
	trace_enter();

	if (transport == NULL) {
		log_error("IoTransport is NULL");
		return false;
	}

	trace_info("IoTransport.type=" << transport->getType());
	IoTransportMap::iterator pos = m_tpMap.find(transport->getType());
	if (pos != m_tpMap.end()) {
		log_error("IoTransport is already exist : IoTransport.type=" << transport->getType());
		return false;
	}
	m_tpMap.insert(make_pair(transport->getType(), transport));

	return true;
}

IoTransport *IoManager::detachTransport(const string &type)
{
	trace_enter_msg("IoTransport.type=" + type);

	IoTransport *transport = NULL;
	IoTransportMap::iterator pos = m_tpMap.find(type);
	if (pos != m_tpMap.end()) {
		transport = pos->second;
		m_tpMap.erase(pos);
	}
	else {
		log_info("IoTransport is not exist : IoTransport.type=" << type);
	}

	return transport;
}

IoTransport *IoManager::getTransport(const string &type)
{
	trace_enter_msg("IoTransport.type=" + type);

	IoTransport *transport = NULL;
	IoTransportMap::iterator pos = m_tpMap.find(type);
	if (pos != m_tpMap.end()) {
		transport = pos->second;
	}
	else {
		transport = createTransport(type);
		if (transport != NULL) {
			m_tpMap[type] = transport;
		}
		else {
			log_error("IoTransport is not exist : IoTransport.type=" << type);
		}
	}

	return transport;
}

void IoManager::releaseTransport(const string &type)
{
	trace_enter_msg("IoEventProcessor.type=" + type);

	IoTransport *transport = detachTransport(type);
	if (transport != NULL) delete transport;
}

void IoManager::releaseTransportAll()
{
	trace_enter();

	IoTransport *transport = NULL;
	IoTransportMap::iterator pos = m_tpMap.begin();
	for (; pos != m_tpMap.end(); ++pos) {
		transport = pos->second;
		if (transport != NULL) delete transport;
	}
	m_tpMap.clear();
}


//---------------------------------------------
// register & start & stop event processors
//
bool IoManager::registerEventProcessor(Config *cfg)
{
	trace_enter();

	try {
		bool enabled = cfg->getAttributeAsBool("enable", true);
		if (!enabled) return true;

		string id  = cfg->getAttribute("id");
		string type = cfg->getAttribute("type", "default");
		long timeout = cfg->getAttributeAsLong("timeout", 0);

		return registerEventProcessor(id, type, timeout);
	}
	catch (Exception &ex) {
		log_error("registerEventProcessor error : ex=" << ex);
	}
	return false;
}

bool IoManager::registerEventProcessor(const string &id, const string &type, long timeout)
{
	trace_enter();

	IoEventProcessor *processor = createEventProcessor(type);
	if (processor != NULL) {
		processor->setId(id);
		processor->setType(type);
		processor->setTimeout(timeout);
		if (!attachEventProcessor(processor))
		{
			delete processor;
			log_error("EventProcessor registration error");
			return false;

		}

		log_info("IoEventProcessor is registered : IoEventProcessor.id=" << id);
		return true;
	}
	else {
		log_error("IoEventProcessor is NULL : IoEventProcessor.id=" << id);
		return false;
	}
}

bool IoManager::startEventProcessor()
{
	trace_enter();

	IoEventProcessorMap::iterator pos = m_epMap.begin();
	for (; pos != m_epMap.end(); ++pos) {
		startEventProcessor(pos->second);
	}

	return true;
}

bool IoManager::startEventProcessor(const string &id)
{
	trace_enter();

	return startEventProcessor(getEventProcessor(id));
}

bool IoManager::startEventProcessor(IoEventProcessor *processor)
{
	trace_enter();

	if (processor == NULL) return false;

	processor->processEvent();
	return true;
}

bool IoManager::stopEventProcessor()
{
	trace_enter();

	IoEventProcessorMap::iterator pos = m_epMap.begin();
	for (; pos != m_epMap.end(); ++pos) {
		stopEventProcessor(pos->second);
	}

	return true;
}

bool IoManager::stopEventProcessor(const string &id)
{
	trace_enter();

	return stopEventProcessor(getEventProcessor(id));
}

bool IoManager::stopEventProcessor(IoEventProcessor *processor)
{
	trace_enter();

	if (processor == NULL) return true;

	processor->close();
	processor->waitUntilFinished();
	return true;
}


//---------------------------------------------
// register & start & stop transport
//

bool IoManager::registerIo(Config *cfg)
{
	trace_enter();

	IoInfo info;
	info.load(cfg);
	if (info.enabled == false) return true;

	return registerIo(info);
}

bool IoManager::registerIo(IoInfo &info)
{
	trace_enter();

	IoEventTarget *temp = getEventTarget(info.id);
	if (temp != NULL) {
		trace("Already registered : id=" << info.id);
		return true;
	}

	//----------------------------
	// io creation & register
	//
	IoTransport *transport = getTransport(info.protocol);
	if (transport == NULL) {
		log_error("Transport is NULL : IoTransport.protocol=" << info.protocol);
		return false;
	}

	IoEventTarget *target = NULL;
	if (info.role == "server") {
		IoAcceptor *acceptor = transport->getAcceptor();
		if (acceptor == NULL) {
			log_error("IoAcceptor is NULL : IoTransport.protocol=" << info.protocol);
			delete acceptor;
			return false;
		}
		target = acceptor;
	}
	else if (info.role == "client") {
		IoConnector *connector = transport->getConnector();
		if (connector == NULL) {
			log_error("IoConnector is NULL : IoTransport.protocol=" << info.protocol);
			return false;
		}

		connector->setProperties(info.props);
		if (connector->open() == false) {
			delete connector;
			log_error("IoConnector open FAIL : IoTransport.protocol=" << info.protocol);
			return false;
		}
		target = connector;
	}
	else {
		log_error("Invalid Io Role : io.role=" << info.role);
		return false;
	}

	if (info.id != "") {
		target->setId(info.id);
	}
	
	info.id = target->getId();
	target->setId(info.id);
	target->setProperties(info.props);
	target->getProperties()->setProperty("id", info.id);
	target->getProperties()->setProperty("protocol", info.protocol);
	target->getProperties()->setProperty("role", info.role);
	target->setEventHandler(this);

	if (target->open() == false || attachEventTarget(target) == false) {
		delete target;
		log_error("IoTarget open FAIL : IoTransport.protocol=" << target->getId());
		return false;
	}

	log_info("Io is registered : id=" << info.id << "; role=" << info.role << "; protocol=" << info.protocol);
	return true;
}

bool IoManager::startIo()
{
	trace_enter();

	IoEventTargetMap &map = getEventTargetMap();
	IoEventTargetMap::iterator pos = map.begin();
	for (; pos != map.end(); ++pos) {
		startIo(pos->second);
	}

	return true;
}

bool IoManager::startIo(const string &id)
{
	trace_enter();

	IoEventTarget *tg = getEventTarget(id);

	if ( tg==NULL)
	{
		log_error("start io fail=" << id);
		return false;
	}

	return startIo(tg);
}

bool IoManager::startIo(IoEventTarget *target)
{
	trace_enter();

	string handlerId = target->getProperties()->getProperty("cnn.handler", "");
	IoConnectionHandler *ch = getConnectionHandler(handlerId);
	if (ch == NULL) {
		log_error("IoConnectionHandler is NULL : IoConnectionHandler.id=" << handlerId);
		return false;
	}
	target->setConnectionHandler(ch);


	// 해당 IoEventTarget에 대한 이벤트 처리기 (IoEventProcessor)가 있으면 세팅
	// IoEventProcessor가 없으면 다른 객체에서 이벤트를 처리해야 함
	string processorId  = target->getProperties()->getProperty("evt.processor", "");
	IoEventProcessor *processor = NULL;
	if (processorId != "") {
		processor = getEventProcessor(processorId);
		if (processor == NULL) {
			log_error("IoEventProcessor is NULL : IoEventProcessor.id=" << processorId);
			return false;
		}
	}

	target->setEventProcessor(processor);

	// 만약 target이 IoConnector이면 바로 커넥션 수행
	// event_processor에 저장하여 추후에 커넥션하는 게 잘 안됨...
	if (target->getType() == EVT_TARGET_CONNECTOR) {
		IoConnector *connector = portable_dynamic_cast(IoConnector *, target);
		if (connector != NULL) {

			if (handleConnect(target)==false) return false;
		}
	}
	else if (processor != NULL) {
		// event_processor가 NULL이 아니면 세팅
		processor->registerEventTarget(target);
	}

	log_info("Io is about to start : id=" << target->getId());

	return true;
}


bool IoManager::stopIo()
{
	trace_enter();

	IoEventTargetMap map = getEventTargetMap();
	IoEventTargetMap::iterator pos = map.begin();
	for (; pos != map.end(); ++pos) {
		stopIo(pos->second);
	}

	return true;
}

bool IoManager::stopIo(const string &id)
{
	trace_enter();

	return stopIo(getEventTarget(id));
}

bool IoManager::stopIo(IoEventTarget *target)
{
	trace_enter();

	IoEventProcessor *processor = target->getEventProcessor();
	if (processor != NULL) {
		processor->unregisterEventTarget(target);
	}

	// TO DO
	// consider below???
	/*
	if (target->isConnection()) {
		IoConnection *cnn = portable_dynamic_cast(IoConnection *, target);
		if (cnn != NULL) {
			IoConnectionHandler *ch = cnn->getConnectionHandler();
			if (ch != NULL) ch->connectionClose(cnn);
		}
	}
	*/

	return true;
}


//-----------------------------------------------------
// Handler of events
//
bool IoManager::handleAccept(IoEventTarget *target)
{
	trace_enter_msg("***************** handleAccept");

	if (target == NULL) {
		log_error("Target is NULL");
		return true;
	}

	IoAcceptor *acceptor = portable_dynamic_cast(IoAcceptor *, target);
	if (acceptor == NULL) {
		log_error("Target is not IoAcceptor");
		return true;
	}

	IoConnection *cnn = acceptor->accept();
	if (cnn == NULL) {
		log_error("IoAcceptor.accept FAIL : IoAcceptor.id=" << acceptor->getId());
		return false;
	}

	log_info("Connection Accepted ======> cnn.id=" << cnn->getId());

	IoConnectionHandler *ch = acceptor->getConnectionHandler();
	if (ch == NULL) {
		log_error("IoConnectionHandler is NULL");
		cnn->close();
		delete cnn;
		return false;
	}

	IoConnectionResult r = ch->connectionAccept(cnn);
	if (r == IO_CONNECTION_OK) {
		IoEventProcessor *processor = acceptor->getEventProcessor();
		if (processor != NULL) {
			cnn->setEventHandler(this);
			cnn->setConnectionHandler(ch);
			cnn->setEventProcessor(processor);
			processor->registerEventTarget(cnn);
		}
	}
	else 
	if (r == IO_CONNECTION_DIRECT) {
		// do nothing
	}
	else {
		log_error("cnn=" << cnn->getId() << " is closed because of channel::connectionAccept() error");
		cnn->close();
		delete cnn;
		return false;
	}

	ch->connectionAcceptAfter(cnn);
	return 	true;
}

bool IoManager::handleConnect(IoEventTarget *target)
{
	trace_enter_msg("***************** handleConnect START");

	if (target == NULL) {
		log_error("Target is NULL");
		return true;
	}

	IoConnector *connector = portable_dynamic_cast(IoConnector *, target);
	if (connector == NULL) {
		log_error("Not IoConnector");
		target->close();
		return false;
	}

	IoConnection *cnn = connector->connect();
	if (cnn == NULL) {
		log_error("IoConnector.connect FAIL : IoConnector.id=" << connector->getId());
		target->close();
		return false;
	}

	log_info("Connection Connected ======> cnn.id=" << cnn->getId());

	IoConnectionHandler *ch = connector->getConnectionHandler();
	if (ch == NULL) {
		log_error("IoConnectionHandler is NULL");
		cnn->close();
		delete cnn;
		return false;
	}

	IoConnectionResult r = ch->connectionConnect(cnn);
	if (r == IO_CONNECTION_OK) {
		IoEventProcessor *processor = connector->getEventProcessor();
		if (processor != NULL) {
			cnn->setEventHandler(this);
			cnn->setConnectionHandler(ch);
			cnn->setEventProcessor(processor);
			processor->registerEventTarget(cnn);
		}
	}
	else {
		log_error("cnn=" << cnn->getId() << " is closed because of channel::connectionConnect() error");
		cnn->close();
		delete cnn;
		return false;
	}

	ch->connectionConnectAfter(cnn);
	return true;
}

bool IoManager::handleRead(IoEventTarget *target)
{
	trace_enter();

	if (target == NULL) {
		log_error("Target is NULL");
		return true;
	}

	IoConnection *cnn = portable_dynamic_cast(IoConnection *, target);
	if (cnn == NULL) {
		log_error("Not IoConnection");
		return true;
	}

	IoConnectionHandler *ch = cnn->getConnectionHandler();
	if (ch == NULL) {
		log_error("IoConnectionHandler is NULL : cnn.id=" << cnn->getId());
		return true;
	}

	return ch->connectionRead(cnn);
}

bool IoManager::handleWrite(IoEventTarget *target)
{
	trace_enter();

	if (target == NULL) {
		log_error("Target is NULL");
		return true;
	}

	IoConnection *cnn = portable_dynamic_cast(IoConnection *, target);
	if (cnn == NULL) {
		log_error("Not IoConnection");
		return true;
	}

	IoConnectionHandler *ch = cnn->getConnectionHandler();
	if (ch == NULL) {
		log_error("IoConnectionHandler is NULL : cnn.id=" << cnn->getId());
		return true;
	}


	return ch->connectionWrite(cnn);
}

bool IoManager::handleClose(IoEventTarget *target)
{
	trace_enter();

	if (target == NULL) {
		log_error("Target is NULL");
		return true;
	}

	trace("target.id=" << target->getId());
	if (target->isConnection()) {
		IoConnection *cnn = portable_dynamic_cast(IoConnection *, target);
		if (cnn == NULL) {
			log_error("IoEventTarget is not IoConnection");
			return true;
		}

			IoConnectionHandler *ch = cnn->getConnectionHandler();
			if (ch == NULL) {
				log_error("IoConnectionHandler is NULL : cnn.id=" << cnn->getId());
			}
			else {
				ch->connectionClose(cnn);
			}

		IoEventTarget *parent = cnn->getParent();
		if (parent != NULL && parent->isConnector()) {
			log_info("io.connector.close : id=" << parent->getId());

			// IoConnector close
			parent->close();
			removeEventTarget(parent->getId());
		}
//		removeEventTarget(target->getId());
	}
	else {
		removeEventTarget(target->getId());
	}

	return true;
}

bool IoManager::handleOther(IoEventTarget *target, IoEvent evt)
{
	trace_enter();

	if (target == NULL) {
		log_error("Target is NULL : evt=" << evt);
		return true;
	}

	return true;
}
