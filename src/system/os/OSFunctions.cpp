/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSFunctions.cpp
 *
 */

#include "OSFunctions.h"
#include "system/util/StringUtil.h"

using namespace std;

#ifdef OS_WIN32

int OS::getLastError()
{
	return errno = ::GetLastError();
}

void OS::sleepMS(uint32 dwMilliseconds) {
	::Sleep(dwMilliseconds);
}

char OS::fileSeparator() {
	return  '\\';
}

#else // OS_LINUX

#include <errno.h>
#include <unistd.h>
#include <time.h>

int OS::getLastError()
{
	return errno;
}

void OS::sleepMS(uint32 dwMilliseconds) {
		struct timespec tm;

        tm.tv_sec = dwMilliseconds/1000;
        tm.tv_nsec = (dwMilliseconds % 1000L)*1000000L;

		nanosleep(&tm, 0);
}

char OS::fileSeparator() {
	return  '/';
}

#endif

int OS::select(int nfds, fd_set *rset, fd_set *wset, fd_set *eset, long msTimeout)
{
	if (msTimeout == 0) {
		return ::select(nfds, rset, wset, eset, NULL);
	}
	else {
		struct timeval tmp;
		tmp.tv_sec = msTimeout / 1000;
		tmp.tv_usec = msTimeout % 1000;
		return ::select(nfds, rset, wset, eset, &tmp);
	}
}

string OS::toDir(const string &path)
{
	string dir = "";

	if (path.size() > 0) {
		int idx = path.size()-1;
		for (; idx >= 0; --idx) {
			if (path[idx] != ' ') break;
		}
		if (idx >= 0) {
			dir = path.substr(0, idx+1);
			string fs;
			fs.push_back(OS::fileSeparator());
			StringUtil::replaceAll(dir, "/", fs);
			if (dir[idx] != OS::fileSeparator()) {
				dir += OS::fileSeparator();
			}
		}
	}

	return dir;
}

string OS::getFileName(const string& fullpath)
{
	size_t found;
	found = fullpath.find_last_of(OS::fileSeparator());
	if (found != string::npos) {
		return fullpath.substr(found+1);
	}
	return fullpath;
}

string OS::getDirName(const string& fullpath)
{
	size_t found;
	found = fullpath.find_last_of(OS::fileSeparator());
	if (found != string::npos) {
		return fullpath.substr(0, found);
	}
	return "";
}

string OS::splitExt(const string &filename)
{
	string fname = OS::getFileName(filename);

	unsigned int idx = fname.find_last_of('.');
	if (idx != string::npos) return fname.substr(0, idx);
	else return fname;
}

