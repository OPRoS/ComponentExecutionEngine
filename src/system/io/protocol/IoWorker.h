/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoWorker.h
 *
 */

#ifndef IO_WORKER_H
#define IO_WORKER_H

#include "system/os/OSNotify.h"
#include "system/os/OSThread.h"

class IoWorkerManager;

/** 
	원격 노드의 요청을 수신하고 이를 처리하기 위한 워커 클래스.
*/
class IoWorker : public OSThread
{
protected :
	IoWorkerManager *m_mgr;

	OSNotify m_nf;
	bool m_workStarted;
	bool m_closed;

	std::string m_cnnId;

public :

	IoWorker();
	virtual ~IoWorker();

	void setWorkerManager(IoWorkerManager *mgr) { m_mgr = mgr; };
	IoWorkerManager *getWorkerManager() { return m_mgr; };

	/** 
		요청이 온 IoConnection 아이디 세팅 함수. 
		요청에 대한 응답을 보낼 때 IoConnection 아이디를 사용하여
		요청이 온 노드로 응답을 보냄.

		@param cnnId 세팅할 커넥션 아이디
	*/
	void setCnnId(const std::string &cnnId) { m_cnnId = cnnId; };

	/** IoConnection 아이디 반환 함수. */
	std::string &getCnnId() { return m_cnnId; };

	/** 
		요청 처리를 시작함. 
		기다리고 있던 쓰레드를 깨움.
	*/
	virtual void startWork(const std::string &cnnId);

	/** 
		startWork()에서 깨어난 쓰레드가 실제 요청을 처리하기 위해
		호출하는 함수로 IoWorker를 상속한 클래스가 구현해야 함.
	*/
	virtual void processWork() = 0;

	/** 쓰레드 실행 시작 함수 */ 
	virtual void run();

	/** 
		워커의 사용이 종료되어 워커를 클로스하는 함수.
		작업의 시작을 기다리고 있는 쓰레드를 깨우고
		쓰레드를 종료하도록 함.
	*/
	virtual void close();
};

#endif


