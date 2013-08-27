/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoResponse.h
 *
 */

#ifndef _IO_RESPONSE_H_
#define _IO_RESPONSE_H_

#include "system/util/BasicTypes.h"

class IoResponse
{
private:
	byte *m_buf;
	int m_size;

public:
	IoResponse() : m_buf(NULL), m_size(0) {};
	virtual ~IoResponse() {};

	void  setBuffer(byte *buf, int size) { m_buf = buf; m_size = size; };
	byte *getBuffer() { return m_buf; };

	int   getSize() { return m_size; };
};

#endif // _IO_BUFFER_ADAPTER_H_
