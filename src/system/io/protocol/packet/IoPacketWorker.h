/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoPacketWorker.h
 *
 */

#ifndef IO_PACKET_WORKER_H
#define IO_PACKET_WORKER_H

#include <map>
#include <string>

#include "system/io/IoConnection.h"
#include "system/io/protocol/IoWorker.h"

class IoPacketWorker : public IoWorker
{
protected :
	byte *m_buf;
	int m_size;

public :
	IoPacketWorker();
	virtual ~IoPacketWorker();

	virtual void setData(IoConnection *cnn, byte *data, int size);
};

#endif


