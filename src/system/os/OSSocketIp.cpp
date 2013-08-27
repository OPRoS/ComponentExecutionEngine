/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSocketIp.cpp
 *
 */


#include "OSSocketIp.h"

#ifdef OS_WIN32

#include <stdlib.h>

#else // OS_WIN32

#include <errno.h>
#include <netdb.h>

#endif

#include <cstring>
#include <ctype.h>
#include <fcntl.h>

using namespace std;

OSSocketIp::OSSocketIp()
{
}

OSSocketIp::~OSSocketIp()
{
}

ulong OSSocketIp::ipToAddr(const string &ip)
{
	ulong ipaddr;

	if (ip == "any") {
		ipaddr = INADDR_ANY;
	}
	else {
		ipaddr = inet_addr(ip.c_str());
	}

	if (ipaddr == INADDR_NONE) {
		return 0;
	}
	else {
		return ipaddr;
	}

}


