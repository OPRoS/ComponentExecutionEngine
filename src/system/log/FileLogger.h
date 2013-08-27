/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : FileLogger.h
 *
 */
#ifndef _FILE_LOGGER_H_
#define _FILE_LOGGER_H_

#include <iostream>
#include <fstream>

#include "Logger.h"

/**
	파일에 로그 메시지를 출력하는 로거 클래스.
*/
class FileLogger : public Logger
{
	/** 로그 메시지를 출력할 파일 스트림. */
	std::ofstream m_ofs;

	/** 로거가 오픈 되었는지 나타내는 플래그. */
	bool m_opened;

public:
	FileLogger();
	virtual ~FileLogger();

	/** 파일 로거 open 함수 */
	virtual	bool open(Properties &props);

	/**
		파일에 로그 메시지를 출력하는 함수.
		StringStream::output_string() 함수를 구현한 함수.

		@param str 출력할 로그 메시지

		@see StringStream::output_string
	*/
	virtual void output_string(const std::string &str);

};


#endif 
