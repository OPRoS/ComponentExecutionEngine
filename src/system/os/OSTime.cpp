/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSTime.cpp
 *
 */

#include <cstdio>
#include <ctime>
#include <cstring>

#include "OSSetting.h"
#include "OSTime.h"

using namespace std;

void OSTime::getCurrentTime(OSTime &ost)
{
	ost.resetToCurrentTime();
}

OSTime OSTime::set(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
{
	if (nYear >= 2038) nYear = 2037;		// 날짜가 초과하면 m_time==-1만 세팅되므로 그것을 방지한다.
	else if (nYear < 1970)	nYear = 1970;	// mktime 날짜가 1970년1월1일부터만 다루므로.

	struct tm  t;
	t.tm_sec    = nSec;
	t.tm_min    = nMin;
	t.tm_hour   = nHour;
	t.tm_mday   = nDay;
	t.tm_mon    = nMonth ? nMonth - 1 : 0;
	t.tm_year   = nYear % 1900;
	t.tm_isdst 	= nDST;
	m_time = mktime(&t);
	// t.tm_wday    		 		//* Day of the week 자동 셋팅 됨.

	return *this;
}


char *OSTime::asctime()
{
	struct tm  *tt = localtime(&m_time);
	if (tt == NULL) return NULL;
	return ::asctime(tt);	// 표준 asCTime은 끝에 \n 이 붙는다. 그리고 날짜 형식이 우리와 맞지 않다.
}

string OSTime::dateTimeToString(int type)
{
	char buf[101];

	struct tm  *tt = localtime(&m_time);
	if (tt == NULL) return string("");
	char  fmt[32];  // 정적 메모리 변경 위반에 대비하여 local 메모리 할당
	strcpy(fmt,"%04d-%02d-%02d %02d:%02d:%02d");		
	if (type & NO_SEC) strcpy(fmt,"%04d-%02d-%02d %02d:%02d"); // 초 생략하라.
	if (type & SHORT_YEAR) fmt[2] = '2';		// %02d 년도를 2자리로.
	sprintf(buf, fmt,
		type & SHORT_YEAR ? tt->tm_year % 100: tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday,
		tt->tm_hour, tt->tm_min, tt->tm_sec);
	if (type & NO_SEPARATE) deleteSeparate(buf);
	return string(buf);
}

string OSTime::dateToString(int type)
{
	char buf[101];

	struct tm  *tt = localtime(&m_time);
	if (tt == NULL) return string("");
	char  fmt[16]; // 정적 메모리 변경 위반에 대비하여 local 메모리 할당
	strcpy(fmt,"%04d-%02d-%02d");
	if (type & SHORT_YEAR) fmt[2] = '2';		// %02d 년도를 2자리로.
	sprintf(buf, fmt,
		type & SHORT_YEAR ? tt->tm_year % 100: tt->tm_year + 1900, tt->tm_mon + 1, tt->tm_mday);
	if (type & NO_SEPARATE) deleteSeparate(buf);

	return string(buf);
}

string OSTime::timeToString(int type)
{
	char buf[101];

	struct tm  *tt = localtime(&m_time);
	if (tt == NULL) return string("");
	char  fmt[16];
	strcpy(fmt,"%02d:%02d:%02d");
	if (type & NO_SEC) strcpy(fmt,"%02d:%02d");
	sprintf(buf, fmt, tt->tm_hour, tt->tm_min, tt->tm_sec);
	if (type & NO_SEPARATE) deleteSeparate(buf);
	return string(buf);
}

// strftime 등에서 바로 사용하기 위해,
// 예)strftime(buf, 100, "%y%m%d %H%M%S", time.tm());
struct tm  *OSTime::getTimeStruct()
{
	return localtime(&m_time);
}

// strftime 에 준하는 포멧을 사용해야 한다.
// 예:String datetime = t.format("%Y.%m.%d %H:%M:%S");
string OSTime::format(char *fmt)
{
	char buf[101];
	const tm tmp_tm = tm();
	strftime(buf, 100, fmt, &tmp_tm);
	return string(buf);
}

void OSTime::deleteSeparate(char buf[])
{
	char  *p = buf;
	for(int c = 0; buf[c]; c++) {
		if (buf[c] == '-' || buf[c] == ':' || buf[c] == ' ') continue;
		*p++ = buf[c];
	}
	*p = 0;
}

