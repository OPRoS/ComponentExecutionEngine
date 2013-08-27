/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CerrLogger.h
 *
 */
#ifndef _CERR_LOGGER_H_
#define _CERR_LOGGER_H_

#include <iostream>
#include <fstream>

#include "Logger.h"

/**
	std::cerr에 로그 메시지를 출력하는 로거 클래스.
*/
class CerrLogger : public Logger
{
public:
	CerrLogger() { };
	virtual ~CerrLogger() {};

	/**
		std::cerr에 로그 메시지를 출력하는 함수.
		StringStream::output_string() 함수를 구현한 함수.

		@param str 출력할 로그 메시지

		@see StringStream::output_string
	*/
	virtual void output_string(const std::string &str) {
		std::cerr << str;
		std::cerr.flush();
	};
};


#endif 
