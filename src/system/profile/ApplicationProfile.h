/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ApplicationProfile.h
 *
 */

#ifndef _APPLICATION_PROFILE_H_
#define _APPLICATION_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "AppDomainInfo.h"
#include "CopyrightInfo.h"
#include "ExecutionEnvironmentInfo.h"
#include "ComponentInfo.h"
#include "ExportPortInfo.h"
#include "PortConnectionInfo.h"
#include "CoordinationInfo.h"

#define APP_PROFILE_TAG "package_profile"
#define APP_PROFILE_TAG_OLD "application_profile"

/**
	어플리케이션 프로파일 정보를 xml 파일에서 읽어 
	저장하기 위한 클래스.
*/
class ApplicationProfile : public Profile
{
public :

	/** 어플리케이션 이름 */
	std::string  name;

	/** ?? */
	std::string  reference;

	/** 어플리케이션에 대한 설명 정보 */
	std::string  description;

	/** 응용 도메인 정보 */
	AppDomainInfo appDomain;

	/** 응용에 대한 copyright 정보 */
	CopyrightInfo copyright;

	/** 컴포넌트 실행 환경 정보 */
	ExecutionEnvironmentInfo execEnv;

	/** 응용에 속한 서브 컴포넌트 리스트 */
	ComponentInfoList componentList;

	/** 응용에 속한 서브 컴포넌트 간의 포트 연결 정보 */
	PortConnectionInfoList portConnectionList;

	CoordinationInfoList coordinationInfoList;

public:
	ApplicationProfile();
	virtual ~ApplicationProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return APP_PROFILE_TAG; };

	virtual bool parse(Config *cfg);

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
