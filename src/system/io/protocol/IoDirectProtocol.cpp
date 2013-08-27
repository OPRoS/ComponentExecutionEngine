/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoDirectProtocol.cpp
 *
 */

#include "IoDirectProtocol.h"

#include "IoTranceiver.h"
#include "IoWorker.h"

#include "system/Registry.h"
#include "system/io/IoConnection.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoDirectProtocol::IoDirectProtocol()
{
}

IoDirectProtocol::~IoDirectProtocol()
{
}

IoConnectionResult IoDirectProtocol::connectionAccept(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return IO_CONNECTION_FAIL;
	}

	// cnn을 담당할 worker 생성
	if (handleConnectionAccept(cnn)) {
		// 커넥션 저장
		IoDirectWorker *w = createDirectWorker(cnn);
		w->startWork();
		return IO_CONNECTION_DIRECT;
	}
	else {
		return IO_CONNECTION_FAIL;
	}
}

IoConnectionResult IoDirectProtocol::connectionConnect(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return IO_CONNECTION_FAIL;
	}

	// cnn을 담당할 worker 생성
	if (handleConnectionConnect(cnn)) {
		// 커넥션 저장
		IoDirectWorker *w = createDirectWorker(cnn);
		w->startWork();
		return IO_CONNECTION_DIRECT;
	}
	else {
		return IO_CONNECTION_FAIL;
	}
}

bool IoDirectProtocol::connectionClose(IoConnection *cnn)
{
	trace_enter();

	return true;
}

bool IoDirectProtocol::connectionRead(IoConnection *cnn)
{
	trace_enter();

	return true;
}

bool IoDirectProtocol::connectionWrite(IoConnection *cnn)
{
	trace_enter();

	return true;
}
