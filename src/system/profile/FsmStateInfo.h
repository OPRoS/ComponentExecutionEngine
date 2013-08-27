/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmStateInfo.h
 *
 */

#ifndef _FSM_STATE_INFO_H_
#define _FSM_STATE_INFO_H_

#include <string>
#include <list>

#include "Profile.h"
#include "FsmTransitionInfo.h"

// FsmStateInfo
class FsmStateInfo : public ProfileInfo
{
public:
	std::string  type;
	std::string  name;
	std::string  description;

	FsmTransitionInfoList transitionList;

public:
	FsmStateInfo();
	virtual ~FsmStateInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<FsmStateInfo *> FsmStateInfoList;

#endif /* _FSM_STATE_INFO_H_ */
