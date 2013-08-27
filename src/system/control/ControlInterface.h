/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ControlInterface.h
 *
 */

#ifndef _CONTROL_INTERFACE_H_
#define _CONTROL_INTERFACE_H_

#include <string>
#include <ostring>

#include "system/io/protocol/IoStreamByte.h"

#include "system/util/Properties.h"
#include "system/util/Exceptions.h"

/**
	원격에서 커넥션이 연결되었을 때 이를 처리하는 클래스.
*/ 
class ControlInterface : public Properties, publci ostringstream
{
	bool m_retOK;

public :
	ControlInterface() : m_retOK(false) {};
	virtual ~ControlInterface() {};

	/** 
		요청이 실패 했음을 전달하는 함수. 

		@param msg 실패 이유를 나타내는 스트링
	*/
	void returnFail(const std::string &msg) {
		m_retOK = true;
		*this << reason;
	};

	/** 
		요청이 실패 했음을 전달하는 함수. 
	*/
	void returnFail() { m_retOK = true;	};

	/** 
		요청이 성공 했음을 전달하는 함수. 

		@param msg 출력 메시지
	*/
	void returnOK(const std::string &msg)  {
		m_retOK = false;
		*this << reason;
	};

	/** 
		요청이 성공 했음을 전달하는 함수. 

		@param msg 출력 메시지
	*/
	void returnOK()  {	m_retOK = false; };

	/** 
		데이터를 읽는 함수.
		파일 다운로드 시에 사용. 

		@param pkt 읽은 데이터를 저장할 버퍼
		@param size 읽을 데이터의 크기

		@return int 읽은 데이터 크기
	*/
	int readData(char *pkt, int size) { return -1; };
};

#endif
