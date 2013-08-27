/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitorWorker.h
 *
 */

#ifndef _MONITOR_WORKER_H_
#define _MONITOR_WORKER_H_

#include <string>

#include "system/io/IoConnection.h"
#include "system/io/protocol/IoDirectWorker.h"

class MonitorProtocol;

/**
	원격에서 커넥션이 연결되었을 때 이를 처리하는 클래스.
*/ 
class MonitorWorker : public IoDirectWorker
{
	MonitorProtocol *m_mp;

public :
	MonitorWorker(MonitorProtocol *mp, IoConnection *cnn);
	virtual ~MonitorWorker();

	/** 커넥션 연결 시 해당 커넥션의 요청을 처리하는 함수 */
	virtual void processWork() throw (IOException);

	/** 
		요청이 성공 했음을 전달하는 함수. 

		@param reason 출력 메시지
	*/
	void returnOK(const std::string &target, const std::string &cmd, const std::string &ret) throw (IOException);
	void returnOK(const std::string &target, const std::string &cmd, const std::string &ret, const std::string &payload) throw (IOException);
	void returnOK(Properties &props, const std::string &ret) throw (IOException);
	void returnOK(Properties &props, const std::string &ret, const std::string &payload) throw (IOException);

	/** 
		요청이 실패 했음을 전달하는 함수. 

		@param reason 실패 이유를 나타내는 스트링
	*/
	void returnFail(const std::string &target, const std::string &cmd, const std::string &ret) throw (IOException);
	void returnFail(Properties &props, const std::string &ret) throw (IOException);

};

#endif
