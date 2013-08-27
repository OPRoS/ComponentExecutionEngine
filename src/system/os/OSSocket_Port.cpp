/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 6
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSocket.cpp
 *
 */

#include "OSSocket.h"
#include "system/Trace.h"

OSSocketHandle OSSocket::createSocket(int af, int at, int ap)
{
	OSSocketHandle handle = ::socket(af, at, ap);

	if (handle == INVALID_SOCKET) {
		trace_error("socket : errno=" << Errno << "; msg=" << StrError);
	}

	int opt = 1; 
	if( setsockopt( handle, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt) ) != 0 ) {
		trace_error("socket: errorno=" << Errno <<"; msg=" << StrError); 
	}

	return handle;
}

#ifdef OS_WIN32 // OS_WIN32 ----------------------------------

WSAInitializer m_winsock_init;

bool OSSocket::setNonBlocking(bool nb)
{
	if (!isValid()) return false;

	OSSocketHandle handle = getSocketHandle();

	unsigned long l = nb ? 1 : 0;
	int n = ioctlsocket(handle, FIONBIO, &l);
	if (n != 0) {
		trace_error("ioctlsocket(FIONBIO) : errorno=" << Errno);
		nb = false;
	}
	else {
		nb = true;
	}
	getSocketOptions()->nonBlocking = nb;
	return nb;
}

bool OSSocket::setOpt(int level, int optName, const void *optValue, OSSocketAddrLen optLen)
{
	if (!isValid()) return false;

	if (::setsockopt(getSocketHandle(), level, optName, (const char *) optValue, optLen) == 0) return true;
	else return false;
}

bool OSSocket::getOpt(int level, int optName, void *optValue, OSSocketAddrLen *optLen)
{
	if (!isValid()) return false;

	if (::getsockopt(getSocketHandle(), level, optName, (char *) optValue, optLen) == 0) return true;
	else return false;
}

void OSSocket::setMemoryToZero(char *buf, size_t size)
{
	ZeroMemory(buf, size);
}

void OSSocket::closeHandle(OSSocketHandle &handle)
{
	::closesocket(handle);
}

#else // OS_LINUX -------------------------------------


bool OSSocket::setNonBlocking(bool nb)
{
	if (!isValid()) return false;

	OSSocketHandle handle = getSocketHandle();

	if (nb) {
		if (fcntl(handle, F_SETFL, O_NONBLOCK) == -1) {
			trace_error("fcntl(F_SETFL, O_NONBLOCK) : errno=" << Errno << "; msg=" << StrError);
			nb = false;
		}
	}
	else {
		if (fcntl(handle, F_SETFL, 0) == -1) {
			trace_error("fcntl(F_SETFL, 0) : errno=" << Errno << "; msg=" << StrError);
			nb = false;
		}
		else {
			nb = true;
		}
	}

	getSocketOptions()->nonBlocking = nb;
	return nb;
}

bool OSSocket::setOpt(int level, int optName, const void *optValue, OSSocketAddrLen optLen)
{
	if (!isValid()) return false;

	if (setsockopt(getSocketHandle(), level, optName, optValue, optLen) == 0) return true;
	else return false;
}

bool OSSocket::getOpt(int level, int optName, void *optValue, OSSocketAddrLen *optLen)
{
	if (!isValid()) return false;

	if (getsockopt(getSocketHandle(), level, optName, optValue, optLen) == 0) return true;
	else return false;
}

void OSSocket::setMemoryToZero(char *buf, size_t size)
{
	bzero(buf, size);
}

void OSSocket::closeHandle(OSSocketHandle &handle)
{
	::close(handle);
}

#endif
