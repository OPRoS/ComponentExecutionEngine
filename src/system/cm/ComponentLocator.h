/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentLocator.cpp
 *
 */

#ifndef _COMPONENT_LOCATOR_H
#define _COMPONENT_LOCATOR_H

#include <map>

#include "system/util/BasicTypes.h"

/**
	 컴포넌트가 존재하는 분산 노드의 주소를 저장하고 있는 클래스.
 */
class ComponentLocator
{
	/** 컴포넌트가 존재하는 분산 노드의 주소를 저장하고 있는 Map.  */
	StringMap m_compMap;

public :
	ComponentLocator();
	virtual ~ComponentLocator();

	/**
		컴포넌트 주소를 맵에 추가하는 함수.

		@param compId 컴포넌트 아이디
		@param nodeId 컴포넌트가 존재하는 노드 주소
	*/
	void addComponentLocation(const std::string &compId, const std::string &nodeId);

	/**
		컴포넌트가 존재하는 노드 주소를 반환하는 함수.

		@param compId 컴포넌트 아이디

		@return std::string 컴포넌트가 존재하는 노드의 주소
	*/
	std::string getComponentLocation(const std::string &compId);

	/**
		컴포넌트 주소를 맵에서 삭제하는 함수.

		@param compId 컴포넌트 아이디
	*/
	void removeComponentLocation(const std::string &compId);
};

#endif
