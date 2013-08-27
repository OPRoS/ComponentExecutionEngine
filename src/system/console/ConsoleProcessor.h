/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ConsoleProcessor.h
 *
 */

#ifndef _CONSOLE_PROCESSOR_H_
#define _CONSOLE_PROCESSOR_H_

#include <string>
#include <iostream>
#include <vector>

/**
	컴포넌트 실행엔진 커맨드 처리 클래스. 
 */
class ConsoleProcessor
{
	/** 디폴트 응용 이름 */
	std::ostream *m_os;
	std::istream *m_is;

	void printStr(const std::string &str, int size);

public:
	ConsoleProcessor(std::ostream *os = &std::cout, std::istream *is = &std::cin);
	~ConsoleProcessor();

	void setIStream(std::istream *is) { m_is = is; };
	void setOStream(std::ostream *os) { m_os = os; };

	/** 
		컴포넌트 실행엔진에 대한 콘솔 명령을 처리하는 함수.
		콘솔에서 명령을 입력 받아 처리함.
	*/
	void processConsoleCommand();

	/** 
		컴포넌트 실행엔진에 대한 명령을 처리하는 함수.
		processCommand() 함수에서 호출함.

		@param cmd 처리할 커맨드

		@return bool 콘솔 화면 종료 여부
	*/
	bool doCommand(const std::string &cmd);

	/** 
		커맨드에 대한 help 명령 처리 함수 
	*/
	bool printHelp();

	/** 
		컴포넌트 실행엔진 관련 커맨드(init, start, stop) 처리 함수 

		@param list 처리할 커맨드 및 인자 값

		@return bool 콘솔 화면 종료 여부
	*/
	bool processEngine(std::vector<std::string> &list);

	/** 
		컴포넌트 관련 커맨드 처리 함수 

		@param list 처리할 커맨드 및 인자 값

		@return bool 콘솔 화면 종료 여부
	*/
	bool processComponent(std::vector<std::string> &list);

	/** 
		모니터링 관련 커맨드 처리 함수 

		@param list 처리할 커맨드 및 인자 값

		@return bool 콘솔 화면 종료 여부
	*/
	bool processMonitor(const std::string &cmd);

	/** 
		로봇 응용 관련 커맨드(응용 배포, 시작, 중지 등) 처리 함수 

		@param list 처리할 커맨드 및 인자 값

		@return bool 콘솔 화면 종료 여부
	*/
	bool processApp(std::vector<std::string> &list);
};

#endif  
