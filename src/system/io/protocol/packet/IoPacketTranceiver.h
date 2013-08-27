/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoPacketTranceiver.h
 *
 */

#ifndef IO_PACKET_TRANSCEIVER_H
#define IO_PACKET_TRANSCEIVER_H

#include <map>

#include "system/io/protocol/IoStreamByte.h"
#include "system/util/DataObject.h"
#include "system/os/OSNotify.h"
#include "system/io/protocol/IoTranceiver.h"

#include "IoPacketProtocol.h"

#define IO_MSG_HEADER_SIZE 4
#define IO_MSG_MAGIC   ((byte) 0xFC)
#define IO_MSG_VERSION 0x01

class IoPacketTranceiver : public IoTranceiver
{
protected :

	typedef enum { MSG_RECV_INIT, MSG_RECV_MAGIC, MSG_RECV_HEADER, MSG_RECV_BODY } IoReceiveState;

	IoPacketProtocol *m_protocol;
	IoConnection *m_cnn;

	bool m_closed;
	OSNotify m_nf;

	// 보내야 할 데이터
	byte *m_sendBuf;
	int   m_sendBufSize;
	int   m_writePos;

	// for receiving
	byte *m_recvBuf;
	int   m_recvBufSize;
	IoReceiveState m_recvState;
	int m_readPos;

	// 메시지 헤더
	IoStreamByte m_header;

	virtual void endWrite();
	virtual void errorWrite();
	virtual void processReceive();

public :

	IoPacketTranceiver(IoPacketProtocol *p, IoConnection *cnn);
	virtual ~IoPacketTranceiver();

	virtual bool sendPacket(byte *buf, int bufSize);

	//---------------------
	// IoTranceiver 함수
	virtual bool onSend();
	virtual bool onReceive();
	virtual void onClose();
};

#endif


