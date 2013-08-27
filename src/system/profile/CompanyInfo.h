/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CompanyInfo.h
 *
 */

#ifndef _COMPANY_INFO_H_
#define _COMPANY_INFO_H_

#include <string>
#include "Profile.h"

/** 
	회사에 대한 정보를 나타내는 클래스.
*/
class CompanyInfo : public ProfileInfo
{
public :
	/** 회사 이름 */
	std::string  name;

	/** 회사 전화번호 */
	std::string  phone;

	/** 회사 팩스 번호 */
	std::string  fax;

	/** 회사 주소 */
	std::string  address;

	/** 회사 홈페이지 */
	std::string  homepage;

public:
	CompanyInfo();
	virtual ~CompanyInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _COMPANY_INFO_H_ */
