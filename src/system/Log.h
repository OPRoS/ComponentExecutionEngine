/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Log.h
 *
 */

#ifndef _LOG_H_
#define _LOG_H_

#include "log/Logger.h"
#include "log/LoggerFactory.h"
#include "Registry.h"

#include "system/os/OSMutex.h"
#include "system/os/OSGuard.h"

// =============================================
//
//	Logging Macros
//
// =============================================

extern OSMutex g_logMutex;
extern bool g_logSync;

#define DEBUG_ON
#define INFO_ON
#define WARN_ON
#define ERROR_ON

#define LOG_SYNC
// #define LOG_DETAIL_OFF

#ifdef DEBUG_ON
	#define log_debug(str) if (g_logSync) { OSGuard g(&g_logMutex); Registry::getRegistry()->getSystemLogger()->debug(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; } \
							else { Registry::getRegistry()->getSystemLogger()->debug(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; }
	#define log_mydebug(logger, str) if (logger->isDebug()) if (g_logSync) { OSGuard g(&g_logMutex); logger->debug(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; } \
										else { logger->debug(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; }
#else
	#define log_debug(str)
	#define log_mydebug(logger, str)
#endif

#ifdef INFO_ON
	#define log_info(str) if (g_logSync) { OSGuard g(&g_logMutex); Registry::getRegistry()->getSystemLogger()->info(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; } \
							else { Registry::getRegistry()->getSystemLogger()->info(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; }
	#define log_myinfo(logger, str) if (logger->isInfo()) if (g_logSync) { OSGuard g(&g_logMutex); logger->info(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; } \
									else { logger->info(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; }
#else
	#define log_info(str)
	#define log_myinfo(logger, str)
#endif

#ifdef WARN_ON
	#define log_warn(str) if (g_logSync) { OSGuard g(&g_logMutex); Registry::getRegistry()->getSystemLogger()->warn(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; } \
							else { Registry::getRegistry()->getSystemLogger()->warn(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; }
	#define log_mywarn(logger, str) if (logger->isWarn()) if (g_logSync) { OSGuard g(&g_logMutex); logger->warn(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; } \
										else { logger->warn(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; }
#else
	#define log_warn(str)
	#define log_mywarn(logger, str)
#endif

#ifdef ERROR_ON
	#define log_error(str) if (g_logSync) { OSGuard g(&g_logMutex); Registry::getRegistry()->getSystemLogger()->error(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; } \
							else { Registry::getRegistry()->getSystemLogger()->error(__FILE__,__FUNCTION__ , __LINE__) << str << std::endl; }
	#define log_myerror(logger, str) if (logger->isError()) if (g_logSync) { OSGuard g(&g_logMutex); logger->error(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; } \
										else { logger->error(__FILE__,__FUNCTION__ , __LINE__)  << str << std::endl; }
#else
	#define log_error(str)
	#define log_myerror(logger, str)
#endif

#endif
