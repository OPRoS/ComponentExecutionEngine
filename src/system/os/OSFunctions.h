/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSFunctions.h
 *
 */

#ifndef OS_FUNCTIONS_H_
#define OS_FUNCTIONS_H_

#include <string>

#include "OSSetting.h"
#include "OSTimeVal.h"

/**
	운영체제에 의존적인 유틸리티 함수 래핑 클래스.
*/
class OS
{
public :
	static int getLastError();
	static void sleepMS(uint32 dwMilliseconds);
	static char fileSeparator();
	static std::string toDir(const std::string &dir);

	static std::string  getFileName(const std::string & fullpath);
	static std::string  getDirName(const std::string & fullpath);
	static std::string  splitExt(const std::string  &filename);

	static int select(int nfds,	fd_set *rset, fd_set *wset,	fd_set *eset, long msTimeout);
};

#endif 
