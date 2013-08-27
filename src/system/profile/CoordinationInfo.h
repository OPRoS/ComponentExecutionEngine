/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2009 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 8. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CoordinationInfo.h
 *
 */

#ifndef _COORDINATION_INFO_H_
#define _COORDINATION_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	두 컴포넌트 간의 포트 연결 정보를 나타내는 클래스.
*/
class CoordinationInfo : public ProfileInfo
{
public:

	/** Coordination 에서 소스 컴포넌트(뒷순서) 이름 */
	std::string  sourceComponentName;
 
	/** Coordination에서 타겟 컴포넌트(앞순서) 이름 */
	std::string  targetComponentName;

public:
	CoordinationInfo();
	virtual ~CoordinationInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<CoordinationInfo> CoordinationInfoList;

#endif 
