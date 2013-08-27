/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Package.h
 *
 */

#ifndef _PACKAGE_H_
#define _PACKAGE_H_

#include <string>
#include "system/SystemTypes.h"
#include "system/config/Config.h"
#include "system/profile/Profile.h"

/** 
	패키지의 상태. 
	- PKG_CREATED : 패키지 객체가 생성된 상태
	- PKG_LOADED  : 패키지가 로딩된 상태
	- PKG_CONNECTED : 패키지 내의 컴포넌트의 포트가 연결된 상태
	- PKG_ACTIVATED : 패키지 내의 컴포넌트가 실행된 상태
*/
typedef enum { PKG_INVALID, PKG_APP, PKG_ATOMIC, PKG_COMPOSITE } PackageType;

/** 
	패키지 타입.
	- PKG_INVALID : 알수없는 패키지 타입. 패키지 생성 시 디폴트 값.
	- PKG_APP : application 을 배포할 때 생성되는 패키지 타입
	- PKG_COMPOSITE : 컴포짓 컴포넌트를 배포할 때 생성되는 패키지 타입
	- PKG_ATOMIC : atomic component를 배포할 때 생성되는 패키지 타입
*/
typedef enum { PKG_CREATED, PKG_LOADED, PKG_CONNECTED, PKG_ACTIVATED } PackageStatus;

class PackageManager;

/**
	배포의 대상이 application profile, composite component profile, 
	(atomic) component profile의 로드/언로드, 컴포넌트 간의 포트 연결/해제,
	컴포넌트의 실행 시작/중지를 실행하는 클래스
*/ 
class Package
{
protected:
	/** PackageManager에 대한 포인터 */
	PackageManager *m_pkgMgr;

	/** 패키지의 이름 */
	std::string m_name;

	/** 
		패키지의 상태. 
		- PKG_CREATED : 패키지 객체가 생성된 상태
		- PKG_LOADED  : 패키지가 로딩된 상태
		- PKG_CONNECTED : 패키지 내의 컴포넌트의 포트가 연결된 상태
		- PKG_ACTIVATED : 패키지 내의 컴포넌트가 실행된 상태
	*/
	PackageStatus m_status;

	/** 
		패키지 타입.
		- PKG_INVALID : 알수없는 패키지 타입. 패키지 생성 시 디폴트 값.
		- PKG_APP : application 을 배포할 때 생성되는 패키지 타입
		- PKG_COMPOSITE : 컴포짓 컴포넌트를 배포할 때 생성되는 패키지 타입
		- PKG_ATOMIC : atomic component를 배포할 때 생성되는 패키지 타입
	*/
	PackageType m_type;

	/** 이 패키지에 해당되는 프로파일이 존재하는 디렉토리 path */
	std::string m_path;

	/** 이 패키지에 해당되는 프로파일의 파일 이름 */
	std::string m_reference;

	/** 해당 패키지에 속한 서브 컴포넌트들이 있는 디렉토리 path */
	std::string m_componentPath;

	/** 패키지 로딩 함수 */
	virtual bool doLoad() = 0;

	/** 패키지 언로딩 함수 */
	virtual bool doUnload() = 0;

	/** 패키지 내에 포함된 컴포넌트 간의 포트 연결 함수 */
	virtual bool doConnectPort() = 0;

	/** 패키지 내에 포함된 컴포넌트 간의 포트 연결 해제 함수 */
	virtual bool doDisconnectPort() = 0;

	/** 패키지 내에 포함된 컴포넌트 들을 실행시키는 함수 */
	virtual bool doActivate() = 0;

	/** 패키지 내에 포함된 컴포넌트 들의 실행을 중지시키는 함수 */
	virtual bool doPassivate() = 0;

public:
	Package();
	virtual ~Package();

	PackageManager *getPackageManager() { return m_pkgMgr; };
	void setPackageManager(PackageManager *mgr) { m_pkgMgr = mgr; };

	std::string  getName() { return m_name; };
	void setName(const std::string  &name) { m_name = name; };

	virtual PackageType getType() { return m_type; };
	virtual void setType(PackageType type) { m_type = type; };

	PackageStatus getStatus() { return m_status; };
	std::string statusToStr();

	std::string  getPath() { return m_path; };
	void setPath(const std::string  &path) { m_path = path; };

	std::string  getReference() { return m_reference; };
	void setReference(const std::string  &ref) { m_reference = ref; };

	std::string  getComponentPath() { return m_componentPath; };
	void setComponentPath(const std::string  &path) { m_componentPath = path; };

	/** 
		패키지 로딩 함수. 
		패키지 상태를 체크하여 가능한 경우에만 로딩 수행.

		@return bool 로딩 성공 여부
	*/
	virtual bool load();

	/** 
		패키지 언로딩 함수. 
		패키지 상태를 체크하여 가능한 경우에만 언로딩 수행.

		@return bool 언로딩 성공 여부
	*/
	virtual bool unload();

	/** 
		패키지 내에 포함된 컴포넌트 간의 포트 연결 함수. 
		패키지 상태를 체크하여 가능한 경우에만 포트 연결 수행.

		@return bool 성공 여부
	*/
	virtual bool connectPort();

	/** 
		패키지 내에 포함된 컴포넌트 간의 포트 연결 해제 함수. 
		패키지 상태를 체크하여 가능한 경우에만 포트 연결 해제 수행.

		@return bool 성공 여부
	*/
	virtual bool disconnectPort();

	/** 
		패키지 내에 포함된 컴포넌트 들을 실행시키는 함수. 
		패키지 상태를 체크하여 가능한 경우에만 수행.

		@return bool 성공 여부
	*/
	virtual bool activate();

	/** 
		패키지 내에 포함된 컴포넌트 들을 실행을 중지시키는 함수. 
		패키지 상태를 체크하여 가능한 경우에만 수행.

		@return bool 성공 여부
	*/
	virtual bool passivate();
};

#endif
