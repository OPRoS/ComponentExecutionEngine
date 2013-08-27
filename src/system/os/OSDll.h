/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSDll.h
 *
 */

#ifndef _OS_DLL_H_
#define _OS_DLL_H_

#include <string>

#include "OSSetting.h"
#include "OSMutex.h"

#ifdef OS_WIN32

#define INVALID_DLL NULL

typedef HINSTANCE OSDllHandle;
typedef FARPROC   ProcAddr;

#else // -------------  Linux/POSIX

#define INVALID_DLL NULL

typedef void *OSDllHandle;
typedef void *ProcAddr;

#endif

/**
	DLL 혹은 so 파일 관련 함수 래핑 클래스.
*/
class OSDll
{
public:
	OSDll();
	virtual ~OSDll();

	static OSDllHandle loadLibrary(const std::string &dllFile);
	static ProcAddr getProcAddress(OSDllHandle handle, const std::string &procName);
	static bool freeLibrary(OSDllHandle handle);

	static std::string dllExt();
};

#endif 
