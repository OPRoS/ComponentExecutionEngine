/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataInfo.h
 *
 */

#ifndef _DATA_PROFILE_H_
#define _DATA_PROFILE_H_

#include <string>
#include <iostream>

#include "Profile.h"
#include "DataStructInfo.h"

/**
	포트에서 사용되는 사용자 정의 데이터 타입에
	대한 정보를 저장하기 위한 클래스.
*/
class DataProfile : public Profile
{
public :
	/** 사용자 정의 데이터 타입 리스트 */
	DataStructInfoList structList;

public:
	DataProfile();
	virtual ~DataProfile();

	virtual std::string  getName() { return ""; };
	virtual std::string  getProfileTag() { return "data_profile"; };

	virtual bool doParse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _DATA_PROFILE_H_ */
