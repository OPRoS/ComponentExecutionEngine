/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : NodeInfo.h
 *
 */

#ifndef _NODE_INFO_H_
#define _NODE_INFO_H_

#include <string>

#include "system/io/IoInfo.h"


/**
	노드에 대한 정보를 저장하고 있는 클래스.
*/
class NodeInfo
{
public:
	/** 노드 id */
	std::string  node_id;

	/** 해당 노드와 커넥션을 연결하기 위한 io 정보 */
	IoInfo io;

	/** 해당 노드와 연결되었는지 나타내는 플래그 */
	bool   isConnected;

	NodeInfo() : isConnected(false) {};
	virtual ~NodeInfo();

	/** Config (XML)로 부터 노드 정보를 세팅하기 위한 함수 */
	bool load(Config *cfg);

};

#endif
