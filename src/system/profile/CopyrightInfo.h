/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CopyrightInfo.h
 *
 */

#ifndef _COPYRIGHT_INFO_H_
#define _COPYRIGHT_INFO_H_

#include <string>

#include "CompanyInfo.h"
#include "Profile.h"

/** 
	copyright 정보를 나타내는 클래스.
*/
class CopyrightInfo : public ProfileInfo
{
public :
	/** 해당 컴포넌트 혹은 로봇응용을 소유한 회사에 대한 정보 */
	CompanyInfo company;

	/** 해당 컴포넌트 혹은 로봇응용의 라이센스 정책 */
	std::string  licensePolicy;

	/** 해당 컴포넌트 혹은 로봇응용의 버전 정보 */
	std::string  version;

public:
	CopyrightInfo();
	virtual ~CopyrightInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _COPYRIGHT_INFO_H_ */
