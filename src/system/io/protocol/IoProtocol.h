/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoProtocol.h
 *
 */

#ifndef IO_PROTOCOL_H
#define IO_PROTOCOL_H

#include <map>
#include <string>

#include "system/os/OSMutex.h"
#include "system/io/IoConnectionHandler.h"
#include "system/io/protocol/IoWorkerManager.h"
#include "system/io/protocol/IoTranceiver.h"

/**
	커넥션에서 들어오는 요청을 처리하거나 커넥션에 데이터를 출력하는 함수. 	
*/
class IoProtocol : public IoConnectionHandler
{
protected :
	IoWorkerManager *m_workerMgr;

	OSMutex m_mutex;
	IoConnectionMap	m_cnnMap;

protected:

	virtual bool handleConnectionAccept(IoConnection *cnn) { return true; };
	virtual bool handleConnectionConnect(IoConnection *cnn) { return true; };
	virtual void handleConnectionClose(IoConnection *cnn) {};

	virtual IoTranceiver *createTransceiver(IoConnection *cnn) = 0;

public :

	IoProtocol();
	virtual ~IoProtocol();

	void setWorkerManager(IoWorkerManager *mgr) { m_workerMgr = mgr; };
	IoWorkerManager *getWorkerManager() { return m_workerMgr; };

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

		@param cnn 커넥션 객체

		@return 성공 여부
	*/
	virtual bool connectionRead(IoConnection *cnn);

	/**
		커넥션에 데이터를 쓸 수 있는 상태이면 호출되는 함수.
		커넥션이 non blocking일 때에 event processor에서 호출됨.

		@param cnn 커넥션 객체

		@return 성공 여부
	*/
	virtual bool connectionWrite(IoConnection *cnn);

	/**
		커넥션을 검색하는 함수.

		@param cnnId 검색할 커넥션 아이디

		@return IoConnection * 검색한 커넥션 객체
	*/
	virtual IoConnection *findConnection(const std::string &cnnId);
};

#endif
