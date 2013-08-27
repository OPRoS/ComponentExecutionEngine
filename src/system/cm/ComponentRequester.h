/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentRequester.cpp
 *
 */

#ifndef _COMPONENT_REQUESTER_H
#define _COMPONENT_REQUESTER_H

#include <string>

#include "ComponentLocator.h"
#include "NodeLocator.h"

#include "system/io/protocol/IoWorkerManager.h"
#include "system/io/protocol/rpc/IoRequestProtocol.h"
#include "system/io/protocol/rpc/IoResponse.h"

/** 
	원격 노드에 존재하는 컴포넌트을 접근하거나
	원격 노드의 컴포넌트 접근 요청을 처리하기 위한 객체 
*/
class ComponentRequester : public IoWorkerManager, public IoRequestProtocol
{
	OSMutex m_mutex;
	

	/** 컴포넌트가 존재하는 노드의 주소를 관리하는 객체 */
	ComponentLocator *m_componentLocator;

	/** 노드의 실제 주소를 저장하고 있는 객체 */
	NodeLocator *m_nodeLocator;

protected:

	/**
		IoAcceptor에서 커넥션 연결 요청이 오면 IoRequestProtocol::connectionAccept() 함수에서
		콜백 되는 함수.

		@param cnn 연결된 커넥션 객체

		@return 커넥션 처리 성공 여부
	*/
	virtual bool handleConnectionAccept(IoConnection *cnn);

	/**
		IoConnector에서 커넥션 연결 요청이 오면 IoRequestProtocol::connectionConnected() 함수에서
		콜백 되는 함수.

		@param cnn 연결된 커넥션 객체

		@return 커넥션 처리 성공 여부
	*/
	virtual bool handleConnectionConnect(IoConnection *cnn);

	/**
		커넥션 연결이 종료되면 IoRequestProtocol::connectionClosed() 함수에서
		콜백 되는 함수.

		@param cnn 연결이 해제된 커넥션 객체
	*/
	virtual void handleConnectionClose(IoConnection *cnn);

public :
	ComponentRequester();
	virtual ~ComponentRequester();

	void setComponentLocator(ComponentLocator *l) { m_componentLocator = l; };
	ComponentLocator *getComponentLocator() { return m_componentLocator; };

	void setNodeLocator(NodeLocator *l) { m_nodeLocator = l; };
	NodeLocator *getNodeLocator() { return m_nodeLocator; };

	/**
		원격 노드에서 요청이나 응답이 왔을 때 처리할 worker를 생성하는 함수.
		IoWorkerManager의 순수 함수 
	*/
	virtual IoWorker *createWorker();

	/**
		컴포넌트 이름과 프로토콜 정보를 이용하여 해당 컴포넌트가 있는 노드 주소 탐색.
		컴포넌트 이름을 이용하여 해당 컴포넌트가 존재하는 노드 id를 구할 수 있음

		@param compName 컴포넌트 이름
		@param protocol 삭제할 노드 내의 프로토콜

		@return std::string 해당 컴포넌트가 존재하는 노드의 주소
	*/
	std::string findConnectionId(const std::string &compName, const std::string &protocol);

	/**
		요구 서비스 포트(required service port)를 이용하여 메소드 호출을 수행하는 함수.
		응답이 올때까지 일정시간 기다린 후 응답을 IoResponse 객체에 전달함.

		@param compName  컴포넌트 이름
		@param portName  서비스 포트 이름
		@param methodName 호출할 서비스 포트의 메소드 이름
		@param param 메소드의 파라미터를 패킹한 바이너리 데이터
		@param paramSize param의 길이
		@param protocol 메소드 호출 시 사용할 프로토콜 이름

		@return IoResponse * 메소드 호출의 응답을 저장할 객체
	*/
	virtual IoResponse *methodCall(const std::string &compName, const std::string &portName, const std::string &methodName, \
		byte *param, int paramSize, const std::string &protocol = "");

	/**
		제공 서비스 포트(provided service port)를 이용하여 메소드 요청이 왔을 때
		이에 대한 응답을 전달하는 함수.
		- 제공 서비스 포트(provided service port)를 이용하여 메소드 요청이 오면
		  이를 처리할 worker가 할당됨
		- 해당 worker가 메소들 호출 결과를 methodResponse() 함수를 이용하여 전달함

		@param cnnId   응답을 보낼 커넥션 아이디
		@param reqId   응답에 대응하는 요청 아이디
		@param retOK   메소드 호출 성공 여부 (메소드 호출 실패 시 false)
		@param compName  컴포넌트 이름
		@param portName  서비스 포트 이름
		@param methodName 응답을 보낼 서비스 포트의 메소드 이름
		@param param 응답을 패킹한 바이너리 데이터
		@param paramSize param의 길이
		@param protocol 응답 시 사용할 프로토콜 이름

		@return bool 응답 전달 성공 여부
	*/
	virtual bool methodResponse(const std::string &cnnId, long reqId, bool retOK, \
		const std::string &compName, const std::string &portName, const std::string &methodName, \
		byte *retValue, int retValueSize, const std::string &protocol = "");

	/**
		output data port를 이용하여 데이터를 전달할 때 사용되는 함수.

		@param compName  컴포넌트 이름
		@param portName  데이터 포트 이름
		@param data 전달할 데이터를 바이너리로 패킹한 것
		@param dataSize data의 길이
		@param protocol 사용할 프로토콜 이름

		@return IoResponse * 메소드 호출의 응답을 저장할 객체
	*/
	virtual bool dataCall(const std::string &compName, const std::string &portName, \
		byte *data, int dataSize, const std::string &protocol = "");

	/**
		output event port를 이용하여 이벤트를 전달할 때 사용되는 함수.

		@param compName  컴포넌트 이름
		@param portName  이벤트 포트 이름
		@param evt 전달할 이벤트를 바이너리로 패킹한 것
		@param evtSize evt의 길이
		@param protocol 사용할 프로토콜 이름

		@return IoResponse * 메소드 호출의 응답을 저장할 객체
	*/
	virtual bool eventCall(const std::string &compName, const std::string &portName, \
		byte *evt, int evtSize, const std::string &protocol = "");
};

#endif
