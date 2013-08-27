/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitorProtocol.h
 *
 */

#ifndef _MONITOR_PROTOCOL_H_
#define _MONITOR_PROTOCOL_H_

#include <string>

#include "system/io/protocol/IoDirectProtocol.h"

#include "MonitorWorker.h"

class MonitorManager;

/**
	원격에서 컴포넌트 실행엔진에 전달된 명령을 처리하는 클래스.	
*/ 
class MonitorProtocol : public IoDirectProtocol
{
	MonitorManager *m_monitorMgr;

public:
	MonitorProtocol();
	virtual ~MonitorProtocol();

	void setMonitorManager(MonitorManager *mgr) { m_monitorMgr = mgr; };
	MonitorManager *getMonitorManager() { return m_monitorMgr; };

	/** 
		초기화 함수.

		@param cnnHandler 커넥션이 연결되었을 때 처리할 핸들러의 아이디

		@return 성공여부
	*/
	virtual bool open(const std::string &cnnHandler);


	/**
		원격에서 커넥션이 연결되었을 때 이를 처리할 worker를 생성하는 함수.

		@param cnn 원격에서 연결되 커넥션

		@return IoDirectWorker * 해당 커넥션을 처리할 worker
	*/
	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn);

	virtual bool processCommand(MonitorWorker *mw, const char *line, int size);
};

#endif  
