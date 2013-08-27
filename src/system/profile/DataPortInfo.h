/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataPortInfo.h
 *
 */

#ifndef _DATA_PORT_INFO_H_
#define _DATA_PORT_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

/** 
	데이터 포트 정보를 나타내는 클래스
*/
class DataPortInfo : public ProfileInfo
{
public :
	/** 데이터 포트 이름 */
	std::string  name;

	/** 데이터 포트 usage : input, output */
	std::string  usage;

	/** 데이터 포트가 전달하는 데이터의 타입 */
	std::string  dataType;

	/** 데이터 포트 큐잉 정책 : LAST, FISO, UFIFO */
	std::string  queueingPolicy;

	/** 데이터 포트 큐 사이즈 */
	int queueSize;

public:
	DataPortInfo();
	virtual ~DataPortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<DataPortInfo> DataPortInfoList;

#endif /* _DATA_PORT_PROFILE_H_ */
