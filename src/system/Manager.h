/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 4
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Manager.h
 *
 */

#ifndef _MANAGER_H
#define _MANAGER_H

#include "ManagedObject.h"

#define COMPONENT_ENGINE		"component_engine"
#define CONFIG_LOADER_FACTORY	"config_loader_factory"
#define LOGGER_FACTORY 			"logger_factory"
#define IO_MGNAGER 				"io_manager"
#define SERVICE_MGNAGER 		"service_manager"
#define COMPONENT_MANAGER 		"component_manager"
#define PACKAGE_DEPLOYER 		"package_deployer"

#define CONTROLLER_MANAGER 		"controller_manager"
#define MONITOR_MANAGER 		"monitor_manager"

#define MWADAPTOR_MANAGER 		"mwadaptor_manager"

/**
	������Ʈ ���� ���� ���� �����ϴ� ������(Manager) ��ü�� ���̽� Ŭ����.
	������Ʈ ���� ���� ���� �����ϴ� �ֿ� ������ ��ü
	- PackageDeployer
	- ComponentManager

	@see PackageDeployer
	@see ComponentManager
*/
class Manager : public ManagedObject
{
protected :
	/** ������ �̸� */
	std::string m_name;

public:
	Manager() {};
	virtual ~Manager() {};

	/**
		set the name of the manager
	*/
	void setName(const std::string &name) { m_name = name; };

	/**
		get the name of the manager
	*/
	std::string getName() { return m_name; };
};

#endif  
