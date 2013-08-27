/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoPacketProtocol.cpp
 *
 */

#include "IoPacketProtocol.h"

#include "system/Registry.h"
#include "system/io/IoConnection.h"
#include "system/os/OSGuard.h"
#include "system/io/protocol/IoStreamByte.h"

#include "IoPacketTranceiver.h"
#include "IoPacketWorker.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoPacketProtocol::IoPacketProtocol()
{
}

IoPacketProtocol::~IoPacketProtocol()
{
}

IoTranceiver *IoPacketProtocol::createTransceiver(IoConnection *cnn)
{
	return new IoPacketTranceiver(this, cnn);
}

bool IoPacketProtocol::packet_send(const string &cnnAddr, byte *body, int bodySize)
{
	trace_enter();

	IoConnection *cnn = findConnection(cnnAddr);
	if (cnn == NULL) {
		log_error("IoConnection is null");
		return false;
	}

	IoPacketTranceiver *tx = NULL;
	tx = (IoPacketTranceiver *) cnn->getUserObject("transceiver");
	if (tx == NULL) {
		log_error("IoMsgTranceiver is NULL : cnn.id=" << cnn->getId());
		return true;
	}

	if (tx->sendPacket(body, bodySize) == false) {
		log_error("Sending ERROR");
		return false;
	}

	return true;
}

template <typename T> bool IoPacketProtocol::packet_send(const std::string &cnnAddr, T obj) 
{
	IoStreamByte m_ios;
	m_ios << obj;
	m_ios.flip();

	byte *buf = m_ios.getBuffer();
	int size = m_ios.limit();
	m_ios.setBuffer(NULL,0);

	return packet_send(cnnAddr, buf, size);
}

//
// Read Callback
//
bool IoPacketProtocol::packet_receive(IoConnection *cnn, byte *body, int bodySize)
{
	trace_enter();

	if (cnn == NULL) {
		log_error("IoConnection is NULL");
		return true;
	}

	if (m_workerMgr != NULL) {
		IoWorker *worker = m_workerMgr->getWorker();
		if (worker != NULL) {
			IoPacketWorker *l_worker = dynamic_cast<IoPacketWorker *>(worker);
			if (l_worker != NULL) {
				l_worker->setData(cnn, body, bodySize);
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

