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

/** 디폴트 로그 레벨 타입 */
enum { 	LOG_LEVEL_NONE = 0,
		LOG_LEVEL_ERROR = 100,
		LOG_LEVEL_WARN 	= 200,
		LOG_LEVEL_INFO 	= 300,
		LOG_LEVEL_DEBUG = 400,
};


/**
	로그 정보를 기록할 로거 클래스.
*/
class Logger : public StringStream
{
	/** 로거 이름 */
	std::string m_name;

	/** 로그 레벨 */
	int m_level;

	/** 
		출력하고자 하는 로그 레벨이 세팅된 로글 레벨보다 낮은 경우
		해당 메시지는 출력하지 않아야 한다.
		이를 위해 NULL 스트림 객체를 사용한다.
	*/
	NullStream    m_nullStream;

	/** 로그 셋업 함수. */
	void setup();

public:
	Logger();
	virtual ~Logger();

	/** 로거 이름 세팅 함수. */
	void setName(const std::string &name) { m_name = name; };

	/** 로거 이름 반환 함수. */
	std::string getName() { return m_name; };

	/** 로그 레벨 세팅 함수. */
	void setLevel(int level) { m_level = level; };

	/** 로그 레벨 반환 함수. */
	int getLevel() { return m_level; };

	/** LOG_LEVEL_DEBUG 모드를 활성화시키는 함수. */
	void enableDebug() { if (getLevel() < LOG_LEVEL_DEBUG) setLevel(LOG_LEVEL_DEBUG); };

	/** LOG_LEVEL_DEBUG 모드를 비활성화시키는 함수. */
	void disableDebug() { if (getLevel() > LOG_LEVEL_DEBUG) setLevel(LOG_LEVEL_DEBUG-1); }

	/** LOG_LEVEL_DEBUG 모드가 활성화 되었는지 반환하는 함수 */
	bool isDebug() { return ((getLevel() >= LOG_LEVEL_DEBUG) ? true : false); };

	/** LOG_LEVEL_INFO 모드를 활성화시키는 함수. */
	void enableInfo() { if (getLevel() < LOG_LEVEL_INFO) setLevel(LOG_LEVEL_INFO); };

	/** LOG_LEVEL_INFO 모드를 비활성화시키는 함수. */
	void disableInfo() { if (getLevel() > LOG_LEVEL_INFO) setLevel(LOG_LEVEL_INFO-1); }

	/** LOG_LEVEL_INFO 모드가 활성화 되었는지 반환하는 함수 */
	bool isInfo() { return ((getLevel() >= LOG_LEVEL_INFO) ? true : false); };

	/** LOG_LEVEL_WARN 모드를 활성화시키는 함수. */
	void enableWarn() { if (getLevel() < LOG_LEVEL_WARN) setLevel(LOG_LEVEL_WARN); };

	/** LOG_LEVEL_WARN 모드를 비활성화시키는 함수. */
	void disableWarn() { if (getLevel() > LOG_LEVEL_WARN) setLevel(LOG_LEVEL_WARN-1); }

	/** LOG_LEVEL_WARN 모드가 활성화 되었는지 반환하는 함수 */
	bool isWarn() { return ((getLevel() >= LOG_LEVEL_WARN) ? true : false); };

	/** LOG_LEVEL_ERROR 모드를 활성화시키는 함수. */
	void enableError() { if (getLevel() < LOG_LEVEL_ERROR) setLevel(LOG_LEVEL_ERROR); };

	/** LOG_LEVEL_ERROR 모드를 비활성화시키는 함수. */
	void disableError() { if (getLevel() > LOG_LEVEL_ERROR) setLevel(LOG_LEVEL_ERROR-1); }

	/** LOG_LEVEL_ERROR 모드가 활성화 되었는지 반환하는 함수 */
	bool isError() { return ((getLevel() >= LOG_LEVEL_ERROR) ? true : false); };

	/** 
		로거를 open하는 함수. 
		인자로 들어오는 프로퍼티에는 로거 설정 정보가 들어 있음.
		예를 들어 파일 로거의 경우 파일 이름이 들어 있음.

		@param props 로거 환경 설정 정보

		@return bool 성공 여부
	*/
	virtual	bool open(Properties &props);

	/** 로거 close 함수. */
	virtual void close();

	/** 로그 레벨을 스트링으로 변환하여 반환하는 함수. */
	virtual std::string levelToStr(int level);

	/** 스트링을 로그 레벨로 변환하는 함수. */
	virtual int strToLevel(const std::string &str);

	/** 로그 레벨등을 고려하여 출력할 메시지의 헤더를 반환하는 함수. */
	virtual std::string getLogHeader(int level);

	/** LOG_LEVEL_DEBUG 용 로그 스트림 반환 함수. */
	std::ostream &debug();

	/** LOG_LEVEL_INFO 용 로그 스트림 반환 함수. */
	std::ostream &info();

	/** LOG_LEVEL_WARN 용 로그 스트림 반환 함수. */
	std::ostream &warn();

	/** LOG_LEVEL_ERROR 용 로그 스트림 반환 함수. */
	std::ostream &error();

	/** 로그 레벨이 level인 로그 메시지를 출력할 스트림 반환 함수. */
	std::ostream &log(int level);

	/** 
		LOG_LEVEL_DEBUG 용 로그 스트림 반환 함수. 
		인자로 들어오는 filename, fnname, line 번호를 로그 메시지 헤더에 출력함.
	*/
	std::ostream &debug(const std::string &filename, const std::string &fnname, int line);

	/** 
		LOG_LEVEL_INFO 용 로그 스트림 반환 함수. 
		인자로 들어오는 filename, fnname, line 번호를 로그 메시지 헤더에 출력함.
	*/
	std::ostream &info(const std::string &filename, const std::string &fnname, int line);

	/** 
		LOG_LEVEL_WARN 용 로그 스트림 반환 함수. 
		인자로 들어오는 filename, fnname, line 번호를 로그 메시지 헤더에 출력함.
	*/
	std::ostream &warn(const std::string &filename, const std::string &fnname, int line);

	/** 
		LOG_LEVEL_ERROR 용 로그 스트림 반환 함수. 
		인자로 들어오는 filename, fnname, line 번호를 로그 메시지 헤더에 출력함.
	*/
	std::ostream &error(const std::string &filename, const std::string &fnname, int line);

	/** 
		로그 레벨이 level인 로그 메시지를 출력할 로그 스트림 반환 함수. 
		인자로 들어오는 filename, fnname, line 번호를 로그 메시지 헤더에 출력함.
	*/
	std::ostream &log(int level, const std::string &filename, const std::string &fnname, int line);
};


#endif 
