/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MethodProfile.h
 *
 */

#ifndef _METHOD_PROFILE_H_
#define _METHOD_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "MethodInfo.h"

/**
	서비스 포트에서 사용되는 메소드들에
	대한 정보를 저장하기 위한 클래스.
*/
class MethodProfile : public ProfileInfo
{
public:
	/**  서비스 포트 이름 */
	std::string  name;

	/** 서비스 포트에 속하는 메소드 리스트 */
	MethodInfoList methodList;

public:
	MethodProfile();
	virtual ~MethodProfile();

	virtual std::string  getName() { return name; };
	virtual std::string  getProfileTag() { return "method_profile"; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif 
