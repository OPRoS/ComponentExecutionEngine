/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 6
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventTargetManager.h
 *
 */

#ifndef _IO_EVENT_TARGET_MANAGER_H_
#define _IO_EVENT_TARGET_MANAGER_H_

#include <string>
#include "IoEventTarget.h"

class IoEventTargetManager
{
	std::string m_evtTargetMgrId;
	IoEventTargetMap m_evtTargetMap;
public:
	IoEventTargetManager();
	virtual ~IoEventTargetManager();

	void setEventTargetManagerId(const std::string &id) { m_evtTargetMgrId = id; };
	std::string getEventTargetManagerId() { return m_evtTargetMgrId; };

	IoEventTargetMap &getEventTargetMap() { return m_evtTargetMap; };

	IoEventTarget *getEventTarget(const std::string &id);

	bool attachEventTarget(IoEventTarget *cnn);
	IoEventTarget *detachEventTarget(const std::string &id);

	// erase from the list and delete
	void removeEventTarget(const std::string &id);
	void removeEventTargetAll();
};

#endif /* _IO_EVENT_TARGET_MANAGER_H_ */
