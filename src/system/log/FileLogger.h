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
	���Ͽ� �α� �޽����� ����ϴ� �ΰ� Ŭ����.
*/
class FileLogger : public Logger
{
	/** �α� �޽����� ����� ���� ��Ʈ��. */
	std::ofstream m_ofs;

	/** �ΰŰ� ���� �Ǿ����� ��Ÿ���� �÷���. */
	bool m_opened;

public:
	FileLogger();
	virtual ~FileLogger();

	/** ���� �ΰ� open �Լ� */
	virtual	bool open(Properties &props);

	/**
		���Ͽ� �α� �޽����� ����ϴ� �Լ�.
		StringStream::output_string() �Լ��� ������ �Լ�.

		@param str ����� �α� �޽���

		@see StringStream::output_string
	*/
	virtual void output_string(const std::string &str);

};


#endif 
