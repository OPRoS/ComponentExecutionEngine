/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoRequestTranceiver.h
 *
 */

#ifndef IO_REQUEST_TRANSCEIVER_H
#define IO_REQUEST_TRANSCEIVER_H

#include <map>

#include "system/os/OSNotify.h"
#include "system/io/protocol/IoTranceiver.h"

#include "IoRequestProtocol.h"

typedef std::map<long, IoRequestPtr> IoRequestMap;

class IoRequestTranceiver : public IoTranceiver
{
protected :

	typedef enum { RECV_INIT, RECV_HEADER, RECV_BODY } IoReceiveState;

	IoRequestMap m_reqMap;

	bool m_closed;
	OSNotify m_nf;

	IoRequestProtocol *m_protocol;
	IoConnection *m_cnn;

	// for writing
	IoRequestPtr m_req;
	IoRequestPtr m_nullReqPtr;
	int m_writePos;

	// for receiving
	IoRequest m_res;
	IoReceiveState m_recvState;
	int m_readPos;

	virtual void endRequest();
	virtual void errorRequest();

	virtual void processReceive();

public :

	IoRequestTranceiver(IoRequestProtocol *p, IoConnection *cnn);
	virtual ~IoRequestTranceiver();

	virtual bool sendRequest(IoRequestPtr req);

	// IoTransceiver method
	virtual bool onSend();
	virtual bool onReceive();
	virtual void onClose();
};

#endif


