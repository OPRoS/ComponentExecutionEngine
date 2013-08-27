/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSSocketTcp.cpp
 *
 */

#include "OSSocketTcp.h"

#include "system/util/BasicTypes.h"
#include "system/util/StringUtil.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "OSSocket.h"

using namespace std;

OSSocketTcp::OSSocketTcp()
{
}

OSSocketTcp::~OSSocketTcp()
{
}

string OSSocketTcp::makeId(OSSocketTcpAddress &addr)
{
	string id("tcp://");
	id.append(addr.ip);
	id.append(":");
	id.append(StringUtil::intToStr(addr.port));
	return id;
}

bool OSSocketTcp::open(Properties &props)
{
	trace_enter();

	// already opened
	if (isValid()) return true;

	//
	// --- create socket
	OSSocketHandle handle;

	// AF_INET,
	handle = createSocket(m_addr.family, m_addr.type, m_addr.protocol);
	if (!isValid(handle)) {
		log_error("Cannot create SOCKET");
		return false;
	}


	setSocketHandle(handle);

#if _DEBUG
	int option = 1;
	setOpt(SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));	
#endif

	//
	// --- set Config
	//
	try {
		m_addr.ip = props.getProperty("ip.addr", "any");
		m_addr.port = props.getInt("ip.port", 0);

		m_options.nonBlocking = props.getBool("nonblocking", false);
		m_options.backlog = props.getInt("backlog", SOMAXCONN);

		setNonBlocking(m_options.nonBlocking);

		m_connectTimeout = props.getLong("connect.timeout", 200000); // default 200 msec
	}
	catch (Exception &e) {
		log_error("Error when configuring : ex = " << e.getMessage());
		close();
		return false;
	}

	setId(makeId(m_addr));

	log_info("TCP Socket Open OK... : id=" << getId());

	return true;
}

bool OSSocketTcp::close()
{
	OSSocketHandle h = getSocketHandle();
	if (isValid(h)) {
		closeHandle(h);
		setSocketHandle(INVALID_SOCKET);
	}

	return true;
}

bool OSSocketTcp::bind()
{
	trace_enter();

	if (!isValid()) {
		log_error("Invalid Socket Handle");
		return false;
	}

	sockaddr_in addr;
	setMemoryToZero((char *) &addr, sizeof(addr));
	addr.sin_family = m_addr.family;
	addr.sin_port = htons(m_addr.port);
	addr.sin_addr.s_addr = ipToAddr(m_addr.ip);
	if (addr.sin_addr.s_addr == 0) {
		log_error("Invalid IP Address");
		return false;
	}

	trace("address.ip=" << m_addr.ip << " - in.addr = " << addr.sin_addr.s_addr);
	trace("address.port=" << m_addr.port << " - in.port = " << addr.sin_port);

	int retVal = ::bind(getSocketHandle(), (sockaddr *) &addr, sizeof(addr));
	if (retVal == SOCKET_ERROR) {
		log_error("Socket bind error : ip="<<m_addr.ip <<";port="<<m_addr.port <<"; errno=" << Errno << "; msg=" << StrError);
		return false;
	}
	else {
		return true;
	}
}

bool OSSocketTcp::listen()
{
	trace_enter();

	if (!isValid()) {
		log_error("Invalid Socket Handle");
		return false;
	}

	int retVal = ::listen(getSocketHandle(), m_options.backlog);
	if (retVal == SOCKET_ERROR) {
		log_error("Socket listen error : errno=" << Errno << "; msg=" << StrError);
		return false;
	}
	else {
		return true;
	}
}

OSSocket *OSSocketTcp::accept()
{
	trace_enter();

	if (!isValid()) {
		log_error("Invalid Socket Handle");
		return NULL;
	}

	OSSocketHandle clientSocket;
	sockaddr_in clientAddr;
	OSSocketAddrLen addrLen = sizeof(clientAddr);
	clientSocket = ::accept(getSocketHandle(), (sockaddr *) &clientAddr, &addrLen);

	if (!isValid(clientSocket)) {
		log_error("Socket accept error : errno=" << Errno << "; msg=" << StrError);
		return NULL;
	}
	OSSocketTcp *tcpSock = new OSSocketTcp();
	if (tcpSock == NULL) return NULL;

	tcpSock->setSocketHandle(clientSocket);
	OSSocketTcpAddress *ipAddr = portable_dynamic_cast(OSSocketTcpAddress *, tcpSock->getSocketAddress());
	ipAddr->ip = string(inet_ntoa(clientAddr.sin_addr));
	ipAddr->port = (int) ntohs(clientAddr.sin_port);
	tcpSock->setId(makeId(*ipAddr));

	return tcpSock;
}

bool OSSocketTcp::connect()
{
	trace_enter();

	return connect_nonblocking();
}

bool OSSocketTcp::connect_blocking()
{
	trace_enter();

	if (!isValid()) {
		log_error("Invalid Socket Handle");
		return false;
	}

	sockaddr_in addr;
	setMemoryToZero((char *) &addr, sizeof(addr));
	addr.sin_family = m_addr.family;
	addr.sin_port = htons(m_addr.port);
	addr.sin_addr.s_addr = ipToAddr(m_addr.ip);
	if (addr.sin_addr.s_addr == 0) return false;

	int retVal = ::connect(getSocketHandle(), (sockaddr *) &addr, sizeof(addr));
	if (retVal == SOCKET_ERROR) {
		log_error("Socket connect error : errno=" << Errno << "; msg=" << StrError);
		return false;
	}
	else {
		return true;
	}
}

#ifdef OS_WIN32

#include <errno.h>

bool OSSocketTcp::connect_nonblocking()
{
	trace_enter();

	if (!isValid()) {
		log_error("Invalid Socket Handle");
		return false;
	}

	int    flags, n, error=0;
	fd_set rset, wset;

	struct timeval tval;
	tval.tv_sec = m_connectTimeout / 1000000L;
	tval.tv_usec = m_connectTimeout % 1000000L;

	OSSocketHandle sockfd = getSocketHandle();
	sockaddr_in addr;
	setMemoryToZero((char *) &addr, sizeof(addr));
	addr.sin_family = m_addr.family;
	addr.sin_port = htons(m_addr.port);
	addr.sin_addr.s_addr = ipToAddr(m_addr.ip);
	if (addr.sin_addr.s_addr == 0) return false;

    int nonblocking =1;
    ioctlsocket(sockfd, FIONBIO, (unsigned long*) &nonblocking);

	int retVal = ::connect(sockfd, (sockaddr *) &addr, sizeof(addr));
	if (retVal < 0) {
        errno = WSAGetLastError();
//        if (errno != EINPROGRESS && errno!= WSAWOULDBLOCK) {
        if (errno != WSAEINPROGRESS && errno!= WSAEWOULDBLOCK) {
            return false;
        }
	}

	/* Do whatever we want while the connect is taking place. */
	if (retVal != 0) {
		FD_ZERO(&rset);
		FD_SET(sockfd, &rset);
		wset = rset;

		if ( (n = select(sockfd+1, &rset, &wset, NULL,((tval.tv_sec>0) || (tval.tv_usec>0))? &tval : NULL)) == 0) {
			close();        /* timeout */
			errno = WSAETIMEDOUT;
			return false;
		}

		if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
			// do nothing
		} 
		else {
			return false; //err_quit("select error: sockfd not set");
		}
	}

    nonblocking =0;
    ioctlsocket(sockfd, FIONBIO, (unsigned long*) &nonblocking);

    if (error) {
        close(); /* just in case */
        errno = error;
        return false;
    }

    return true;
}

#else

bool OSSocketTcp::connect_nonblocking()
{
	trace_enter();

	if (!isValid()) {
		log_error("Invalid Socket Handle");
		return false;
	}

	int                flags, n, error;
	socklen_t        len;
	fd_set            rset, wset;

	struct timeval tval;
	tval.tv_sec = m_connectTimeout / 1000000L;
	tval.tv_usec = m_connectTimeout % 1000000L;

	OSSocketHandle sockfd = getSocketHandle();
	sockaddr_in addr;
	setMemoryToZero((char *) &addr, sizeof(addr));
	addr.sin_family = m_addr.family;
	addr.sin_port = htons(m_addr.port);
	addr.sin_addr.s_addr = ipToAddr(m_addr.ip);
	if (addr.sin_addr.s_addr == 0) return false;

    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);


	int retVal = ::connect(sockfd, (sockaddr *) &addr, sizeof(addr));
	if (retVal < 0) {
        if (errno != EINPROGRESS && errno!= EWOULDBLOCK) {
            return false;
        }
	}

	/* Do whatever we want while the connect is taking place. */

	if (retVal != 0) {
		FD_ZERO(&rset);
		FD_SET(sockfd, &rset);
		wset = rset;

		if ( (n = select(sockfd+1, &rset, &wset, NULL,((tval.tv_sec>0) || (tval.tv_usec>0))? &tval : NULL)) == 0) {
			::close(sockfd);        /* timeout */
			errno = ETIMEDOUT;
			return false;
		}

		if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
			len = sizeof(error);
			if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
				return false;            /* Solaris pending error */
		} else {
			return false; //err_quit("select error: sockfd not set");
		}
	}

    fcntl(sockfd, F_SETFL, flags);    /* restore file status flags */

    if (error) {
        ::close(sockfd);        /* just in case */
        errno = error;
        return false;
    }

    return true;
}

#endif

int OSSocketTcp::write(const char *buf, size_t len, int flags)
{
	if (!isValid()) {
		log_error("Socket is not valid : sock.id=" << getId());
		return -1;
	}

	return ::send(getSocketHandle(), buf, len, flags);
}

int OSSocketTcp::read(char *buf, size_t len, int flags)
{
	if (!isValid()) {
		log_error("Socket is not valid : sock.id=" << getId());
		return -1;
	}

	return ::recv(getSocketHandle(), buf, len, flags);
}



