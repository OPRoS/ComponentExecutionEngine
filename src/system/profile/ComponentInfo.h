/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentInfo.h
 *
 */

#ifndef _COMPONENT_INFO_H_
#define _COMPONENT_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

using namespace std;

/**
	어플리케이션 혹은 컴포짓 컴포넌트에 속하는
	서브 컴포넌트에 대한 정보를 나타내는 클래스.
*/
class ComponentInfo : public ProfileInfo
{
public:
	/** 서브 컴포넌트 이름 */
	std::string  name;

	/** 서브 컴포넌트 타입 : atomic or composite */
	std::string  type; 

	/** 서브 컴포넌트 xml 프로파일 정보 */
	std::string  reference;

	/** 
		해당 서브 컴포넌트가 존재하는 노드 정보로서, 
		어플리케이션 프로파일에서만 유효함.
		컴포짓 컴포넌트에 포함된 모든 서브 컴포넌트는
		컴포짓 컴포넌트와 동일한 노드에 배포됨. 
		ex) sensor_node, actuator_node, ...
	*/
	std::string  node_id;   

public:
	ComponentInfo();
	virtual ~ComponentInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ComponentInfo> ComponentInfoList;

#endif 
