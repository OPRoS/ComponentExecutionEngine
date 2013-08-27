/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoDirectWorker.cpp
 *
 */

#include "IoDirectWorker.h"

#include "IoDirectProtocol.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoDirectWorker::IoDirectWorker(IoConnection *cnn)
{
	m_cnn = cnn;
}

IoDirectWorker::~IoDirectWorker()
{
	trace_enter();

	if (m_cnn != NULL) {
		m_cnn->close();
		delete m_cnn;
	}
}

void IoDirectWorker::startWork()
{
	trace_enter();

	setOnFinished(false);

	// worker 쓰레드 시작
	threadStart();
}

void IoDirectWorker::stopWork()
{
	trace_enter();

	// 커넥션 close
	if (m_cnn != NULL) {
		m_cnn->close();
	}
}


void IoDirectWorker::run()
{
	//------------------------
	// processing the work

	try {
		if (m_cnn != NULL) processWork();
		else {
			log_error("IoConnection is NULL. Worker Stopped.");
		}
	}
	catch (IOException &ex) {
		log_info("Connection closed :"+ex.getMessage());
	}
}

int IoDirectWorker::write(const byte *buf, size_t len) throw (IOException)
{
	if (m_cnn == NULL) throw IOException("IoConnection is NULL");

	int r = m_cnn->write(buf, len);
	if (r < 0) throw IOException("IoConnect write error");
	else if (r==0)
	{
		throw IOException("IoConnection closed by peer");
	}

	return r;
}

int IoDirectWorker::read(byte *buf, size_t len) throw (IOException)
{
	if (m_cnn == NULL) throw IOException("IoConnection is NULL");

	int r = m_cnn->read(buf, len);
	if (r < 0) {
//		std::cerr <<"Error Code is "<< ::WSAGetLastError();
		throw IOException("IoConnect read error");
	}
	else if (r==0)
	{
		throw IOException("IoConnection closed by peer");
	}

	return r;
}

int IoDirectWorker::writeString(const string &str) throw (IOException)
{
	return writeString(str.c_str(), str.size());
}

int IoDirectWorker::writeString(const char *buf, size_t len) throw (IOException)
{
	return write((const byte *) buf, len);
}

int IoDirectWorker::readLine(char *buf, size_t len) throw (IOException)
{
	size_t readPos;

	char ch;
	bool isEOL;

	isEOL = false;
	readPos = 0;
	while (readPos < len-1) {
		read((byte *) &ch, 1);
		if (ch == '\r' || ch == '\n') break;
		buf[readPos++] = ch;		
	}
	buf[readPos] = '\0';

	trace("len=" << readPos << ";buf = " << buf);

	return readPos;
}
