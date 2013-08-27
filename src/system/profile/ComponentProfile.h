/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentProfile.h
 *
 */

#ifndef _COMPONENT_PROFILE_H_
#define _COMPONENT_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "AppDomainInfo.h"
#include "CopyrightInfo.h"
#include "ExecutionEnvironmentInfo.h"
#include "ExecutonSemanticsInfo.h"
#include "DataTypeInfo.h"
#include "FsmInfo.h"
#include "FsmProfile.h"
#include "ServicePortInfo.h"
#include "DataPortInfo.h"
#include "EventPortInfo.h"

#define COMPONENT_PROFILE_TAG "component_profile"

/**
	컴포넌트 프로파일 정보를 xml 파일에서 읽어 
	저장하기 위한 클래스.
*/
class ComponentProfile : public Profile
{
public :

	/** 컴포넌트 이름 */
	std::string  name;

	/** 컴포넌트에 대한 설명 정보 */
	std::string  description;

	/** 컴포넌트 응용 도메인 정보 */
	AppDomainInfo appDomain;

	/** 컴포넌트에 대한 copyright 정보 */
	CopyrightInfo copyright;

	/** 컴포넌트 실행 환경 정보 */
	ExecutionEnvironmentInfo execEnv;

	/** 컴포넌트 실행 시멘틱스 정보 */
	ExecutonSemanticsInfo execSemantics;

	/** 컴포넌트 프로퍼티 정보 */
	Properties props;

	/** 서비스 포트나 데이터 포트에서 사용하는 자료 구조 정의 */
	DataTypeInfoList dataTypeList;

	/** FSM 관련 정보 -> 통합 규격에서 삭제됨 */
	FsmInfo fsm;
	FsmProfile *fsmProfile;

	/** Required 서비스 포트 정보 */
	ServicePortInfoList requiredServicePortList;

	/** Provided 서비스 포트 정보 */
	ServicePortInfoList providedServicePortList;

	/** Input 데이터 포트 정보 */
	DataPortInfoList inputDataPortList;

	/** Output 데이터 포트 정보 */
	DataPortInfoList outputDataPortList;

	/** Input 이벤트 포트 정보 */
	EventPortInfoList inputEventPortList;

	/** Output 이벤트 포트 정보 */
	EventPortInfoList outputEventPortList;

public:
	ComponentProfile();
	virtual ~ComponentProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return COMPONENT_PROFILE_TAG; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
