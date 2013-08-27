/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitorManager.h
 *
 */

#ifndef _MONITOR_MANAGER_H_
#define _MONITOR_MANAGER_H_

#include <string>

#include "system/Manager.h"

#include "MonitorProtocol.h"
#include "Monitor.h"

/**
	원격에서 컴포넌트 실행엔진에 전달된 명령을 처리하는 클래스.	
*/ 
class MonitorManager : public Manager
{
	typedef std::map<std::string, Monitor *> MonitorMap;

	/** 모니터를 저장하기 위한 맵 */
	MonitorMap m_monitorMap;

	/** 원격 모니터링 명령을 위한 통신 처리 모듈 */
	MonitorProtocol m_monitorProtocol;

public:
	MonitorManager();
	virtual ~MonitorManager();

	/** 
		모니터 초기화 함수.

		@return 성공여부
	*/
	virtual bool onInit();

	/** 
		모니터 실행 시작 함수.

		@return 성공여부
	*/
	virtual bool onStart();

	/** 
		모니터 실행 중지 함수.

		@return 성공여부
	*/
	virtual bool onStop();

	/** 
		모니터 삭제 함수.

		@return 성공여부
	*/
	virtual bool onDestroy();

	/** 
		모니터 객체를 검색하는 함수. 

		@param monitorName	검색하고자 하는 모니터 객체 이름

		@return Monitor*	검색한 모니터 객체. 
							존재하지 않을 경우 NULL 반환.
	*/
	Monitor *getMonitor(const std::string &monitorName);

	/** 
		모니터 객체를 추가하는 함수. 

		@param monitorName	추가하고자 하는 모니터 객체 이름
		@param Monitor*		추가하고자 하는 모니터 객체. 
	*/
	void setMonitor(const std::string &monitorName, Monitor *monitor);

	/** 
		모니터 객체를 삭제하는 함수.
		메모리에서도 삭제함.

		@param monitorName	삭제하고자 하는 모니터 객체 이름
	*/
	void removeMonitor(const std::string &monitorName);

	/** 
		모든 모니터 객체를 삭제하는 함수. 
	*/
	void clearMonitors();
};

#endif  
