/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : NullStream.h
 *
 */
#ifndef _NULL_STREAM_H_
#define _NULL_STREAM_H_

#include <iostream>
#include <streambuf>

/**
	@class NullStreamBuf 
	@brief a stream buffer for a null stream
*/
class NullStreamBuf : public std::streambuf 
{
protected :
	/**
		사용자가 스트림에 데이터를 기록할 때 콜백되는 함수.
	    NULLStreamBuf이므로 들어오는 데이터를 무시함.
	*/
	virtual int overflow(int c) {
		return std::char_traits<char>::not_eof(c);
	};

	virtual int_type underflow() {
		return std::char_traits<char>::eof();
	}
};


/**
	@class OStringStream
    @brief 스트링을 쓸 수 있는 스트림
	
	@remark 내부적으로 StringStreamBuf를 가지고 있음

	@see StringStreamBuf
 */
class NullStream : public std::istream, public std::ostream
{
	NullStreamBuf m_nsb;

public:
	/** 생성자 */
	NullStream() : std::istream(&m_nsb), std::ostream(&m_nsb) {};

	/** 소멸자 */
	virtual ~NullStream() {};
};

#endif 
