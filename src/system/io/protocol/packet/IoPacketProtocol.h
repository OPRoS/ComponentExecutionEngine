/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoPacketProtocol.h
 *
 */

#ifndef IO_PACKET_PROTOCOL_H
#define IO_PACKET_PROTOCOL_H

#include <map>
#include <string>

#include "system/os/OSMutex.h"
#include "system/io/protocol/IoProtocol.h"

/** 
	@class IoPacketProtocol
	@brief packet을 송수신하는 프로토콜을 구현함
	@see IoProtocol
*/
class IoPacketProtocol : public IoProtocol
{
protected :
	/** IoTranceiver를 생성하는 함수 */
	virtual IoTranceiver *createTransceiver(IoConnection *cnn);

public :
	IoPacketProtocol();
	virtual ~IoPacketProtocol();

	/** 
		@brief 패킷 전송 함수.

		@remark byte * 데이터를 전송하는 함수

		@param cnnAddr 패킷을 전송할 IoConnection의 주소
		@param body 전송할 패킷 내용(unsigned char * = byte)
		@param bodySize 패킷 크기

		@return 전송 성공 여부
	*/
	virtual bool packet_send(const std::string &cnnAddr, byte *body, int bodySize);

	/** 
		@brief 패킷 전송 함수.

		@remark 임의의 데이터 타입(T obj)을 전송하는 함수

		@param cnnAddr 패킷을 전송할 IoConnection의 주소
		@param obj 전송할 객체 (IoStream << obj 가 가능한 객체)

		@return 전송 성공 여부
	*/
	template <typename T> bool packet_send(const std::string &cnnAddr, T obj);

	/** 
		@brief 패킷 수신 함수.

		@remark IoTransceiver에서 수신된 패킷을 처리할 함수

		@param cnn 패킷을 수신한 IoConnection
		@param body 수신한 패킷 내용
		@param bodySize 수신한 패킷 크기

		@return 수신 성공 여부
	*/
	virtual bool packet_receive(IoConnection *cnn, byte *body, int bodySize);
};

#endif
