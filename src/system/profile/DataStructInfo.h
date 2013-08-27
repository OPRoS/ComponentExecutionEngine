/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataStructInfo.h
 *
 */

#ifndef _DATA_STRUCT_INFO_H_
#define _DATA_STRUCT_INFO_H_

#include <list>
#include "Profile.h"
#include "DataStructItemInfo.h"

/** 
	사용자 정의 데이터 타입에 대한 정보를
	저장하기 위한 클래스.
*/
class DataStructInfo : public ProfileInfo
{
public :
	/** 데이터 타입에 속하는 각 데이터 항목 리스트 */
	DataStructItemInfoList itemList;

public:
	DataStructInfo();
	virtual ~DataStructInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<DataStructInfo> DataStructInfoList;

#endif /* _DATA_STRUCT_INFO_H_ */
