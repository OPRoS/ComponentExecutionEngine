/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PackageDeployer.h
 *
 */

#ifndef _PACKAGE_DEPLOYER_H
#define _PACKAGE_DEPLOYER_H

#include <string>

#include "system/Manager.h"
#include "system/os/OSIncludes.h"
#include "system/io/IoConnectionHandler.h"

#include "PackageManager.h"
#include "system/console/TelnetConsole.h"

/**
	�κ� ���� �� ������Ʈ�� �����ϴ� Ŭ����.
*/ 
class PackageDeployer : public Manager
{
	/** ������ ��Ű���� �����ϴ� ��ü */
	PackageManager m_pkgMgr;

	/** ������ ������ ���������� �����ϱ� ���� ���� ���� ��ü */
	OSFileSystem m_fs;

	/** ���� ����� ó���ϱ� ���� ��ü */
	TelnetConsole m_requester;

	/** 
		��Ű���� �����ϴ� �Լ�.
		
		@param pkg ������ ��Ű�� ��ü
		
		@return bool ���� ����
	*/
	bool doDeployPackage(Package *pkg);

public:
	PackageDeployer();
	virtual ~PackageDeployer();

	virtual bool onInit();
	virtual bool onStart();
	virtual bool onStop();
	virtual bool onDestroy();

	/**
		������ ������ �����ϴ� ��Ʈ ���丮(deploy) �Ʒ���
		�����ϴ� ��� xml ���ϵ��� �̸��� ��ȯ�ϴ� �Լ�.

		@param std::vector<std::string> ���� ����Ʈ�� ��ȯ
	*/

	void getRepositoryFileList(std::vector<std::string> &list);

	PackageManager &getPackageManager() { return m_pkgMgr; };

	/**
		Ư�� xml ���������� �ε��ϰ� �����ϴ� �Լ�.
		������ �ش� ��Ű���� �ε�, ��Ʈ ����, ���࿡ �ش� ��.

		@param xmlFilePath ������ xml ���������� ���� �̸�

		@return bool ���� ��������
	*/
	bool deployPackage(const std::string  &xmlFilePath);

	/**
		��Ű�� ���� �Լ�.
		������ �ش� ��Ű���� �ε�, ��Ʈ ����, ���࿡ �ش� ��.

		@param pkg ������ ��Ű�� ��ü

		@return bool ���� ��������
	*/
	bool deployPackage(Package *pkg);

	/**
		������ ��Ű���� undeploy�ϴ� �Լ�.
		undeploy�� �ش� ��Ű���� ��ε�, ��Ʈ ���� ����, ���� ������ �ش� ��.

		@param pkgName undeploy�� ��Ű�� �̸�

		@return bool ��������
	*/
	bool undeployPackage(const std::string  &pkgName);

	/**
		������ ��� ��Ű���� undeploy�ϴ� �Լ�.

		@return bool ��������
	*/
	bool undeployPackageAll();

	/**
		Ư�� xml ���������� �ε��ϴ� �Լ�.
		PackageManager::loadPackage() �Լ� ȣ��.

		@param xmlFilePath �ε��� xml ���������� ���� �̸�

		@return bool ��������

		@see PackageManager::loadPackage() 
	*/
	bool loadPackage(const std::string  &xmlFilePath);

	/**
		��Ű�� ���� ������Ʈ�� ���� ��Ʈ�� �����ϴ� �Լ�.
		PackageManager::connectPackage() �Լ� ȣ��.

		@param pkgName ��Ʈ ������ ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::connectPackage() 
	*/
	bool connectPackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ���� ������ �����ϴ� �Լ�.
		PackageManager::activatePackage() �Լ� ȣ��.

		@param pkgName ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::activatePackage() 
	*/
	bool activatePackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ���� ������ �����ϴ� �Լ�.
		PackageManager::passivatePackage() �Լ� ȣ��.

		@param pkgName ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::passivatePackage() 
	*/
	bool passivatePackage(const std::string  &pkgName);

	/**
		��Ű�� ���� ������Ʈ�� ���� ��Ʈ�� ������ �����ϴ� �Լ�.
		PackageManager::disconnectPackage() �Լ� ȣ��.

		@param pkgName ��Ʈ ������ ������ ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::disconnectPackage() 
	*/
	bool disconnectPackage(const std::string  &pkgName);

	/**
		Ư�� xml ���������� ��ε��ϴ� �Լ�.
		PackageManager::unloadPackage() �Լ� ȣ��.

		@param pkgName ��ε��� ��Ű�� �̸�

		@return bool ��������

		@see PackageManager::unloadPackage() 
	*/
	bool unloadPackage(const std::string  &pkgName);

	virtual bool deleteApplicationFromRepository(const std::string &appName);
};

#endif 
