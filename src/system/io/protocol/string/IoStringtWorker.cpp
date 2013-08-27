/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStringWorker.cpp
 *
 */

#include "IoStringWorker.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoStringWorker::IoStringWorker()
{
}

IoStringWorker::~IoStringWorker()
{
}

void IoStringWorker::setData(IoConnection *cnn, const std::string &str)
{
	trace_enter();

	m_str = str;
}
