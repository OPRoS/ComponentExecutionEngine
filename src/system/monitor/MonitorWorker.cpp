/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitorWorker.cpp
 *
 */


#include "MonitorWorker.h"

#include <iostream>

#include "MonitorProtocol.h"

#include "system/Log.h"
#include "system/Trace.h"

#define HEADER_LEN 4096

using namespace std;

MonitorWorker::MonitorWorker(MonitorProtocol *mp, IoConnection *cnn) : m_mp(mp), IoDirectWorker(cnn)
{
}

MonitorWorker::~MonitorWorker()
{
}

void MonitorWorker::processWork() throw (IOException)
{
	trace_enter();

	bool isDone = false;

	char line[HEADER_LEN];
	while (!isDone) {
		int len = readLine(line, HEADER_LEN);
		isDone = m_mp->processCommand(this, line, len);
	}
}

void MonitorWorker::returnFail(Properties &props, const string &ret) throw (IOException)
{
	string target = props.getProperty("target", "unknown");
	string cmd = props.getProperty("cmd", "unknown");

	returnFail(target, cmd, ret);
}

void MonitorWorker::returnFail(const string &target, const string &cmd, const string &ret) throw (IOException)
{
	trace_enter_msg(ret);

	string msg = "ver=1.0;target=" + target + ";cmd=" + cmd + ";success=fail;ret=" + ret + ";payloadSize=0\n";

	trace("returnFail ==> " << msg);
	writeString(msg);
}

void MonitorWorker::returnOK(Properties &props, const string &ret) throw (IOException)
{
	string target = props.getProperty("target", "unknown");
	string cmd = props.getProperty("cmd", "unknown");

	returnOK(target, cmd, ret);
}

void MonitorWorker::returnOK(Properties &props, const string &ret,const std::string &payload) throw (IOException)
{
	string target = props.getProperty("target", "unknown");
	string cmd = props.getProperty("cmd", "unknown");

	returnOK(target, cmd, ret, payload);
}


void MonitorWorker::returnOK(const std::string &target, const std::string &cmd, const std::string &ret, const std::string &payload) 
	throw (IOException)
{
	trace_enter_msg(ret);
	int psize = payload.size();

	if (psize !=0) 
	{
		stringstream ss;

		ss << "ver=1.0;target="<<target<<";cmd="<<cmd<<";success=ok;ret="<<ret<<";payloadSize="<<psize<<"\n"<<payload;

		string msg = ss.str();

		trace("returnOK ==> " << msg);
		writeString(msg);
	}
	else
	{	
		string msg = "ver=1.0;target=" + target + ";cmd=" + cmd + ";success=ok;ret=" + ret + ";payloadSize=0\n";
		trace("returnOK ==> " << msg);
		writeString(msg);
	}
}

void MonitorWorker::returnOK(const string &target, const string &cmd, const string &ret) throw (IOException)
{
	trace_enter_msg(ret);

	string msg = "ver=1.0;target=" + target + ";cmd=" + cmd + ";success=ok;ret=" + ret + ";payloadSize=0\n";
	
	trace("returnOK ==> " << msg);
	writeString(msg);
}
