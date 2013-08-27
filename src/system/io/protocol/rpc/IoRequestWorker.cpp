/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoRequestWorker.cpp
 *
 */

#include "IoRequestWorker.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoRequestWorker::IoRequestWorker()
{
	m_buf = NULL;
	m_size = 0;
}

IoRequestWorker::~IoRequestWorker()
{
}

void IoRequestWorker::setRequest(IoConnection *cnn, long reqId, byte *buf, int size)
{
	trace_enter();

	m_reqId = reqId;
	m_buf = buf;
	m_size = size;
}
