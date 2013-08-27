/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : EventPortInfo.h
 *
 */

#ifndef _EVENT_PORT_INFO_H_
#define _EVENT_PORT_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

using namespace std;

/** 
	이벤트 포트 정보를 나타내는 클래스
*/
class EventPortInfo : public ProfileInfo
{
public :
	/** 이벤트 포트 이름 */
	std::string  name;

	/** 이벤트 포트 usage : input, output */
	std::string  usage;

	/** 이벤트 포트가 전달하는 데이터의 타입 */
	std::string  dataType;

	/** 이벤트 포트 큐잉 정책 : 사용되지 않음 */
	std::string  queueingPolicy;

	/** 이벤트 포트 큐 사이즈 : 사용되지 않음 */
	int queueSize;

public:
	EventPortInfo();
	virtual ~EventPortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<EventPortInfo> EventPortInfoList;

#endif /* _EVENT_PORT_PROFILE_H_ */
