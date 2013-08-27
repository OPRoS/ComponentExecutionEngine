/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Registry.h
 *
 */

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include <string>

#include "system/util/Properties.h"

#define ENGINE_PATH "engine.path"
#define DEPLOY_PATH "Repository"
#define CONFIG_PATH "Config"
#define LOG_PATH    "Log"

class Manager;
class ConfigLoaderFactory;
class Logger;
class LoggerFactory;
class ComponentEngineLoader;

/**
	������Ʈ ���� ������ �������� ������ �����ϱ� ���� Ŭ����. 
*/
class Registry
{
	/** local node id */
	std::string  m_nodeId;

	/** ������ ��Ʈ ���丮 */
	std::string  m_engineDir;

	/** ������ �κ� ������ ����� ���丮 */
	std::string  m_deployDir;

	/** �ý��� ���� ����(system.xml)�� ����� ���丮 */
	std::string  m_configDir;

	/** �ý��� ���� ���� ���� �̸� (system.xml) */
	std::string  m_sysConfigFileName;

	/** �ý��� ���� ���� ���� �н� : dir + filename */
	std::string  m_sysConfigFilePath; 

	/** �α� ������ ������ ���丮 */
	std::string  m_logDir;

	/** �ý��� ������Ƽ ���� */
	Properties m_sysProps;

	/** ConfigLoader ��ü�� ������ ���丮 ��ü */
	ConfigLoaderFactory *m_clf;

	/** Logger�� ������ ���丮 ��ü */
	LoggerFactory *m_lf;

	/** ���࿣������ ����� �ý��� �ΰ� */
	Logger *m_syslogger;

	/** ������Ʈ ���� ���� �δ� ��ü */
	ComponentEngineLoader *m_engineLoader;

	/** Registry ��ü */
	//static Registry *m_registry;
	static Registry m_registry;

	typedef std::map<std::string , Manager *> ManagerMap;

	/** ������Ʈ ���� �������� ������(Manager) ��ü�� �����ϱ� ���� �� */
	ManagerMap m_managers;

	/** 
		Registry ��ü�� �ý��ۿ� �ϳ��� �����ϴ� static ��ü�̹Ƿ�
		�����ڸ� public���� ���� ����
	*/
	Registry();

public:
	virtual ~Registry();

	/**
		Registry ��ü�� ��ȯ�ϴ� static �Լ�.
		Registry ��ü�� �ý��ۿ� �ϳ��� �����ϴ� static ��ü��

		@return Registry * �ý��ۿ� �����ϴ� Registry ��ü
	*/
	static Registry *getRegistry();

	/** 
		�ý��� �ΰ� ��ȯ �Լ�.
	*/
	Logger *getSystemLogger();

	/** 
		�ý��� �ΰ� ���� �Լ�.
	*/
	void setSystemLogger(Logger *l) { m_syslogger = l; };

	/** 
		ConfigLoaderFactory ��ȯ �Լ�.
	*/
	ConfigLoaderFactory *getConfigLoaderFactory() { return m_clf; };

	/** 
		ConfigLoaderFactory ���� �Լ�.
	*/
	void setConfigLoaderFactory(ConfigLoaderFactory *clf) { m_clf = clf; };

	/** 
		LoggerFactory ��ȯ �Լ�.
	*/
	LoggerFactory *getLoggerFactory() { return m_lf; };

	/** 
		LoggerFactory ���� �Լ�.
	*/
	void setLoggerFactory(LoggerFactory *lf) { m_lf = lf; };

	/** 
		������Ʈ ���� ���� �δ� ��ü ��ȯ �Լ�.
	*/
	ComponentEngineLoader *getComponentEngineLoader() { return m_engineLoader; };

	/** 
		������Ʈ ���� ���� �δ� ��ü ���� �Լ�.
	*/
	void setComponentEngineLoader(ComponentEngineLoader *l) { m_engineLoader = l; };

	/** 
		������Ʈ ���� ���� ���� ������(Manager) ��ü �˻� �Լ�.

		@param name �˻��� ������ �̸�

		@return Manager * �˻��� ������ ��ü
	*/
	Manager *getManager(const std::string  &name);

	/** 
		������Ʈ ���� ���� ���� ������(Manager) ��ü �߰� �Լ�.

		@param name �˻��� ������ �̸�
		@param Manager * �˻��� ������ ��ü
	*/
	void setManager(const std::string  &name, Manager *mgr);

	/** 
		������Ʈ ���� ���� ���� ������(Manager) ��ü ���� �Լ�.

		@param name ������ ������ �̸�
	*/
	void removeManager(const std::string  &name);

	/** 
		�ý��� ������Ƽ ��ȯ �Լ�.
	*/
	Properties *getSystemProperties();

	/** 
		�ý��� ������Ƽ ���� �Լ�.
	*/
	void setSystemProperties(Properties &props);

	const std::string& getDefaultApp() { return getSystemProperties()->getProperty("app.default", ""); };
	void setDefaultApp(const std::string& name) { return getSystemProperties()->setProperty("app.default", name); };

	/** 
		�ý��� ��� ���̵� ��ȯ �Լ�.
	*/
	std::string  getNodeId() { return m_nodeId; };

	/** 
		�ý��� ��� ���̵� ���� �Լ�.
	*/
	void setNodeId(const std::string &id) { m_nodeId = id; };

	/** 
		������ ��Ʈ ���丮 ��ȯ �Լ�.
	*/
	std::string  getEngineDir() { return m_engineDir; };

	/** 
		������ ��Ʈ ���丮 ���� �Լ� 
	*/
	void setEngineDir(const std::string &dir) { m_engineDir = dir; };

	/** 
		�κ� ����/������Ʈ ���� ���丮 ��ȯ �Լ�. 
	*/
	std::string  getDeployDir() { return m_deployDir; };

	/** 
		�κ� ����/������Ʈ ���� ���丮 ���� �Լ�. 
	*/
	void setDeployDir(const std::string &dir) { m_deployDir = dir; };

	/** 
		�ý��� ȯ�� ���� ���丮 ��ȯ �Լ�. 
	*/
	std::string  getConfigDir() { return m_configDir; };

	/** 
		�ý��� ȯ�� ���� ���丮 ���� �Լ�. 
	*/
	void setConfigDir(const std::string &dir) { m_configDir = dir; };

	/** 
		�ý��� ȯ�� ���� ���� �̸� ��ȯ �Լ�. 
	*/
	std::string  getSysConfigFileName() { return m_sysConfigFileName; };

	/** 
		�ý��� ȯ�� ���� ���� �̸� ���� �Լ�. 
	*/
	void setSysConfigFileName(const std::string &fn) { m_sysConfigFileName = fn; };

	/** 
		�ý��� ȯ�� ���� ���� ��ü �н�(���丮 + �����̸�) ��ȯ �Լ�. 
	*/
	std::string  getSysConfigFilePath() { return m_sysConfigFilePath; };

	/** 
		�ý��� ȯ�� ���� ���� ��ü �н�(���丮 + �����̸�) ���� �Լ�. 
	*/
	void setSysConfigFilePath(const std::string &path) { m_sysConfigFilePath = path; };

	/** 
		���ο� �ý��� ȯ�� ���� ���� Path (���丮 + �����̸�) ��ȯ �Լ�. 
	*/
	std::string  newConfigFilePath(const std::string  &filename) { return m_configDir + filename; };

	/** 
		���ο� �ý��� ���� ������ Path (���丮 + �����̸�) ��ȯ �Լ�. 
	*/
	std::string  newDeployFilePath(const std::string  &filename) { return m_deployDir + filename; };

	/** 
		�α� ������ ������ ���丮 �̸� ��ȯ �Լ�. 
	*/
	std::string  getLogDir() { return m_logDir; };

	void toDeployDir(const std::string &dir);
	void toLogDir(const std::string &dir);
	void toConfigDir(const std::string &dir); 
};

#endif 
