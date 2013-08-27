/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TelnetConsoleWorker.h
 *
 */

#ifndef _SERVICE_CONSOLE_WORKER_H_
#define _SERVICE_CONSOLE_WORKER_H_

#include <string>

#include "system/io/protocol/IoDirectWorker.h"

#include <modconfig/ModConfig.h>

class ServiceConsoleProvider;

#ifdef 	MOD_PYTHON
class ServiceConsoleWorkerProvider;
#endif
/**
	원격에서 커넥션이 연결되었을 때 이를 처리하는 클래스.
*/ 
class ServiceConsoleWorker : public IoDirectWorker
{	
#ifdef 	MOD_PYTHON
	ServiceConsoleWorkerProvider *m_provider;
#endif
public :
	ServiceConsoleWorker(IoConnection *cnn, ServiceConsoleProvider *mp);
	virtual ~ServiceConsoleWorker();

	/** 커넥션 연결 시 해당 커넥션의 요청을 처리하는 함수 */
	virtual void processWork() throw (IOException);
};

#endif
