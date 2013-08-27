/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : CoutLogger.h
 *
 */
#ifndef _COUT_LOGGER_H_
#define _COUT_LOGGER_H_

#include <iostream>
#include <fstream>

#include "Logger.h"

/**
	std::cout�� �α� �޽����� ����ϴ� �ΰ� Ŭ����.
*/
class CoutLogger : public Logger
{
public:
	CoutLogger() { };
	virtual ~CoutLogger() {};

	/**
		std::cout�� �α� �޽����� ����ϴ� �Լ�.
		StringStream::output_string() �Լ��� ������ �Լ�.

		@param str ����� �α� �޽���

		@see StringStream::output_string
	*/
	virtual void output_string(const std::string &str) {
		std::cout << str;
		std::cout.flush();
	};
};


#endif 
