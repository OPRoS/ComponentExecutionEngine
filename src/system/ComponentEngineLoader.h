/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentEngineLoader.h
 *
 */

#ifndef _COMPONENT_ENGINE_LOADER_H_
#define _COMPONENT_ENGINE_LOADER_H_

#include "system/util/Properties.h"

/**
	컴포넌트 실행엔진 객체를 생성하고 실행을 시작시키는 클래스. 
 */
class ComponentEngineLoader
{
	/** 디폴트 응용 이름 */
	std::string m_appName;

public:
	ComponentEngineLoader();
	~ComponentEngineLoader();

	/** 
		컴포넌트 실행 엔진을 로딩하는 함수 

		@param argc 커맨드 라인 아규먼트 갯수
		@param argv 커맨들 라인 아규먼트

		@return bool 성공 여부
	*/
	bool load(int argc, char *argv[]);

	/** 
		컴포넌트 실행 엔진을 로딩하는 함수 

		@param props 커맨드 라인 아규먼트를 name/value 형태로 저장한 객체

		@return bool 성공 여부
	*/
	bool load(Properties &props);

	/** 
		커맨드 라인 아규먼트를 name/value 형태의 Properties 객체로 변경하는 함수.

		@param props  커맨드 라인 아규먼트의 name/value를 저장하기 위한 Properties 객체
		@param argc 커맨드 라인 아규먼트 갯수
		@param argv 커맨들 라인 아규먼트
	*/
	void toProperties(Properties &props, int argc, char *argv[]);
};

#endif  
