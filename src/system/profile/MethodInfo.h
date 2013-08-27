/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MethodInfo.h
 *
 */

#ifndef _METHOD_INFO_H_
#define _METHOD_INFO_H_

#include <string>
#include <list>
#include "Profile.h"
#include "ParamInfo.h"

/** 
	서비스 포트에 속하는 메소드에 대한
	정보를 저장하기 위한 클래스
*/
class MethodInfo : public ProfileInfo
{
public:
	/** 메소드 이름 */
	std::string  name;

	/** 리턴 타입 */
	std::string  returnType;

	/** 메소드 파라미터 리스트 */
	ParamInfoList paramList;

public:
	MethodInfo();
	virtual ~MethodInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level);
};

typedef std::list<MethodInfo> MethodInfoList;


#endif 
