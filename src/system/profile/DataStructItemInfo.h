/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataStructItemInfo.h
 *
 */

#ifndef _DATA_STRUCT_ITEM_INFO_H_
#define _DATA_STRUCT_ITEM_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

/** 
	데이터 타입에 속하는 각 데이터 항목에 대한
	정보를 저장하기 위한 클래스
*/
class DataStructItemInfo : public ProfileInfo
{
public:
	/** 데이터 타입 : int, char, long, float,... */
	std::string  type;

	/** 이름 */
	std::string  name;

	/** 디폴트 값 */
	std::string  defaultValue;

public:
	DataStructItemInfo();
	virtual ~DataStructItemInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<DataStructItemInfo> DataStructItemInfoList;

#endif 
