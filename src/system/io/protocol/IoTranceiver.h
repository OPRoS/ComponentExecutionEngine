/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoTranceiver.h
 *
 */

#ifndef IO_TRANSCEIVER_H
#define IO_TRANSCEIVER_H

#include "system/util/DataObject.h"

class IoTranceiver : public DataObject
{
public :
	IoTranceiver() {};
	virtual ~IoTranceiver() {};

	virtual bool onSend() = 0;
	virtual bool onReceive() = 0;
	virtual void onClose() = 0;
};

#endif


