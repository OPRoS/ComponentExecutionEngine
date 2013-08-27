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
	컴포넌트 실행 엔진의 여러가지 정보를 저장하기 위한 클래스. 
*/
class Registry
{
	/** local node id */
	std::string  m_nodeId;

	/** 엔진의 루트 디렉토리 */
	std::string  m_engineDir;

	/** 배포할 로봇 응용이 저장된 디렉토리 */
	std::string  m_deployDir;

	/** 시스템 설정 정보(system.xml)가 저장된 디렉토리 */
	std::string  m_configDir;

	/** 시스템 설정 정보 파일 이름 (system.xml) */
	std::string  m_sysConfigFileName;

	/** 시스템 설정 정보 파일 패스 : dir + filename */
	std::string  m_sysConfigFilePath; 

	/** 로그 정보를 저장할 디렉토리 */
	std::string  m_logDir;

	/** 시스템 프로퍼티 정보 */
	Properties m_sysProps;

	/** ConfigLoader 객체를 생성할 팩토리 객체 */
	ConfigLoaderFactory *m_clf;

	/** Logger를 생성할 팩토리 객체 */
	LoggerFactory *m_lf;

	/** 실행엔진에서 사용할 시스템 로거 */
	Logger *m_syslogger;

	/** 컴포넌트 실행 엔진 로더 객체 */
	ComponentEngineLoader *m_engineLoader;

	/** Registry 객체 */
	//static Registry *m_registry;
	static Registry m_registry;

	typedef std::map<std::string , Manager *> ManagerMap;

	/** 컴포넌트 실행 엔진내의 관리자(Manager) 객체를 보관하기 위한 맵 */
	ManagerMap m_managers;

	/** 
		Registry 객체는 시스템에 하나만 존재하는 static 객체이므로
		생성자를 public으로 하지 않음
	*/
	Registry();

public:
	virtual ~Registry();

	/**
		Registry 객체를 반환하는 static 함수.
		Registry 객체는 시스템에 하나만 존재하는 static 객체임

		@return Registry * 시스템에 존재하는 Registry 객체
	*/
	static Registry *getRegistry();

	/** 
		시스템 로거 반환 함수.
	*/
	Logger *getSystemLogger();

	/** 
		시스템 로거 세팅 함수.
	*/
	void setSystemLogger(Logger *l) { m_syslogger = l; };

	/** 
		ConfigLoaderFactory 반환 함수.
	*/
	ConfigLoaderFactory *getConfigLoaderFactory() { return m_clf; };

	/** 
		ConfigLoaderFactory 세팅 함수.
	*/
	void setConfigLoaderFactory(ConfigLoaderFactory *clf) { m_clf = clf; };

	/** 
		LoggerFactory 반환 함수.
	*/
	LoggerFactory *getLoggerFactory() { return m_lf; };

	/** 
		LoggerFactory 세팅 함수.
	*/
	void setLoggerFactory(LoggerFactory *lf) { m_lf = lf; };

	/** 
		컴포넌트 실행 엔진 로더 객체 반환 함수.
	*/
	ComponentEngineLoader *getComponentEngineLoader() { return m_engineLoader; };

	/** 
		컴포넌트 실행 엔진 로더 객체 세팅 함수.
	*/
	void setComponentEngineLoader(ComponentEngineLoader *l) { m_engineLoader = l; };

	/** 
		컴포넌트 실행 엔진 내의 관리자(Manager) 객체 검색 함수.

		@param name 검색할 관리자 이름

		@return Manager * 검색한 관리자 객체
	*/
	Manager *getManager(const std::string  &name);

	/** 
		컴포넌트 실행 엔진 내의 관리자(Manager) 객체 추가 함수.

		@param name 검색할 관리자 이름
		@param Manager * 검색한 관리자 객체
	*/
	void setManager(const std::string  &name, Manager *mgr);

	/** 
		컴포넌트 실행 엔진 내의 관리자(Manager) 객체 삭제 함수.

		@param name 삭제할 관리자 이름
	*/
	void removeManager(const std::string  &name);

	/** 
		시스템 프로퍼티 반환 함수.
	*/
	Properties *getSystemProperties();

	/** 
		시스템 프로퍼티 세팅 함수.
	*/
	void setSystemProperties(Properties &props);

	const std::string& getDefaultApp() { return getSystemProperties()->getProperty("app.default", ""); };
	void setDefaultApp(const std::string& name) { return getSystemProperties()->setProperty("app.default", name); };

	/** 
		시스템 노드 아이디 반환 함수.
	*/
	std::string  getNodeId() { return m_nodeId; };

	/** 
		시스템 노드 아이디 세팅 함수.
	*/
	void setNodeId(const std::string &id) { m_nodeId = id; };

	/** 
		엔진의 루트 디렉토리 반환 함수.
	*/
	std::string  getEngineDir() { return m_engineDir; };

	/** 
		엔진의 루트 디렉토리 세팅 함수 
	*/
	void setEngineDir(const std::string &dir) { m_engineDir = dir; };

	/** 
		로봇 응용/컴포넌트 배포 디렉토리 반환 함수. 
	*/
	std::string  getDeployDir() { return m_deployDir; };

	/** 
		로봇 응용/컴포넌트 배포 디렉토리 세팅 함수. 
	*/
	void setDeployDir(const std::string &dir) { m_deployDir = dir; };

	/** 
		시스템 환경 설정 디렉토리 반환 함수. 
	*/
	std::string  getConfigDir() { return m_configDir; };

	/** 
		시스템 환경 설정 디렉토리 세팅 함수. 
	*/
	void setConfigDir(const std::string &dir) { m_configDir = dir; };

	/** 
		시스템 환경 설정 파일 이름 반환 함수. 
	*/
	std::string  getSysConfigFileName() { return m_sysConfigFileName; };

	/** 
		시스템 환경 설정 파일 이름 세팅 함수. 
	*/
	void setSysConfigFileName(const std::string &fn) { m_sysConfigFileName = fn; };

	/** 
		시스템 환경 설정 파일 전체 패스(디렉토리 + 파일이름) 반환 함수. 
	*/
	std::string  getSysConfigFilePath() { return m_sysConfigFilePath; };

	/** 
		시스템 환경 설정 파일 전체 패스(디렉토리 + 파일이름) 세팅 함수. 
	*/
	void setSysConfigFilePath(const std::string &path) { m_sysConfigFilePath = path; };

	/** 
		새로운 시스템 환경 설정 파일 Path (디렉토리 + 파일이름) 반환 함수. 
	*/
	std::string  newConfigFilePath(const std::string  &filename) { return m_configDir + filename; };

	/** 
		새로운 시스템 배포 파일의 Path (디렉토리 + 파일이름) 반환 함수. 
	*/
	std::string  newDeployFilePath(const std::string  &filename) { return m_deployDir + filename; };

	/** 
		로그 파일을 저장할 디렉토리 이름 반환 함수. 
	*/
	std::string  getLogDir() { return m_logDir; };

	void toDeployDir(const std::string &dir);
	void toLogDir(const std::string &dir);
	void toConfigDir(const std::string &dir); 
};

#endif 
