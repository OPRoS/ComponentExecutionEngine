/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoRequestWorker.h
 *
 */

#ifndef IO_REQUEST_WORKER_H
#define IO_REQUEST_WORKER_H

#include <map>
#include <string>

#include "system/io/IoConnection.h"
#include "system/io/protocol/IoWorker.h"

/**
	원격 노드에서의 Request 요청을 처리하고 요청에 응답이 전달하는 함수.
*/
class IoRequestWorker : public IoWorker
{
protected :
	byte *m_buf;
	int m_size;
	long m_reqId;

public :
	IoRequestWorker();
	virtual ~IoRequestWorker();

	/**
		원격 노드에서 요청이 왔을 때 요청을 처리하기 전에 
		관련 정보를 세팅하기 위해 호출되는 함수.

		@param cnn 요청을 받은 IoConnection 객체
		@param reqId 요청 아이디
		@param data  요청 파라미터
		@param dataSize 요청 파라미터 사이즈
	*/
	virtual void setRequest(IoConnection *cnn, long reqId, byte *data, int size);
};

#endif


