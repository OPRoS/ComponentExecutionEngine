/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentRequester.cpp
 *
 */


#include "ComponentRequester.h"

#include "ComponentLocator.h"
#include "ComponentRequestWorker.h"

#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/io/IoManager.h"
#include "system/io/protocol/IoStreamByte.h"
#include "system/io/protocol/IoWorkerManager.h"
#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentRequester::ComponentRequester()
{
	setWorkerManager(this);
}

ComponentRequester::~ComponentRequester()
{
}

IoWorker *ComponentRequester::createWorker()
{
	return new ComponentRequestWorker(this);
}

bool ComponentRequester::handleConnectionAccept(IoConnection *cnn)
{
	// 원격 노드에서 현 노드로의 연결은 특별히 관리할 필요 없음. 
	// 원격 노드에서 요청이 오면 IoWorker에 해당 커넥션 id가 세팅되고
	// IoWorker에서 커넥션 id를 이용하여 요청을 보내므로
	// 별도의 정보는 필요 없음.

	return true;
}

bool ComponentRequester::handleConnectionConnect(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return false;
	}

	trace("cnn.id=" << cnn->getId());

	OSGuard guard(&m_mutex);

	IoEventTarget *parent = cnn->getParent();
	string nodeId;
	string protocol;

	if (parent != NULL) {
		nodeId = parent->getProperties()->getProperty("node.id", m_nodeLocator->getLocalNodeId());

		NodeInfo *info = m_nodeLocator->getNodeById(nodeId, protocol);
		if (info == NULL) {
			info = new NodeInfo();
			info->node_id = nodeId;
			info->io.id = cnn->getId();
			info->io.props.setProperty("node.id", nodeId);
			info->io.props.setProperty("node.protocol", protocol);

			m_nodeLocator->addNode(info);
		}
		else {
			info->node_id = nodeId;
			info->io.id = cnn->getId();
			info->io.props.setProperty("node.id", nodeId);
			info->io.props.setProperty("node.protocol", protocol);
		}
		cnn->getProperties()->setProperty("node.id", nodeId);
		cnn->getProperties()->setProperty("node.protocol", info->io.protocol);

		info->isConnected = true;

		return true;
	}
	else {
		log_error("There is no parent : cnn.id=" << cnn->getId());
		return false;
	}
}

void ComponentRequester::handleConnectionClose(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return;
	}

	trace("cnn.id=" << cnn->getId());

	OSGuard guard(&m_mutex);

	try {
		string nodeId = cnn->getProperties()->getProperty("node.id");
		string protocol = cnn->getProperties()->getProperty("node.protocol");

		NodeInfo *info = m_nodeLocator->getNodeById(nodeId, protocol);
		if (info == NULL) return;
		info->isConnected = false;
	}
	catch (Exception &ex) {
		trace_ex(ex);
	}

	log_debug("IoConnection is closed : cnn.id=" << cnn->getId());
}

//----------------------------------------------------
// %findConnection
//
// @desc 컴포넌트 이름과 프로토콜 정보를 이용하여 해당 컴포넌트가 있는 노드 주소 탐색
//       - 컴포넌트 이름을 이용하여 해당 컴포넌트가 존재하는 노드 id를 구할 수 있음
//
// @param compName 컴포넌트 이름
// @param protocol 삭제할 노드 내의 프로토콜
//
// @ret std::string 해당 컴포넌트가 존재하는 노드와 연결된 커넥션의 아이디
//
string ComponentRequester::findConnectionId(const string &compName, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",protocol=" << protocol);

	if (m_componentLocator == NULL) {
		log_error("ComponentManager is NULL");
		return "";
	}

	// finde nodeId in which the component exists
	string nodeId = m_componentLocator->getComponentLocation(compName);
	if (nodeId == "") {
		log_error("There is no node which has the component : comp.name=" << compName);
		return "";
	}

	// find node info in which the component exists
	// & protocol by which we connect to the node
	NodeInfo *info = NULL;
	OSGuard guard(&m_mutex);

	//m_mutex.lock();
	info = m_nodeLocator->getNodeById(nodeId, protocol);
	if (info == NULL) {
		log_error("Cannot connect to the node : node.id=" << nodeId << "; protocol=" << protocol);
	//	m_mutex.unlock();
		return "";
	}

	if (info->isConnected) {
	//	m_mutex.unlock();
		return info->io.id;
	}
//	m_mutex.unlock();

	

	// 해당 노드와 커넥션 연결이 되어 있지 않으면 
	// IoManager에 요청하여 연결을 시도함
//	m_mutex.lock();
	IoManager *iom = portable_dynamic_cast(IoManager *, Registry::getRegistry()->getManager(IO_MGNAGER));	
	info->io.props.setProperty("node.id", nodeId);
	info->io.props.setProperty("node.protocol", protocol);	
	
	
	if (iom->registerIo(info->io) == false) {
		
		return "";
	}
	
	if (iom->startIo(info->io.id) == false) {
//		m_mutex.unlock();
		
		iom->removeEventTarget(info->io.id);
		return "";
	}
//	m_mutex.unlock();
	
	// 연결 후 커넥션 id 반환
//	OSGuard guard(&m_mutex);
	return info->io.id;
}

IoResponse *ComponentRequester::methodCall(const string &compName, const string &portName, const string &methodName, \
										   byte *param, int paramSize, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",port.name=" << portName << ",method=" << methodName << ",protocol=" << protocol);

	//-----------------------------------------------
	// finde node info in which the componet exists
	// & protocol by which we connect to the node
	//
	string cnnId = findConnectionId(compName, protocol);
	if (cnnId == "") {
		log_error("IoConnection is NULL : compName=" << compName << "; protocol=" << protocol);
		return NULL;
	}

	IoStreamByte ios;
	try {
		ios << compName;
		ios << portName;
		int portType = CA_PORT_SERVICE;
		ios << portType;
		ios << methodName;
		ios << paramSize;
		ios.putBytes(param, 0, paramSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return NULL;
	}

	ios.flip();
	byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_request(cnnId, buf, size);
}

bool ComponentRequester::methodResponse(const string &cnnId, long reqId, bool retOK, const string &compName, \
										const string &portName, const string &methodName, byte *retValue, int retValueSize, const string &protocol)
{
	trace_enter_msg("cnn.id=" << cnnId << ",comp.name=" << compName << ",port.name=" << portName << ",method=" << methodName << ",protocol=" << protocol);	

	IoStreamByte ios;
	try {
/*		ios << compName;
		ios << portName;
		int portType = CA_PORT_SERVICE;
		ios << portType;
		ios << methodName; */
		ios.putBytes(retValue, retValueSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	ios.flip();
	byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_response(cnnId, reqId, retOK, buf, size);
}

bool ComponentRequester::dataCall(const string &compName, const string &portName, byte *data, int dataSize, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",port.name=" << portName << ",protocol=" << protocol);

	//-----------------------------------------------
	// finde node info in which the componet exists
	// & protocol by which we connect to the node
	//
	string cnnId = findConnectionId(compName, protocol);
	if (cnnId == "") {
		log_error("IoConnection is NULL : compName=" << compName << "; protocol=" << protocol);
		return false;
	}

	IoStreamByte ios;
	try {
		ios << compName;
		ios << portName;
		int portType = CA_PORT_DATA;
		ios << portType;
		ios << dataSize;
		ios.putBytes(data, dataSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	ios.flip();
	byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_msg(cnnId, buf, size);
}

bool ComponentRequester::eventCall(const string &compName, const string &portName, byte *evt, int evtSize, const string &protocol)
{
	trace_enter_msg("comp.name=" << compName << ",port.name=" << portName << ",protocol=" << protocol);

	//-----------------------------------------------
	// finde node info in which the componet exists
	// & protocol by which we connect to the node
	//
	string cnnId = findConnectionId(compName, protocol);
	if (cnnId == "") {
		log_error("IoConnection is NULL : compName=" << compName << "; protocol=" << protocol);
		return false;
	}

	IoStreamByte ios;
	try {
		ios << compName;
		ios << portName;
		int portType = CA_PORT_EVENT;
		ios << portType;
		ios << evtSize;
		ios.putBytes(evt, evtSize);
	}
	catch (Exception &ex) {
		log_error(ex);
		return false;
	}

	ios.flip();
	byte *buf = ios.getBuffer();
	int  size = ios.limit();
	ios.setBuffer(NULL, 0);

	return rpc_msg(cnnId, buf, size);
}
