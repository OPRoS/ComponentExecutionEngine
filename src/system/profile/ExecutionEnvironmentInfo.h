/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutionEnvironmentInfo.h
 *
 */

#ifndef _EXECUTION_ENVIRONMENT_INFO_H_
#define _EXECUTION_ENVIRONMENT_INFO_H_

#include <string>

#include "Profile.h"

/** 
	컴포넌트 혹은 로봇 응용의 실행 환경 정보를 나타내는 클래스.
*/
class ExecutionEnvironmentInfo : public ProfileInfo
{
public :
	/** 실행 가능한 운영체제 이름 */
	std::string  osName;

	/** 실행 가능한 운영체제 버전 */
	std::string  osVersion;

	/** 실행 가능한 cpu */
	std::string  cpu;

	/** 라이브러리 타입 : dll 혹은 so */
	std::string  libraryType;

	/** 라이브러리 이름 */
	std::string  libraryName;

	/** 구현한 프로그래밍 언어 */
	std::string  implLanguage;

	/** 컴파일러 이름 */
	std::string  compiler;

public:
	ExecutionEnvironmentInfo();
	virtual ~ExecutionEnvironmentInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _EXECUTION_ENVIRONMENT_INFO_H_ */
