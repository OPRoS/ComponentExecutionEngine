/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ServicePortInfo.h
 *
 */

#ifndef _SERVICE_PORT_INFO_H_
#define _SERVICE_PORT_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	서비스 포트 정보를 나타내는 클래스
*/
class ServicePortInfo : public ProfileInfo
{
public :
	/** 서비스 포트 이름 */
	std::string  name;

	/** 서비스 포트 타입 */
	std::string  type;

	/** 서비스 포트 usage : required, provided */
	std::string  usage;

	/** 서비스 포트가 제공하는 메소드들을 기술하는 xml 파일 이름 */
	std::string  reference;

public:
	ServicePortInfo();
	explicit ServicePortInfo(const ServicePortInfo &profile);
	virtual ~ServicePortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ServicePortInfo> ServicePortInfoList;

#endif /* _METHOD_PORT_INFO_H_ */
