/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 31
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Profile.cpp
 *
 */

#include "Profile.h"
#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

Profile::Profile()
{
	m_cfg = NULL;
}

Profile::~Profile()
{
	//if (m_cfg != NULL && m_cfg->getLevel() == 0) delete m_cfg;
}

bool Profile::parse(Config *cfg)
{
	if (cfg == NULL) {
		log_error("<Profile::parse> cfg = NULL");
		return false;
	}

	try {
		m_cfg = cfg->getChild(getProfileTag());
		if (m_cfg == NULL) {
			log_error("<Profile::parse> ProfileTag is NULL : tag=" << getProfileTag());
			return false;
		}

		return doParse(m_cfg);
	}
	catch (Exception &e) {
		log_error("<Profile::parse> Excepton Occurred <= " << e.getMessage());
	}
	return false;
}

