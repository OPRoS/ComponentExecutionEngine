/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStringTranceiver.h
 *
 */

#ifndef _IO_STRING_TRANSCEIVER_H_
#define _IO_STRING_TRANSCEIVER_H_

#include <map>
#include <string>

#include "system/io/protocol/IoStreamByte.h"
#include "system/util/DataObject.h"
#include "system/os/OSNotify.h"
#include "system/io/protocol/IoTranceiver.h"

#include "IoStringProtocol.h"

#define STR_RECV_BUF_SIZE 128

class IoStringTranceiver : public IoTranceiver
{
protected :

	typedef enum { STR_RECV_INIT, STR_RECV_BODY, STR_RECV_CHECK_EOL } IoReceiveState;

	IoStringProtocol *m_protocol;
	IoConnection *m_cnn;

	bool m_closed;
	OSNotify m_nf;

	// 보내야 할 데이터
	//std::string m_sendStr;
	byte *m_sendBuf;
	int   m_sendBufSize;
	int   m_writePos;

	// for receiving
	std::string m_recvStr;
	byte m_recvBuf[STR_RECV_BUF_SIZE];
	int m_recvBufSize;
	int m_readPos, m_readLen;
	IoReceiveState m_recvState;

	virtual void endWrite();
	virtual void errorWrite();
	virtual void processReceive();

public :

	IoStringTranceiver(IoStringProtocol *p, IoConnection *cnn);
	virtual ~IoStringTranceiver();

	virtual bool sendString(const std::string &str);
	
	//---------------------
	// IoTranceiver 함수
	virtual bool onSend();
	virtual bool onReceive();
	virtual void onClose();
};

#endif


