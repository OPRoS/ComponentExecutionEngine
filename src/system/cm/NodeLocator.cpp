/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : NodeLocator.cpp
 *
 */

#include "NodeLocator.h"

#include "system/util/BasicTypes.h"
#include "system/config/Config.h"
#include "system/Registry.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

NodeLocator::NodeLocator()
{
}

NodeLocator::~NodeLocator()
{
	OSGuard guard(&m_mutex);

	NodeIdMap::iterator it = m_idMap.begin();

	for (;it != m_idMap.end(); ++it)
	{
		NodeProtocolMap *p = it->second;

		if (p!=NULL){
			NodeProtocolMap::iterator n_it = p->begin();
			for (;n_it != p->end(); ++n_it)
			{
				delete n_it->second;
			}

			delete p;
		}		
	}
}

//-------------------------
// %addNode
//
// @desc 노드 추가 함수
//
// @param info 추가할 노드 정보
//
// @ret void
//
void NodeLocator::addNode(NodeInfo *info)
{
	trace_enter();

	if (info == NULL) return;

	OSGuard guard(&m_mutex);

	//
	addNodeToNodeIdMap(info);
}

//-------------------------
// %addNodeById
//
// @desc NodeIdMap에 노드를 추가 함수
//
// @param info 추가할 노드 정보
//
// @ret void
//
void NodeLocator::addNodeToNodeIdMap(NodeInfo *info)
{
	if (info == NULL) return;

	NodeProtocolMap *pmap;

	NodeIdMap::iterator pos = m_idMap.find(info->node_id);
	if (pos == m_idMap.end()) {
		pmap = new NodeProtocolMap();
		m_idMap.insert(make_pair(info->node_id, pmap));
	}
	else {
		pmap = pos->second;
	}

	pmap->insert(make_pair(info->io.protocol, info));
}

//-------------------------------------------------------------
// %getNodeById
//
// @desc 노드 id 및 protocol 정보를 이용하여 노드 정보를 탐색하는 함수
//
// @param nodeId 탐색할 노드 id
// @param protocol 탐색할 노드 내의 프로토콜
//
// @ret NodeInfo * 찾은 노드 정보. 없으면 NULL 리턴
//
NodeInfo *NodeLocator::getNodeById(const string &nodeId, const string &protocol)
{
	trace_enter_msg("nodeId=" << nodeId << ";protocol=" << protocol);

	OSGuard guard(&m_mutex);

	NodeIdMap::const_iterator pos = m_idMap.find(nodeId);
	if (pos == m_idMap.end()) return NULL;

	if (protocol == "") {
		// protocol == ""이면 아무거나(맨 처음 것) 리턴
		NodeProtocolMap::iterator iter = pos->second->begin();
		if (iter != pos->second->end()) return iter->second;
		else return NULL;
	}
	else {
		NodeProtocolMap::iterator iter = pos->second->find(protocol);
		if (iter != pos->second->end()) return iter->second;
		else return NULL;
	}

	return NULL;
}

//-------------------------
// %removeNode
//
// @desc 노드 삭제 함수
//
// @param nodeId 삭제할 노드 id
//
// @ret void
//
void NodeLocator::removeNode(const string &nodeId)
{
	trace_enter_msg("nodeId=" << nodeId);

	OSGuard guard(&m_mutex);

	NodeIdMap::iterator pos = m_idMap.find(nodeId);
	if (pos == m_idMap.end()) return;

	// delete all protocols of the node
	NodeProtocolMap::iterator iter = pos->second->begin();
	for (;iter != pos->second->end(); ++iter) {
		// delete NodeInfo
		delete iter->second;
	}

	// NodeProtocolMap 삭제
	delete pos->second;

	// NodeIdMap에서 해당 노드 삭제
	m_idMap.erase(pos);
}

//----------------------------------------------------
// %removeNodeById
//
// @desc 노드 id 및 protocol 정보를 이용하여 노드 정보 삭제
//
// @param nodeId 삭제할 노드 id
// @param protocol 삭제할 노드 내의 프로토콜
//
// @ret void
//
void NodeLocator::removeNodeById(const string &nodeId, const string &protocol)
{
	trace_enter_msg("nodeId=" << nodeId << ";protocol=" << protocol);

	OSGuard guard(&m_mutex);

	// NodeIdMap에서 NodeProtocolMap 찾음
	NodeIdMap::iterator pos = m_idMap.find(nodeId);
	if (pos != m_idMap.end()) {
		NodeProtocolMap::iterator iter = pos->second->find(protocol);
		if (iter != pos->second->end()) {
			// delete from NodeProtocolMap
			pos->second->erase(iter);
		}
	}
}

//----------------------------------------------------
// %deleteNodeFromNodeIdMap
//
// @desc 노드 주소 정보를 이용하여 NodeIdMap에서 노드 정보를 삭제하고
//        메모리에서 노드 정보를 삭제함
//
// @param nodeId 삭제할 노드 id
// @param protocol 삭제할 노드 내의 프로토콜
//
// @ret void
//
void NodeLocator::deleteNodeFromNodeIdMap(const string &nodeId, const string &protocol)
{
	trace_enter_msg("nodeId=" << nodeId << ";protocol=" << protocol);

	// NodeIdMap에서 NodeProtocolMap 찾음
	NodeIdMap::iterator pos = m_idMap.find(nodeId);
	if (pos != m_idMap.end()) {
		NodeProtocolMap::iterator iter = pos->second->find(protocol);
		if (iter != pos->second->end()) {
			// delete NodeInfo
			delete iter->second;

			// delete from NodeProtocolMap
			pos->second->erase(iter);
		}
	}
}


