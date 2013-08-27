/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ApplicationPackage.h
 *
 */

#ifndef _APPLICATION_PACKAGE_H_
#define _APPLICATION_PACKAGE_H_

#include <string>
#include <list>
#include <map>

#include "Package.h"

#include "system/profile/ComponentInfo.h"
#include "system/profile/PortConnectionInfo.h"
#include "system/profile/ApplicationProfile.h"

#include "system/SystemTypes.h"

#include "Component.h"
#include "DataPortLink.h"

class PackageManager;

/**
	배포의 대상이 application profile일 경우
	application profile을 로드/언로드,
	해당 application에 속한 컴포넌트 간의 포트 연결/해제,
	해당 application에 속한 컴포넌트의 실행 시작/중지를 
	실행하는 클래스
*/ 
class ApplicationPackage : public Package
{
	typedef std::list<Package *> PackageList;


protected:
	/** ApplicationPackage 객체에 대응되는 ApplicationProfile 객체 */
	ApplicationProfile *m_profile;

	/** 이 응용에 속한 하위 컴포넌트들에 대한 정보를 담고 있는 Package들의 리스트 */ 
	PackageList m_pkgList;


	Package *findChildPackage(const std::string &name);

	/** 패키지 로딩 함수 */
	virtual bool doLoad();

	/** 패키지 언로딩 함수 */
	virtual bool doUnload();

	/** 패키지 내에 포함된 컴포넌트 간의 포트 연결 함수 */
	virtual bool doConnectPort();

	/** 패키지 내에 포함된 컴포넌트 간의 포트 연결 해제 함수 */
	virtual bool doDisconnectPort();

	/** 패키지 내에 포함된 컴포넌트 들을 실행시키는 함수 */
	virtual bool doActivate();

	/** 패키지 내에 포함된 컴포넌트 들의 실행을 중지시키는 함수 */
	virtual bool doPassivate();

	/** 패키지 내에 포함된 루트 컴포넌트 간의 포트 연결 함수 */
	bool connectPortThis();

	/** 패키지 내에 포함된 루트 컴포넌트 간의 포트 연결 해제 함수 */
	bool disconnectPortThis();

	/** 
		프로파일을 분석하여 패키지의 타입이 PKG_APP, PKG_ATOMIC, PKG_COMPOSITE 인지
		체크하는 함수.
		
		@param path 프로파일이 있는 디렉토리 명
		@param reference 프로파일의 파일 이름
		
		@return PackageType 패키지 타입
	*/
	PackageType checkPackageType(const std::string &path, const std::string &reference);

public:
	ApplicationPackage();
	virtual ~ApplicationPackage();

	ApplicationProfile *getApplicationProfile() { return m_profile; };
	void setApplicationProfile(ApplicationProfile *profile) { m_profile = profile; };

	/** 
		인수로 전달되는 컴포넌트 리스트 내에 포함된 컴포넌트들을 로드하는 함수.

		@param list 컴포넌트 리스트

		@return 성공 여부
	*/
	bool loadComponentList(ComponentInfoList &list);
};

#endif 
