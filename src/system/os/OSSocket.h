/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSocket.h
 *
 */

#ifndef _OS_SOCKET_H
#define _OS_SOCKET_H

#include <string>
#include <iostream>

#include "OSSetting.h"
#include "system/util/Properties.h"


#ifdef OS_WIN32

// class WSAInitializer is a part of the Socket class (on win32)
// as a static instance - so whenever an application uses a Socket,
// winsock is initialized
class WSAInitializer // Winsock Initializer
{
public:
	WSAInitializer() {
		if (WSAStartup(0x101,&m_wsadata))
		{
			exit(-1);
		}
	}
	~WSAInitializer() {
		WSACleanup();
	}
private:
	WSADATA m_wsadata;
};

#define Errno GetLastError()
#define StrError strerror(Errno)

typedef SOCKET OSSocketHandle;
typedef int			OSSocketAddrLen;

#else // -------------  Linux/POSIX

// common unix includes / defines
#include <cstdlib>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

#define Errno errno
#define StrError strerror(errno)

typedef OSHandle	OSSocketHandle;
typedef socklen_t	OSSocketAddrLen;

#endif

class OSSocketAddress
{
public:
	int family;
	int type;
	int protocol;

	OSSocketAddress() : family(-1), type(-1), protocol(-1) {};
	virtual ~OSSocketAddress() {};
};

class OSSocketOptions
{
public:
	bool nonBlocking;

	OSSocketOptions() : nonBlocking(false) {};
	virtual ~OSSocketOptions() {};
};

/**
	소켓 관련 함수 래핑 클래스.
*/
class OSSocket
{
	OSSocketHandle m_handle;
	std::string m_id;

public:
	OSSocket() : m_handle(INVALID_SOCKET) {};
	virtual ~OSSocket() {};

	friend std::ostream & operator<<(std::ostream & os, OSSocket &addr) {
		os << "OSSocket.id=" << addr.getId();
		return os;
	};

	virtual OSSocketHandle getSocketHandle() { return m_handle; }
	virtual void setSocketHandle(OSSocketHandle handle) { m_handle = handle; }
	bool isValid() { return getSocketHandle() != INVALID_SOCKET; };
	bool isValid(OSSocketHandle h) { return h != INVALID_SOCKET; };

	void setId(std::string id) { m_id = id; };
	std::string getId() { return m_id; }

	virtual OSSocketAddress *getSocketAddress() = 0;
	virtual OSSocketOptions *getSocketOptions() = 0;

	void setMemoryToZero(char *buf, size_t size);
	int getError();
	std::string getErrorMsg();

	virtual bool setNonBlocking(bool nb);
	virtual bool isNonBlocking() { return getSocketOptions()->nonBlocking; };
	virtual bool setOpt(int level, int optName, const void *optValue, OSSocketAddrLen  optLen);
	virtual bool getOpt(int level, int optName, void *optValue, OSSocketAddrLen *optLen);

	virtual bool open(Properties &props) = 0;
	virtual bool close() = 0;

	OSSocketHandle createSocket(int af, int at, int ap);
	void closeHandle(OSSocketHandle &handle);

	virtual bool bind() { return true; };
	virtual bool listen() { return true; };
	virtual OSSocket *accept() { return NULL; };
	virtual bool connect() { return true; };

	virtual int write(const char *buf, size_t len, int flags = 0) { return -1; };
	virtual int read(char *buf, size_t len, int flags = 0) { return -1; };
};

#endif
