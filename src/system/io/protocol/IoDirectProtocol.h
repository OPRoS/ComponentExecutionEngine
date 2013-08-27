/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoDirectProtocol.h
 *
 */

#ifndef IO_DIRECT_PROTOCOL_H
#define IO_DIRECT_PROTOCOL_H

#include <map>
#include <string>

#include "system/os/OSMutex.h"
#include "system/os/OSNotify.h"
#include "system/io/IoConnectionHandler.h"
#include "system/os/OSThread.h"

#include "IoDirectWorker.h"

/**
	커넥션이 연결되면 전담 쓰레드를 할당하여 해당 커넥션을
	처리하기 위한 프로토콜을 구현한 클래스.
*/
class IoDirectProtocol : public IoConnectionHandler
{
protected:

	virtual bool handleConnectionAccept(IoConnection *cnn) { return true; };
	virtual bool handleConnectionConnect(IoConnection *cnn) { return true; };
	virtual void handleConnectionClose(IoConnection *cnn) {};

public :

	IoDirectProtocol();
	virtual ~IoDirectProtocol();

	virtual IoDirectWorker *createDirectWorker(IoConnection *cnn) = 0;

	/**
		IoManager의 IoAcceptor에서 커넥션 연결 요청이 오면 호출하는 함수.
	
		@param cnn 연결할 커넥션
	
		@return bool 커넥션 연결 후 해당 커넥션을 IoEventProcessor에게 할당하여
	                 처리할 건지 아니면 직접 처리할 건지를 나타내는 플래그
	*/
	virtual IoConnectionResult connectionAccept(IoConnection *cnn);
	virtual bool connectionAcceptAfter(IoConnection *cnn) { return true; };

	/**
		IoManager에서 IoConnector 커넥션 연결 요청이 오면 호출하는 함수

		@param cnn 연결할 커넥션

		@ret bool 커넥션 연결 후 해당 커넥션을 IoEventProcessor에게 할당하여
		           처리할 건지 아니면 직접 처리할 건지를 나타내는 플래그
	*/
	virtual IoConnectionResult connectionConnect(IoConnection *cnn);
	virtual bool connectionConnectAfter(IoConnection *cnn) { return true; };

	/**
		커넥션의 연결이 종료되었을 때 호출되는 함수.

		@param cnn 커넥션 객체
	*/
	virtual bool connectionClose(IoConnection *cnn);

	/**
		커넥션에서 읽을 데이터가 있으면 호출되는 함수.
		커넥션이 non blocking일 때에 event processor에서 호출됨.
		IoDirectProtocol에서는 직접 커넥션을 접근하므로 호출되지 않음.

		@param cnn 커넥션 객체

		@return 성공 여부
	*/
	virtual bool connectionRead(IoConnection *cnn);

	/**
		커넥션에 데이터를 쓸 수 있는 상태이면 호출되는 함수.
		커넥션이 non blocking일 때에 event processor에서 호출됨.
		IoDirectProtocol에서는 직접 커넥션을 접근하므로 호출되지 않음.

		@param cnn 커넥션 객체

		@return 성공 여부
	*/
	virtual bool connectionWrite(IoConnection *cnn);
};

#endif
