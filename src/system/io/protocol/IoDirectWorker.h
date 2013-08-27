/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoDirectWorker.h
 *
 */

#ifndef IO_DIRECT_WORKER_H
#define IO_DIRECT_WORKER_H

#include <string>
#include <sstream>

#include "system/util/Exceptions.h"
#include "system/os/OSNotify.h"
#include "system/os/OSThread.h"
#include "system/io/IoConnection.h"
#include "system/util/StringStream.h"

/**
	@class IoDirectWorker

	@brief 커넥션에서 들어오는 요청을 처리하거나 커넥션에 데이터를 출력하는 함수

	@remark 커넥션이 연결되면 할당되어 해당 커넥션을 전담하여 처리하는 쓰레드
*/
class IoDirectWorker : public OSThread, public StringStream
{
private :
	/** 이 워커가 담당할 커넥션 */
	IoConnection *m_cnn;

public :
	/** 스트링 출력을 위한 스트림*/
	std::ostringstream m_oss;

	IoDirectWorker(IoConnection *cnn);
	virtual ~IoDirectWorker();

	/** 작업을 시작하는 함수로 쓰레드를 시작 시킴 */
	virtual void startWork();

	/** 실제 커넥션을 처리하는 함수로 IoDirectWorker을 상속 받은 함수에서 구현 */
	virtual void processWork() throw (IOException) = 0;

	/** 작업의 중지를 요청하는 함수 */
	void stopWork();

	/** OSRunnable 구현 함수. 쓰레드 실행 함수. */
	virtual void run();

	/**
		@brief 커넥션에 데이터를 출력하는 함수

		@param buf 출력할 데이터
		@param len 출력할 데이터 크기

		@return 커넥션에 출력한 바이트 수
	*/
	virtual int write(const byte *buf, size_t len) throw (IOException);

	/**
		@brief 커넥션에서 데이터를 읽는 함수

		@param buf 읽은 데이터를 저장할 버퍼
		@param len 읽은 데이터 크기

		@return 커넥션에서 읽은 바이트 수
	*/
	virtual int read(byte *buf, size_t len) throw (IOException);

	/**
		@brief 커넥션에 std::string을 출력하는 함수

		@param str 출력할 std::string 

		@return 커넥션에 출력한 바이트 수
	*/
	virtual int writeString(const std::string &str) throw (IOException);

	/**
		@brief 커넥션에 c string을 출력하는 함수

		@param buf 출력할 c string 데이터
		@param len 출력할 데이터 크기

		@return 커넥션에 출력한 바이트 수
	*/
	virtual int writeString(const char *buf, size_t len) throw (IOException);

	/**
		@brief 커넥션에서 한 줄(라인)을 읽는 함수

		@param buf 읽은 데이터를 저장할 버퍼
		@param len 읽은 데이터 크기

		@return 커넥션에서 읽은 바이트 수
	*/
	virtual int readLine(char *buf, size_t len) throw (IOException);

	/**
		@brief 스트링을 커넥션에 출력하는 함수

		@param str 스트림에 쓸 스트링
	*/
	virtual void output_string(const std::string &str) {
		writeString(str);
	};

	/**
		@brief 커넥션에서 문자 하나를 입력 받는 함수

	    @return 입력 받은 문자
	*/
	virtual char input_char() {
		char ch;
		read((byte *) &ch, 1);
		return ch; 
	};
};

#endif


