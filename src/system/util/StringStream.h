/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : StringStream.h
 *
 */

#ifndef _STRING_STREAM_H_
#define _STRING_STREAM_H_

#include <string>
#include <iostream>
#include <sstream>
#include <streambuf>

class StringStream;

/**
	@class StringStreamBuf
	@brief stringbuf for string stream
*/
class StringStreamBuf : public std::streambuf
{
	/** string stream which includes this StringStreamBuf */
	StringStream *m_stream;

	/** 사용자가 스트림에 기록한 스트링을 저장하기 위한 버퍼 */
	std::string m_buf;

	char m_ibuf;

public:
	/** NULL 생성자 */
	StringStreamBuf() : m_stream(NULL) {};

	/** 생성자 */
	StringStreamBuf(StringStream *stream) : m_stream(stream) {};

	/** 소멸자 */
	virtual ~StringStreamBuf() { flush(); };

	void setStream(StringStream *stream) { m_stream = stream; };
	StringStream *getStream() { return m_stream; };

	/** 사용자가 스트림에 데이터를 기록할 때 버퍼에 오버플로우가 일어나면 콜백되는 함수 */
	int_type overflow(int_type i);

	/** 사용자가 스트림에서 데이터를 가져갈 때 가져갈 데이터가 버퍼에 없는 경우 콜백되는 함수 */
	int_type underflow();

	/** 버퍼에 있는 모든 문자를 비우는 함수 */
	void flush();
};

/**
	@class StringStream

	@brief 스트링을 쓸 수 있는 스트림
	@remark	내부적으로 StringStreamBuf를 가지고 있음

	@see StringStreamBuf
 */
class StringStream : public std::iostream
{
	StringStreamBuf m_ssb;

public:
	/** 생성자 */
	StringStream() : std::iostream(&m_ssb) {
		m_ssb.setStream(this);
	};

	/** 소멸자 */
	virtual ~StringStream() {};

	/**
		@brief 스트링을 출력하는 함수

		@remark StringStreamBuf의 flush에서 호출되는 함수

		@param str 스트림에 쓸 스트링
	*/
	virtual void output_string(const std::string &str) {};

	/**
		@brief 문자 하나를 입력 받는 함수

		@remark StringStreamBuf의 underflow가 발생했을 때 호출되는 함수

	    @return 입력 받은 문자
	*/
	virtual char input_char() { return StringStreamBuf::traits_type::eof(); };

};

#endif
