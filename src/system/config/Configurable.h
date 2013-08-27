/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Configurable.h
 *
 */

#ifndef _CONFIGURABLE_H_
#define _CONFIGURABLE_H_

#include "Config.h"
using namespace opros::engine;

/**
	Config를 저장하고 삭제하기 위한 클래스.
*/
class Configurable
{
protected :
	/** 저장하고 있는 Config 객체 */
	Config *m_cfg;

public:
	/** 생성자 */
	Configurable() : m_cfg(NULL) {};

	/** 
		소멸자.
		Config가 루트이면 삭제
	*/
	virtual ~Configurable() {
		if (m_cfg != NULL && m_cfg->getLevel() == 0) {
			delete m_cfg;
		}
	}

	/** Config 객체 세팅 */
	void setConfig(Config *cfg) { m_cfg = cfg; };

	/** Config 객체 반환 */
	Config *getConfig() { return m_cfg; };
};

#endif 

