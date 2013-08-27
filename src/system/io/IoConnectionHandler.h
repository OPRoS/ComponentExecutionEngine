/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 12
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoConnectionHandler.h
 *
 */

#ifndef _IO_CONNECTION_HANDLER_H_
#define _IO_CONNECTION_HANDLER_H_

#include <string>

#include "IoConnection.h"

//----------------------------------------------------------------------------------
// 커넥션이 연결되었을 때 어떻게 처리할지를 정의하는 enum 타입
// -IO_CONNECTION_FAIL : 커넥션 연결을 하지 말라는 플래그
// -IO_CONNECTION_OK : 정상적으로 커넥션 연결을 하고 
//                     커넥션 관리는 IoEventProcessor에게 하라는 플래그
// -IO_CONNECTION_DIRECT : 정상적으로 커넥션이 연결되었으며,
//                        커넥션 관리는 해당 객체가 알아서 하겠다는 플래그로
//                        IoEventProcessor에 해당 커넥션을 등록하지 말라는 플래그
typedef enum { IO_CONNECTION_FAIL, IO_CONNECTION_OK, IO_CONNECTION_DIRECT } IoConnectionResult;

// IoConnectionHandler
class IoConnectionHandler
{
	std::string m_handlerId;

public:
	IoConnectionHandler() {};
	virtual ~IoConnectionHandler() {};

	virtual IoConnectionResult connectionAccept(IoConnection *cnn) = 0;
	virtual bool connectionAcceptAfter(IoConnection *cnn) { return true; };

	virtual IoConnectionResult connectionConnect(IoConnection *cnn) = 0;
	virtual bool connectionConnectAfter(IoConnection *cnn) { return true; };

	virtual bool connectionClose(IoConnection *cnn) = 0;

	virtual bool connectionRead(IoConnection *cnn) = 0;
	virtual bool connectionWrite(IoConnection *cnn) = 0;

	std::string getConnectionHandlerId() { return m_handlerId; };
	void setConnectionHandlerId(const std::string &id) { m_handlerId = id; };
};

#endif 
