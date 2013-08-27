/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentController.h
 *
 */

#ifndef _COMPONENT_CONTROLLER_H_
#define _COMPONENT_CONTROLLER_H_

#include <string>
#include <vector>
#include "system/util/Properties.h"

#include "Controller.h"

class Registry;
class ComponentManager;

/**
	배포 관련 명령을 처리하는 클래스.	
*/ 
class ComponentController : public Controller
{

protected :

	Registry *m_registry;
	ComponentManager *m_cm;

public:
	ComponentController();
	virtual ~ComponentController();

	virtual bool init();

	// ---

	void compList(std::string &compName, std::vector<std::string> &list, std::vector<std::string> &compTypes);
	std::string compState(const std::string &compName);
	bool compInfo(const std::string &compName, Properties &status);
	bool compProps(const std::string &compName, Properties &status);
};

#endif  
