/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoInfo.h
 *
 */

#ifndef _IO_INFO_H_
#define _IO_INFO_H_

#include <string>
#include "system/config/Config.h"
#include "system/util/Properties.h"

using namespace opros::engine;

struct IoInfo
{
	std::string  id;
	std::string  protocol;
	std::string  role;
	Properties   props;

	bool enabled;

	IoInfo() : enabled(false) {};

	bool load(Config *cfg);
};

#endif
