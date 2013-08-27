/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : AppDomainInfo.h
 *
 */

#ifndef _APP_DOMAIN_INFO_H_
#define _APP_DOMAIN_INFO_H_

#include <string>

#include "Profile.h"

/** 
	응용 도메인 정보를 나타내는 클래스.
*/
class AppDomainInfo : public ProfileInfo
{
public :
	/** 응용 도메인 이름 */
	std::string  domain;

	/** 응용이 가능한 로봇 타입 이름 */
	std::string  robotType;

public:
	AppDomainInfo();
	virtual ~AppDomainInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);

};

#endif /* _APP_DOMAIN_INFO_H_ */
