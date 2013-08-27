/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentRequestWorker.cpp
 *
 */

#ifndef _COMPONENT_REQUEST_WORKER_H
#define _COMPONENT_REQUEST_WORKER_H

#include <string>

#include "system/io/protocol/IoStreamByte.h"
#include "system/io/protocol/rpc/IoRequestWorker.h"

class ComponentRequester;

/**
	원격 노드에서의 요청을 처리하고 요청에 응답이 전달하는 함수.
*/
class ComponentRequestWorker : public IoRequestWorker
{
protected :
	ComponentRequester *m_requestor;

	/** */
	IoStreamByte m_ios;

public :
	ComponentRequestWorker();
	ComponentRequestWorker(ComponentRequester *r);
	virtual ~ComponentRequestWorker();

	void setComponentRequester(ComponentRequester *r) { m_requestor = r; };
	ComponentRequester *getComponentRequester() { return m_requestor; };


	/**
		원격 노드에서 요청을 처리하는 함수.
		메소드 호출, 데이터 전송, 이벤트 전송인지에 따라 다른 일을 수행함.
	*/
	virtual void processWork();
};

#endif  

