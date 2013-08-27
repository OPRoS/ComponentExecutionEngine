/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmInfo.h
 *
 */

#ifndef _FSM_INFO_H_
#define _FSM_INFO_H_

#include <string>
#include "Profile.h"

// FsmInfo
class FsmInfo : public ProfileInfo
{
public :
	std::string  name;
	std::string  type;
	std::string  reference;

public:
	FsmInfo();
	virtual ~FsmInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _FSM_INFO_H_ */
