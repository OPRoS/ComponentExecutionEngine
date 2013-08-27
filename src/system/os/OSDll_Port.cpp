/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSDll.cpp
 *
 */

#include "OSDll.h"

#include "system/Log.h"
#include "system/Trace.h"

OSDll::OSDll() {
}

OSDll::~OSDll() {
}

#ifdef OS_WIN32

OSDllHandle OSDll::loadLibrary(const std::string &dllFile)
{
	return LoadLibraryA(dllFile.c_str());
}

ProcAddr OSDll::getProcAddress(OSDllHandle handle, const std::string &procName)
{
	return GetProcAddress(handle, procName.c_str());
}

bool OSDll::freeLibrary(OSDllHandle handle)
{
	return FreeLibrary(handle);
}

std::string OSDll::dllExt()
{
	return "dll";
}

#else

#include <dlfcn.h>
#include <iostream>

OSDllHandle OSDll::loadLibrary(const std::string &dllFile)
{
	void *handle = ::dlopen(dllFile.c_str(), RTLD_NOW); // | RTLD_GLOBAL | RTLD_NODELETE);
	if (!handle){
		log_error("Cannot open library: " << dlerror());
		return handle;
	}

	return handle;

}

ProcAddr OSDll::getProcAddress(OSDllHandle handle, const std::string &procName)
{
	return ::dlsym(handle, procName.c_str());

}

bool OSDll::freeLibrary(OSDllHandle handle)
{
	::dlclose(handle);
	return true;
}

std::string OSDll::dllExt()
{
	return "so";
}

#endif
