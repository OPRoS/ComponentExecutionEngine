/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoMsg.h
 *
 */

#ifndef _IO_MSG_H_
#define _IO_MSG_H_

#include <iostream>

class IoStream;

class IoMsg
{
public :
	IoMsg() {};
	virtual ~IoMsg() {};


	//
	// user defined
	//
	virtual void serialize(IoStream &ios) = 0;
	virtual void deserialize(IoStream &ios) = 0;

	friend IoStream & operator<<(IoStream &ios, IoMsg &msg) {
		msg.serialize(ios);
		return ios;
	};

	friend IoStream & operator>>(IoStream &ios, IoMsg &msg) {
		msg.deserialize(ios);
		return ios;
	};
};

#endif // _IO_MSG_H_
