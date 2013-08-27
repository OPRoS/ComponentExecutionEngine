/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ParamInfo.h
 *
 */

#ifndef _PARAM_INFO_H_
#define _PARAM_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	메소드 파라미터 정보를 저장하기 위한 클래스
*/
class ParamInfo : public ProfileInfo
{
public:
	/** 파라미터 인덱스 : 사용되지 않음 */
	int index;

	/** 파라미터 타입 */
	std::string  type;

	/** 파라미터 이름 */
	std::string  name;

	/** 파라미터 디폴트 값 */
	std::string  defaultValue;

public:
	ParamInfo();
	virtual ~ParamInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ParamInfo> ParamInfoList;

#endif 
