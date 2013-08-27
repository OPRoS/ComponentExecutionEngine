/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataTypeInfo.h
 *
 */

#ifndef _DATA_TYPE_INFO_H_
#define _DATA_TYPE_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	자료 구조를 정의하는 xml 파일에 대한 정보를 담고 있는 클래스. 
*/
class DataTypeInfo : public ProfileInfo
{
public :
	/** xml 파일 이름 */
	std::string  filename;

public:
	DataTypeInfo();
	virtual ~DataTypeInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level);
};

typedef std::list<DataTypeInfo> DataTypeInfoList;

#endif /* _DATA_TYPE_INFO_H_ */
