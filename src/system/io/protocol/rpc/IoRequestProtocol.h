/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoRequestProtocol.h
 *
 */

#ifndef IO_REQUEST_PROTOCOL_H
#define IO_REQUEST_PROTOCOL_H

#include <map>
#include <string>

#include "system/io/protocol/IoProtocol.h"

#include "IoRequest.h"
#include "IoResponse.h"
#include <opros_smart_ptr.h>

//#include "boost/shared_ptr.hpp"

//typedef boost::shared_ptr<IoRequest> IoRequestPtr;
typedef opros::smart_ptr<IoRequest> IoRequestPtr;

/** 
	@class IoRequestProtocol
	@brief RPC Request를 송수신하는 프로토콜을 구현함
	@see IoProtocol
*/
class IoRequestProtocol : public IoProtocol
{
protected :
	int32 m_reqId;

protected:
	/** IoRequest의 아이디 생성 함수 */
	long nextRequestId();

	/** 
		@brief		Request를 전송하는 함수

		@remark		request를 전송하고 바로 제어 반환. 
					응답이 올때까지 기다리지 않음

		@param cnnId	Request를 전송할 IoConnection의 주소
		@param req		전송할 Request 

		@return	전송 성공 여부 
	*/
	bool sendRequest(const std::string &cnnId, IoRequestPtr req);

	/** IoTranceiver를 생성하는 함수 */
	virtual IoTranceiver *createTransceiver(IoConnection *cnn);

public :

	IoRequestProtocol();
	virtual ~IoRequestProtocol();

	/**
		@brief	request를 전달하는 함수.

		@remark	request를 통해 메시지(byte *body)를 전달하고 응답이 올때까지 일정시간 기다림.

		@param cnnId	요청을 보낼 노드와의 커넥션 주소
		@param body		전달할 메시지 바디
		@param bodySize	전달할 메시지 사이즈
		@param timeout	응답이 올때까지 기다릴 시간
		
		@return 응답을 포함하고 있는 객체
	*/
	virtual IoResponse*  rpc_request(const std::string &cnnId, byte *body, int bodySize, long timeout = REQUEST_TIMEOUT);

	/**
		@brief	request를 전달하는 함수.

		@remark	request를 통해 임의 타입의 데이터를 전달하고 응답이 올때까지 일정시간 기다림.

		@param cnnId	요청을 보낼 노드와의 커넥션 주소
		@param obj		전달할 객체 (IoStream의 << 연산자로 구현된 객체)
		@param timeout	응답이 올때까지 기다릴 시간
		
		@return 응답을 포함하고 있는 객체
	*/
	template <typename T> IoResponse* rpc_request(const std::string &cnnId, T obj, long timeout = REQUEST_TIMEOUT);

	
	/**
		@brief	request에 대한 응답을 전달하는 함수.
		
		@remark 원격 노드로부터 온 요청에 대해 메시지(byte *body)를 응답으로 전달하는 함수
		
		@param cnnId	요청을 보낼 노드와의 커넥션의 주소
		@param reqId	원격노드로 부터 온 요청에 대한 id
		@param retOK	요청 실패 여부 (true : 성공, false : 실패)
		@param body		전달할 메시지 바디
		@param bodySize	전달할 메시지 사이즈
		
		@return 응답을 포함하고 있는 객체
	*/
	virtual bool rpc_response(const std::string &cnnId, long reqId, bool retOK, byte *body, int bodySize);

	/**
		@brief	request에 대한 응답을 전달하는 함수.
		
		@remark 원격 노드로부터 온 요청에 대한 임의 타입의 데이터를 응답으로 전달하는 함수
		
		@param cnnId	요청을 보낼 노드와의 커넥션의 주소
		@param reqId	원격노드로 부터 온 요청에 대한 id
		@param retOK	요청 실패 여부 (true : 성공, false : 실패)
		@param obj		전달할 객체 (IoStream의 << 연산자로 구현된 객체)
		
		@return 응답을 포함하고 있는 객체
	*/
	template <typename T>  bool rpc_response(const std::string &cnnId, long reqId, bool retOK, T obj);

	/**
		@brief	request를 전달하는 함수.
		
		@remark 원격 노드에 요청을 보낸 후 응답이 올때까지 기다리지 않고 바로 리턴하는 함수.
		        추후 리턴 값인 IoRequestPtr을 이용하여 응답을 받음.
		
		@param cnnId	요청을 보낼 노드와의 커넥션의 주소
		@param body		전달할 메시지 바디
		@param bodySize	전달할 메시지 사이즈
		
		@return 요청에 대한 정보를 포함하고 있는 객체
	*/
	virtual IoRequestPtr rpc_send(const std::string &cnnId, byte *body, int bodySize);

	/**
		@brief	request를 전달하는 함수.
		
		@remark 원격 노드에 요청을 보낸 후 응답이 올때까지 기다리지 않고 바로 리턴하는 함수.
		        추후 리턴 값인 IoRequestPtr을 이용하여 응답을 받음.
		
		@param cnnId	요청을 보낼 노드와의 커넥션의 주소
		@param obj		전달할 객체 (IoStream의 << 연산자로 구현된 객체)
		
		@return 요청에 대한 정보를 포함하고 있는 객체
	*/
	template <typename T> IoRequestPtr rpc_send(const std::string &cnnId, T obj);

	/**
		@brief	request를 전달하는 함수.
		
		@remark 원격 노드에 요청을 보낸 후 응답이 올때까지 기다리지 않고 바로 리턴하는 함수.
		        리턴 값이 없느 경우에 사용함.
		
		@param cnnId	요청을 보낼 노드와의 커넥션의 주소
		@param body		전달할 메시지 바디
		@param bodySize	전달할 메시지 사이즈
		
		@return 전송 성공 여부(상대방이 해당 request를 받았는지는 알 수 없으며
				단지 해당 request가 에러 없이 전송되었는지만 나타냄)
	*/
	virtual bool rpc_msg(const std::string &cnnId, byte *body, int bodySize);

	/**
		@brief	request를 전달하는 함수.
		
		@remark 원격 노드에 요청을 보낸 후 응답이 올때까지 기다리지 않고 바로 리턴하는 함수.
		        리턴 값이 없느 경우에 사용함.
		
		@param cnnId	요청을 보낼 노드와의 커넥션의 주소
		@param obj		전달할 객체 (IoStream의 << 연산자로 구현된 객체)
		
		@return 전송 성공 여부(상대방이 해당 request를 받았는지는 알 수 없으며
				단지 해당 request가 에러 없이 전송되었는지만 나타냄)
	*/
	template <typename T> bool rpc_msg(const std::string &cnnId, T obj);

	/** 
		@brief request 수신 함수.

		@remark IoTransceiver에서 수신된 request를 처리할 함수

		@param cnn	request를 수신한 IoConnection
		@param rs	수신한 request

		@return 수신 성공 여부
	*/
	virtual bool rpc_receive(IoConnection *cnn, IoRequest &rs);
};

#endif
