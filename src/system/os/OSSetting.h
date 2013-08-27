/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 10
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSetting.h
 *
 */

#ifndef _OS_SETTING_H_
#define _OS_SETTING_H_

#include "system/util/BasicTypes.h"

// =============================================
//
//	OS Portable Macros
//
// =============================================
// #define OS_POSIX

#ifdef WIN32 // Win32  -------------------------------

#define OS_WIN32

//#include "stdafx.h"
#include <windows.h>
#include <winsock.h>
typedef HANDLE OSHandle;
typedef DWORD ThreadFunctionReturn;

#else //  Linux/POSIX -------------------------------

#define OS_LINUX
#define OS_POSIX
#define OS_INVALID_HANDLE -1
typedef int OSHandle;
typedef void* ThreadFunctionReturn;

#endif // ------------- end macros


// =============================================
//
//	Other Macros
//
// =============================================
# ifdef __cplusplus
extern "C"
{
# endif  /* __cplusplus */

typedef ThreadFunctionReturn (*ThreadFunction)(void *);
typedef void (*AIOCallBack)(int ,int , void *);

# ifdef __cplusplus
}
# endif  /* __cplusplus */

#endif /* _OS_SETTING_H_ */
