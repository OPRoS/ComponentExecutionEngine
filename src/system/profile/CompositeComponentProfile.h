/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CompositeComponentProfile.h
 *
 */

#ifndef _COMPOSITE_COMPONENT_PROFILE_H_
#define _COMPOSITE_COMPONENT_PROFILE_H_

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

#define COMPOSITE_COMPONENT_PROFILE_TAG "composite_component_profile"

using namespace std;

/**
	컴포짓 컴포넌트 프로파일 정보를 xml 파일에서 읽어 
	저장하기 위한 클래스.
*/
class CompositeComponentProfile : public Profile
{
public :

	/** 컴포짓 컴포넌트 이름 */
	std::string  name;

	/** 컴포짓 컴포넌트에 대한 설명 정보 */
	std::string  description;

	/** 응용 가능한 도메인 정보 */
	AppDomainInfo appDomain;

	/** 컴포짓 컴포넌트에 대한 copyright 정보 */
	CopyrightInfo copyright;

	/** 컴포넌트 실행 환경 정보 */
	ExecutionEnvironmentInfo execEnv;

	/** 컴포짓 컴포넌트에 속한 서브 컴포넌트 리스트 */
	ComponentInfoList componentList;

	/** 컴포짓 컴포넌트가 외부에 노출하는 포트 리스트 */
	ExportPortInfoList exportPortList;

	/** 컴포짓 컴포넌트에 속한 서브 컴포넌트 간의 포트 연결 정보 */
	PortConnectionInfoList portConnectionList;

	CoordinationInfoList coordinationInfoList;

public:
	CompositeComponentProfile();
	virtual ~CompositeComponentProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return COMPOSITE_COMPONENT_PROFILE_TAG; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
