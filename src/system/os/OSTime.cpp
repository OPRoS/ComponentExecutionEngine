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
	if (nYear >= 2038) nYear = 2037;		// ��¥�� �ʰ��ϸ� m_time==-1�� ���õǹǷ� �װ��� �����Ѵ�.
	else if (nYear < 1970)	nYear = 1970;	// mktime ��¥�� 1970��1��1�Ϻ��͸� �ٷ�Ƿ�.

	struct tm  t;
	t.tm_sec    = nSec;
	t.tm_min    = nMin;
	t.tm_hour   = nHour;
	t.tm_mday   = nDay;
	t.tm_mon    = nMonth ? nMonth - 1 : 0;
	t.tm_year   = nYear % 1900;
	t.tm_isdst 	= nDST;
	m_time = mktime(&t);
	// t.tm_wday    		 		//* Day of the week �ڵ� ���� ��.

	return *this;
}


char *OSTime::asctime()
{
	struct tm  *tt = localtime(&m_time);
	if (tt == NULL) return NULL;
	return ::asctime(tt);	// ǥ�� asCTime�� ���� \n �� �ٴ´�. �׸��� ��¥ ������ �츮�� ���� �ʴ�.
}

string OSTime::dateTimeToString(int type)
{
	char buf[101];

	struct tm  *tt = localtime(&m_time);
	if (tt == NULL) return string("");
	char  fmt[32];  // ���� �޸� ���� ���ݿ� ����Ͽ� local �޸� �Ҵ�
	strcpy(fmt,"%04d-%02d-%02d %02d:%02d:%02d");		
	if (type & NO_SEC) strcpy(fmt,"%04d-%02d-%02d %02d:%02d"); // �� �����϶�.
	if (type & SHORT_YEAR) fmt[2] = '2';		// %02d �⵵�� 2�ڸ���.
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
	char  fmt[16]; // ���� �޸� ���� ���ݿ� ����Ͽ� local �޸� �Ҵ�
	strcpy(fmt,"%04d-%02d-%02d");
	if (type & SHORT_YEAR) fmt[2] = '2';		// %02d �⵵�� 2�ڸ���.
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

// strftime ��� �ٷ� ����ϱ� ����,
// ��)strftime(buf, 100, "%y%m%d %H%M%S", time.tm());
struct tm  *OSTime::getTimeStruct()
{
	return localtime(&m_time);
}

// strftime �� ���ϴ� ������ ����ؾ� �Ѵ�.
// ��:String datetime = t.format("%Y.%m.%d %H:%M:%S");
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

