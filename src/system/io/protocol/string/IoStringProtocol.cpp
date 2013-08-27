/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStringProtocol.cpp
 *
 */

#include "IoStringProtocol.h"

#include "IoStringTranceiver.h"
#include "IoStringWorker.h"

#include "system/Registry.h"
#include "system/io/IoConnection.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoStringProtocol::IoStringProtocol()
{
}

IoStringProtocol::~IoStringProtocol()
{
}

IoTranceiver *IoStringProtocol::createTransceiver(IoConnection *cnn)
{
	return new IoStringTranceiver(this, cnn);
}

//--------------------------------------------
// 메시지 전송 함수
//
// @param cnn 메시지를 전송할 IoConnection
// @param body 전송할 메시지 바디(unsigned char * = byte)
// @param bodySize 메시지 크기
//
// @ret 전송 성공 여부
//
bool IoStringProtocol::string_send(const string &cnnAddr, const string &str)
{
	trace_enter();

	IoConnection *cnn = findConnection(cnnAddr);
	if (cnn == NULL) {
		log_error("IoConnection is null");
		return false;
	}

	IoStringTranceiver *tx = NULL;
	tx = (IoStringTranceiver *) cnn->getUserObject("transceiver");
	if (tx == NULL) {
		log_error("IoMsgTranceiver is NULL : cnn.id=" << cnn->getId());
		return true;
	}

	if (tx->sendString(str) == false) {
		log_error("Sending ERROR");
		return false;
	}

	return true;
}

//
// Read Callback
//
bool IoStringProtocol::string_receive(IoConnection *cnn, const string &str)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return true;
	}

	if (m_workerMgr != NULL) {
		IoWorker *worker = m_workerMgr->getWorker();
		if (worker != NULL) {
			IoStringWorker *l_worker = dynamic_cast<IoStringWorker *>(worker);
			if (l_worker != NULL) {
				l_worker->setData(cnn, str);
				l_worker->startWork(cnn->getId());
			}
			else {
				log_error("Not IoRequestWorker");
			}
		}
		else {
			log_error("No IoWorker");
		}
	}
	else {
		log_error("No WorkerManger");
	}

	return true;
}

