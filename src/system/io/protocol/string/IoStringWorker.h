/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStringWorker.h
 *
 */

#ifndef _IO_STRING_WORKER_H_
#define _IO_STRING_WORKER_H_

#include <map>
#include <string>

#include "system/io/IoConnection.h"
#include "system/io/protocol/IoWorker.h"

class IoStringWorker : public IoWorker
{
protected :
	std::string m_str;

public :
	IoStringWorker();
	virtual ~IoStringWorker();

	virtual void setData(IoConnection *cnn, const std::string &str);
};

#endif


