/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : NodeLocator.h
 *
 */

#ifndef _NODE_LOCATOR_H_
#define _NODE_LOCATOR_H_

#include <string>
#include <map>

#include "NodeInfo.h"
#include "system/os/OSMutex.h"

/**
	 노드의 실제 주소를 관리하는 클래스.
*/
class NodeLocator
{
	/**
		프로토콜에 대한 노드 정보 저장을 위한 맵.
		(node_protcol, NodeInfo *) pairs
	*/
	typedef std::map<std::string, NodeInfo *> NodeProtocolMap;

	/** 
		하나의 노드에 존재하는 여러개의 프로토콜(tcp, udp, upnp, ...) 정보를 저장하는 맵.
		(node_id, NodeProtocolMap *) pairs
	*/
	typedef std::map<std::string, NodeProtocolMap *> NodeIdMap;

	/** local node id */
	std::string m_localNodeId;

	/** 노드 정보 저장을 위한 맵. (node_id, NodeInfo *) pairs. */
	NodeIdMap m_idMap;

	/** m_idMap 동기화용 mutex */
	OSMutex m_mutex;

protected :

	/**
		NodeIdMap에 노드를 추가하는 함수.

		@param info 추가할 노드 정보 객체
	*/
	void addNodeToNodeIdMap(NodeInfo *info);

	/**
		NodeIdMap에서 노드의 특정 프로토콜을 삭제하는 함수.
		해당 노드을 연결할 프로토콜이 여러개 존재하는 데 그 중
		하나를 삭제할 때 사용하는 함수.

		@param nodeId 노드 아이디
		@param protocol 삭제할 프로토콜 이름
	*/
	void deleteNodeFromNodeIdMap(const std::string &nodeId, const std::string &protocol);


public:
	NodeLocator();
	virtual ~NodeLocator();

	/**
		로컬 노드 아이디를 세팅하는 함수.

		@param id 로컬 노드 주소
	*/
	void setLocalNodeId(const std::string &id) { m_localNodeId = id; };

	/**
		로컬 노드 아이디를 반환하는 함수.

		@return std::string 로컬 노드 주소
	*/
	std::string getLocalNodeId() { return m_localNodeId; };

	/**
		노드를 추가하는 함수. 
		노드 아이디 및 프로토콜을 이용하여 노드 주소를 생성한 후,
		NodeIdMap 및 NodeAddrMap에 노드 정보를 추가함.

		@param info 추가할 노드 정보
	*/
	void addNode(NodeInfo *info);

	/**
		노드 아이디 및 프로토콜을 이용하여 노드 정보를 검색하는 함수.
		프로토콜이 공백 문자열("")이면 해당 노드의 첫번째로 등록된
		프로토콜에 해당하는 정보를 반환함.
		해당 노드는 여러개의 프로토콜로 연결이 가능하므로, 
		노드 아이디와 프로토콜이 주어져야 해당하는 노드 정보를 
		가져올 수 있음.

		@param nodeId 노드 아이디
		@param protocol 프로토콜 이름

		@return NodeInfo * 검색한 노드 정보
	*/
	NodeInfo *getNodeById(const std::string &nodeId, const std::string &protocol = "");

	/**
		노드를 삭제하는 함수. 
		노드에 할당된 모든 프로토콜을 삭제함.
		NodeIdMap 및 NodeAddrMap에서 노드 정보를 삭제함.

		@param nodeId 삭제할 노드 아이디
	*/
	void removeNode(const std::string &nodeId);

	/**
		노드 아이디 및 프로토콜에 해당하는 노드 정보를 삭제하는 함수.

		@param nodeId 노드 아이디
		@param protocol 프로토콜 이름
	*/
	void removeNodeById(const std::string &nodeId, const std::string &protocol);
};

#endif
