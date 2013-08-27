/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Trace.h
 *
 */

#ifndef _CTRACE_H_
#define _CTRACE_H_

#include <iostream>
#include <string>

//
//#define CTRACE_ON
// #define TRACE_OFF

// defined in Registry.h
//extern bool c_trace;

extern bool c_trace;

class CBlockTracer {
	std::string m_file;
	std::string m_func;
	int m_line;

public :
	CBlockTracer(const std::string &file, const std::string &func, int line) {
		m_file = file;
		m_func = func;
		m_line = line;
//		if (c_trace)
//			std::cout << "!TRACE-Func-ENTER(" << m_file << ',' << m_func << ',' << m_line << ")% enter " << std::endl;
	};

	CBlockTracer(const std::string &file, const std::string &func, int line, std::string str) {
		m_file = file;
		m_func = func; 
		m_line = line;
//		if (c_trace)
//			std::cout << "!TRACE-Func-ENTER(" << m_file << ',' << m_func << ',' << m_line << ")% enter : " << str << std::endl;
	};

	void test() {};

	~CBlockTracer() {
		if (c_trace) {
//			OSGuard g(&c_traceMutex);
			std::cout << "!TRACE-Func-EXIT(" << m_file << ',' << m_func << ',' << m_line << ")% exit" << std::endl;
		}
	};
};

#ifdef CTRACE_ON

/*
#define trace_enter() if (c_trace) { OSGuard g(&c_traceMutex); std::cout << "!TRACE-Func-ENTER(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% enter " << std::endl;} BlockTracer opros_block_tracer(__FILE__, __FUNCTION__ , __LINE__)
#define trace_enter_msg(str) if (c_trace) { OSGuard g(&c_traceMutex); std::cout << "!TRACE-Func-ENTER(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% enter : " << str << std::endl; } BlockTracer opros_block_tracer(__FILE__, __FUNCTION__ , __LINE__)

#define trace(str) if (c_trace) { OSGuard g(&c_traceMutex);  std::cout << "%TRACE-INFO(" << __FILE__ << ',' << __LINE__ << ")% " << str << std::endl; } std::cout << ""
#define trace_info(str) if (c_trace) { OSGuard g(&c_traceMutex);  std::cout << "%TRACE-INFO(" << __FILE__ << ',' << __LINE__ << ")% " << str << std::endl; } std::cout << ""
#define trace_error(str) if (c_trace) { OSGuard g(&c_traceMutex);  std::cout << "%***TRACE-ERROR***(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; } std::cout << ""
#define trace_ex(ex) if (c_trace) { OSGuard g(&c_traceMutex);  ex.print(); } std::cout << ""
#define trace_cfg(cfg) if (c_trace)  { OSGuard g(&c_traceMutex); cfg->print(); } std::cout << ""
*/
#define ctrace_enter() if (c_trace) { std::cout << "!TRACE-Func-ENTER(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% enter " << std::endl;} CBlockTracer opros_block_tracer(__FILE__, __FUNCTION__ , __LINE__)
#define ctrace_enter_msg(str) if (c_trace) {  std::cout << "!TRACE-Func-ENTER(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% enter : " << str << std::endl; } CBlockTracer opros_block_tracer(__FILE__, __FUNCTION__ , __LINE__)

#define ctrace(str) if (c_trace) {  std::cout << "%TRACE-INFO(" << __FILE__ << ',' << __LINE__ << ")% " << str << std::endl; } std::cout << ""
#define ctrace_info(str) if (c_trace) {   std::cout << "%TRACE-INFO(" << __FILE__ << ',' << __LINE__ << ")% " << str << std::endl; } std::cout << ""
#define ctrace_error(str) if (c_trace) {   std::cout << "%***TRACE-ERROR***(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; } std::cout << ""
#define ctrace_ex(ex) if (c_trace) {  ex.print(); } std::cout << ""
#define ctrace_cfg(cfg) if (c_trace)  {  cfg->print(); } std::cout << ""

#else
	#define ctrace_enter()
	#define ctrace(str)
	#define ctrace_info(str)
	#define ctrace_error(str)
	#define ctrace_ex(ex)
	#define ctrace_cfg(cfg)
#endif

#endif /* _CTRACE_H_ */
