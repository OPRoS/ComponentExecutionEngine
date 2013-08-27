/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSocketTcp.h
 *
 */

#ifndef _OS_SOCKET_TCP_H_
#define _OS_SOCKET_TCP_H_

#include <string>
#include "OSSocketIp.h"

class OSSocketTcpAddress : public OSSocketAddress
{
public:
	std::string ip;
	int port;

	OSSocketTcpAddress() : port(-1) {
		family = AF_INET;
		type = SOCK_STREAM;
		protocol = 0;
		port = -1;
	};
	virtual ~OSSocketTcpAddress() {};
};

class OSTcpSocketOptions : public OSSocketOptions
{
public :
	int  backlog;

	OSTcpSocketOptions() : backlog(SOMAXCONN) {};
	virtual ~OSTcpSocketOptions() {};
};

/**
	TCP 소켓 관련 함수 래핑 클래스.
*/
class OSSocketTcp: public OSSocketIp
{
protected:
	
	OSSocketTcpAddress m_addr;
	OSTcpSocketOptions m_options;

	long m_connectTimeout;

public:
	OSSocketTcp();
	virtual ~OSSocketTcp();

	virtual OSSocketAddress *getSocketAddress() { return &m_addr; };
	virtual OSSocketOptions *getSocketOptions() { return &m_options; };;
	virtual OSSocketTcpAddress *getSocketTcpAddress() { return &m_addr; };

	virtual bool open(Properties &props);
	virtual bool close();

	virtual bool bind();
	virtual bool listen();
	virtual OSSocket *accept();
	virtual bool connect();

	virtual int write(const char *buf, size_t len, int flags = 0);
	virtual int read(char *buf, size_t len, int flags = 0);

	virtual std::string makeId(OSSocketTcpAddress &addr);

protected :
	virtual bool connect_blocking();
	virtual bool connect_nonblocking();
};

#endif /* _OS_SOCKET_TCP_H_ */
