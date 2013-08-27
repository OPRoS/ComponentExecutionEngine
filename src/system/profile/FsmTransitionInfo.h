/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FsmTransitionInfo.h
 *
 */

#ifndef _FSM_TRANSITION_INFO_H_
#define _FSM_TRANSITION_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

class FsmTransitionInfo: public ProfileInfo
{
public:
	std::string  eventPortName;
	std::string  eventType;
	std::string  eventId;
	std::string  guardFuntionName;
	std::string  nextState;

public:
	FsmTransitionInfo();
	virtual ~FsmTransitionInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<FsmTransitionInfo *> FsmTransitionInfoList;

#endif /* _FSM_TRANSITION_INFO_H_ */
