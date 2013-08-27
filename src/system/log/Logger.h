/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Logger.h
 *
 */
#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <iostream>
#include <streambuf>
#include <locale>
#include <cstdio>

#include "system/util/Properties.h"
#include "system/util/StringStream.h"
#include "system/util/NullStream.h"

/** ����Ʈ �α� ���� Ÿ�� */
enum { 	LOG_LEVEL_NONE = 0,
		LOG_LEVEL_ERROR = 100,
		LOG_LEVEL_WARN 	= 200,
		LOG_LEVEL_INFO 	= 300,
		LOG_LEVEL_DEBUG = 400,
};


/**
	�α� ������ ����� �ΰ� Ŭ����.
*/
class Logger : public StringStream
{
	/** �ΰ� �̸� */
	std::string m_name;

	/** �α� ���� */
	int m_level;

	/** 
		����ϰ��� �ϴ� �α� ������ ���õ� �α� �������� ���� ���
		�ش� �޽����� ������� �ʾƾ� �Ѵ�.
		�̸� ���� NULL ��Ʈ�� ��ü�� ����Ѵ�.
	*/
	NullStream    m_nullStream;

	/** �α� �¾� �Լ�. */
	void setup();

public:
	Logger();
	virtual ~Logger();

	/** �ΰ� �̸� ���� �Լ�. */
	void setName(const std::string &name) { m_name = name; };

	/** �ΰ� �̸� ��ȯ �Լ�. */
	std::string getName() { return m_name; };

	/** �α� ���� ���� �Լ�. */
	void setLevel(int level) { m_level = level; };

	/** �α� ���� ��ȯ �Լ�. */
	int getLevel() { return m_level; };

	/** LOG_LEVEL_DEBUG ��带 Ȱ��ȭ��Ű�� �Լ�. */
	void enableDebug() { if (getLevel() < LOG_LEVEL_DEBUG) setLevel(LOG_LEVEL_DEBUG); };

	/** LOG_LEVEL_DEBUG ��带 ��Ȱ��ȭ��Ű�� �Լ�. */
	void disableDebug() { if (getLevel() > LOG_LEVEL_DEBUG) setLevel(LOG_LEVEL_DEBUG-1); }

	/** LOG_LEVEL_DEBUG ��尡 Ȱ��ȭ �Ǿ����� ��ȯ�ϴ� �Լ� */
	bool isDebug() { return ((getLevel() >= LOG_LEVEL_DEBUG) ? true : false); };

	/** LOG_LEVEL_INFO ��带 Ȱ��ȭ��Ű�� �Լ�. */
	void enableInfo() { if (getLevel() < LOG_LEVEL_INFO) setLevel(LOG_LEVEL_INFO); };

	/** LOG_LEVEL_INFO ��带 ��Ȱ��ȭ��Ű�� �Լ�. */
	void disableInfo() { if (getLevel() > LOG_LEVEL_INFO) setLevel(LOG_LEVEL_INFO-1); }

	/** LOG_LEVEL_INFO ��尡 Ȱ��ȭ �Ǿ����� ��ȯ�ϴ� �Լ� */
	bool isInfo() { return ((getLevel() >= LOG_LEVEL_INFO) ? true : false); };

	/** LOG_LEVEL_WARN ��带 Ȱ��ȭ��Ű�� �Լ�. */
	void enableWarn() { if (getLevel() < LOG_LEVEL_WARN) setLevel(LOG_LEVEL_WARN); };

	/** LOG_LEVEL_WARN ��带 ��Ȱ��ȭ��Ű�� �Լ�. */
	void disableWarn() { if (getLevel() > LOG_LEVEL_WARN) setLevel(LOG_LEVEL_WARN-1); }

	/** LOG_LEVEL_WARN ��尡 Ȱ��ȭ �Ǿ����� ��ȯ�ϴ� �Լ� */
	bool isWarn() { return ((getLevel() >= LOG_LEVEL_WARN) ? true : false); };

	/** LOG_LEVEL_ERROR ��带 Ȱ��ȭ��Ű�� �Լ�. */
	void enableError() { if (getLevel() < LOG_LEVEL_ERROR) setLevel(LOG_LEVEL_ERROR); };

	/** LOG_LEVEL_ERROR ��带 ��Ȱ��ȭ��Ű�� �Լ�. */
	void disableError() { if (getLevel() > LOG_LEVEL_ERROR) setLevel(LOG_LEVEL_ERROR-1); }

	/** LOG_LEVEL_ERROR ��尡 Ȱ��ȭ �Ǿ����� ��ȯ�ϴ� �Լ� */
	bool isError() { return ((getLevel() >= LOG_LEVEL_ERROR) ? true : false); };

	/** 
		�ΰŸ� open�ϴ� �Լ�. 
		���ڷ� ������ ������Ƽ���� �ΰ� ���� ������ ��� ����.
		���� ��� ���� �ΰ��� ��� ���� �̸��� ��� ����.

		@param props �ΰ� ȯ�� ���� ����

		@return bool ���� ����
	*/
	virtual	bool open(Properties &props);

	/** �ΰ� close �Լ�. */
	virtual void close();

	/** �α� ������ ��Ʈ������ ��ȯ�Ͽ� ��ȯ�ϴ� �Լ�. */
	virtual std::string levelToStr(int level);

	/** ��Ʈ���� �α� ������ ��ȯ�ϴ� �Լ�. */
	virtual int strToLevel(const std::string &str);

	/** �α� �������� ����Ͽ� ����� �޽����� ����� ��ȯ�ϴ� �Լ�. */
	virtual std::string getLogHeader(int level);

	/** LOG_LEVEL_DEBUG �� �α� ��Ʈ�� ��ȯ �Լ�. */
	std::ostream &debug();

	/** LOG_LEVEL_INFO �� �α� ��Ʈ�� ��ȯ �Լ�. */
	std::ostream &info();

	/** LOG_LEVEL_WARN �� �α� ��Ʈ�� ��ȯ �Լ�. */
	std::ostream &warn();

	/** LOG_LEVEL_ERROR �� �α� ��Ʈ�� ��ȯ �Լ�. */
	std::ostream &error();

	/** �α� ������ level�� �α� �޽����� ����� ��Ʈ�� ��ȯ �Լ�. */
	std::ostream &log(int level);

	/** 
		LOG_LEVEL_DEBUG �� �α� ��Ʈ�� ��ȯ �Լ�. 
		���ڷ� ������ filename, fnname, line ��ȣ�� �α� �޽��� ����� �����.
	*/
	std::ostream &debug(const std::string &filename, const std::string &fnname, int line);

	/** 
		LOG_LEVEL_INFO �� �α� ��Ʈ�� ��ȯ �Լ�. 
		���ڷ� ������ filename, fnname, line ��ȣ�� �α� �޽��� ����� �����.
	*/
	std::ostream &info(const std::string &filename, const std::string &fnname, int line);

	/** 
		LOG_LEVEL_WARN �� �α� ��Ʈ�� ��ȯ �Լ�. 
		���ڷ� ������ filename, fnname, line ��ȣ�� �α� �޽��� ����� �����.
	*/
	std::ostream &warn(const std::string &filename, const std::string &fnname, int line);

	/** 
		LOG_LEVEL_ERROR �� �α� ��Ʈ�� ��ȯ �Լ�. 
		���ڷ� ������ filename, fnname, line ��ȣ�� �α� �޽��� ����� �����.
	*/
	std::ostream &error(const std::string &filename, const std::string &fnname, int line);

	/** 
		�α� ������ level�� �α� �޽����� ����� �α� ��Ʈ�� ��ȯ �Լ�. 
		���ڷ� ������ filename, fnname, line ��ȣ�� �α� �޽��� ����� �����.
	*/
	std::ostream &log(int level, const std::string &filename, const std::string &fnname, int line);
};


#endif 
