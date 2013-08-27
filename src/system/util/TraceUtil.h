/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TraceUtil.h
 *
 */

#ifndef _TRACE_UTIL_H_
#define _TRACE_UTIL_H_

#include <iostream>
#include <string>

// =============================================
//	Trace Macros
// =============================================

#ifdef TRACE_UTIL_ON

class BlockTracerUtil {
	std::string m_file;
	std::string m_func;
	int m_line;

public :
	BlockTracerUtil(const std::string &file, const std::string &func, int line) {
		m_file = file;
		m_func = func;
		m_line = line;
//		if (g_trace)
//			std::cout << "!!! TRACE-ENTER(" << m_file << ',' << m_func << ',' << m_line << ")% " << std::endl;
	};

	BlockTracerUtil(const std::string &file, const std::string &func, int line, std::string str) {
		m_file = file;
		m_func = func;
		m_line = line;
//		if (g_trace)
//			std::cout << "!!! TRACE-ENTER(" << m_file << ',' << m_func << ',' << m_line << ")% " << str << std::endl;
	};

	~BlockTracerUtil() {
		std::cout << "!!! TRACE-EXIT(" << m_file << ',' << m_func << ',' << m_line << ")% " << std::endl;
	};
};

#define tx_enter() std::cout << "!!! TRACE-ENTER(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% " << std::endl; BlockTracerUtil tx_block_tracer(__FILE__, __FUNCTION__ , __LINE__)
#define tx_enter_msg(str) std::cout << "!!! TRACE-ENTER(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; BlockTracerUtil tx_block_tracer(__FILE__, __FUNCTION__ , __LINE__)

#define tx(str) std::cout << "%TRACE(" << __FILE__ << ',' << __LINE__ << ")% " << str << std::endl
#define tx_info(str) std::cout << "%TRACE(" << __FILE__ << ',' << __LINE__ << ")% " << str << std::endl
#define tx_warn(str) std::cout << "%***TRACE-WARN***(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl
#define tx_error(str) std::cout << "%***TRACE-ERROR***(" << __FILE__ << ',' << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl
#define tx_ex(ex) ex.print()

#else
	#define tx_enter(str)
	#define tx(str)
	#define tx_warn(str) 
	#define tx_info(str)
	#define tx_error(str)
	#define tx_ex(ex)
#endif

#endif
