/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoProtocol.cpp
 *
 */

#include "IoProtocol.h"

#include "IoTranceiver.h"
#include "IoWorker.h"

#include "system/Registry.h"
#include "system/io/IoConnection.h"
#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoProtocol::IoProtocol()
{
	m_workerMgr = NULL;
}

IoProtocol::~IoProtocol()
{
}

IoConnectionResult IoProtocol::connectionAccept(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return IO_CONNECTION_FAIL;
	}

	if (handleConnectionAccept(cnn)) {
		// IoTranceiver 积己 
		IoTranceiver *t = createTransceiver(cnn);
		cnn->setUserObject("transceiver", t);

		// 目池记 历厘
		OSGuard guard(&m_mutex);
		m_cnnMap[cnn->getId()] = cnn;
		return IO_CONNECTION_OK;
	}
	else {
		return IO_CONNECTION_FAIL;
	}
}

IoConnectionResult IoProtocol::connectionConnect(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return IO_CONNECTION_FAIL;
	}

	// cnn狼 addr阑  促矫 技泼且 荐 乐澜
	if (handleConnectionConnect(cnn)) {
		// IoTranceiver 积己 
		IoTranceiver *t = createTransceiver(cnn);
		cnn->setUserObject("transceiver", t);

		// 目池记 历厘
		OSGuard guard(&m_mutex);
		m_cnnMap[cnn->getId()] = cnn;
		return IO_CONNECTION_OK;
	}
	else {
		return IO_CONNECTION_FAIL;
	}
}

bool IoProtocol::connectionClose(IoConnection *cnn)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("Connection is NULL");
		return true;
	}

	IoTranceiver *tx = (IoTranceiver *) cnn->getUserObject("transceiver");
	if (tx != NULL) tx->onClose();

	handleConnectionClose(cnn);

	// 目池记 昏力
	OSGuard guard(&m_mutex);
	m_cnnMap.erase(cnn->getId());

	return true;
}

bool IoProtocol::connectionRead(IoConnection *cnn)
{
	trace_enter();

	IoTranceiver *tx = NULL;
	tx = (IoTranceiver *) cnn->getUserObject("transceiver");

	if (tx == NULL) {
		log_error("IoMsgTranceiver is NULL");
		return true;
	}

	return tx->onReceive();
}

bool IoProtocol::connectionWrite(IoConnection *cnn)
{
	trace_enter();

	IoTranceiver *tx = NULL;
	tx = (IoTranceiver *) cnn->getUserObject("transceiver");
	if (tx == NULL) {
		log_error("IoMsgTranceiver is NULL");
		return true;
	}

	return tx->onSend();
}


IoConnection *IoProtocol::findConnection(const std::string &cnnId)
{
	// 目池记 昏力
	OSGuard guard(&m_mutex);
	IoConnectionMap::iterator pos = m_cnnMap.find(cnnId);
	if (pos != m_cnnMap.end()) {
		return pos->second;
	}

	return NULL;
}

