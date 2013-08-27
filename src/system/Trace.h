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

#ifndef _TRACE_H_
#define _TRACE_H_

#include <iostream>
#include <sstream>
#include <string>

// =============================================
//	Trace Macros
// =============================================
#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"

#define SYSTEM_TRACE_ON

// defined in Trace.cpp
extern bool g_isTrace;

// defined in Log.cpp for synchronizing trace messages
extern OSMutex g_logMutex;

//---------------------------
// if trace on
//
#ifdef SYSTEM_TRACE_ON

	class SystemBlockTracer {
		std::string m_func;
		int m_line;

	public :
		SystemBlockTracer(const std::string &func, int line, bool flag = true) {
			m_func = func;
			m_line = line;
			if (g_isTrace && flag) {
				OSGuard g(&g_logMutex);
				std::cout << "%TRACE-ENTER(" << m_func << ',' << m_line << ")%" << std::endl;
			}
		};

		SystemBlockTracer(const std::string &func, int line, std::string str, bool flag = true) {
			m_func = func;
			m_line = line;
			if (g_isTrace && flag) {
				OSGuard g(&g_logMutex);
				std::cout << "%TRACE-ENTER(" << m_func << ',' << m_line << ")%" << str << std::endl;
			}
		};

		void test() {};

		~SystemBlockTracer() {
			if (g_isTrace) {
				OSGuard g(&g_logMutex);
				std::cout << "%TRACE-EXIT(" << m_func << ',' << m_line << ")%" << std::endl;
			}
		};
	};

	#define trace_enter() SystemBlockTracer opros_block_tracer(__FUNCTION__ , __LINE__)
	#define trace_enter_msg(str) if (g_isTrace) { OSGuard g(&g_logMutex); std::cout << "%TRACE-ENTER(" << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; } SystemBlockTracer opros_block_tracer(__FUNCTION__ , __LINE__, false)

	#define trace(str) if (g_isTrace) { OSGuard g(&g_logMutex);  std::cout << "%TRACE-INFO(" << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; }
	#define trace_info(str) if (g_isTrace) { OSGuard g(&g_logMutex);  std::cout << "%TRACE-INFO(" << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; }
	#define trace_error(str) if (g_isTrace) { OSGuard g(&g_logMutex);  std::cout << "%***TRACE-ERROR***(" << __FUNCTION__ << ',' << __LINE__ << ")% " << str << std::endl; }
#define trace_ex(ex) if (g_isTrace) { OSGuard g(&g_logMutex);  std::cout << ex; }
	#define trace_cfg(cfg) if (g_isTrace)  { OSGuard g(&g_logMutex); cfg->print(); }

#else
//---------------------------
// if trace off
//

	#define trace_enter()
	#define trace_enter_msg(str)
	#define trace(str)
	#define trace_info(str)
	#define trace_error(str)
	#define trace_ex(ex)
	#define trace_cfg(cfg)
#endif

#endif
