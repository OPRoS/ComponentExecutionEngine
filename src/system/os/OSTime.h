/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSTimeVal.h
 *
 */

#ifndef _OS_TIME_H_
#define _OS_TIME_H_

#include <ctime>
#include <string>

/**
	�ð� ���� �Լ��� ������ Ŭ����.
*/
class OSTime
{
	time_t m_time;

	enum {
		NO_SEC 			= 0x1,	// �ʻ���
		SHORT_YEAR		= 0x2,	// ��2�ڸ���
		NO_SEPARATE		= 0x4,	// - : ���� ���ҹ��ڸ� ���ش�. sDateTime������ ��ĭ�� ���ش�.
	};

public :
	static void getCurrentTime(OSTime &ost);

public :
	OSTime() : m_time(0) {};

	OSTime(OSTime& src) : m_time(src.m_time)	{};
	OSTime(time_t atime) {
		set(atime);
	}

	OSTime(int nYear, int nMonth, int nDay, int nHour=0, int nMin=0, int nSec=0, int nDST=-1) {
		set(nYear, nMonth, nDay, nHour, nMin, nSec);
	}

	~OSTime() {};

	OSTime set(int nYear, int nMonth, int nDay, int nHour=0, int nMin=0, int nSec=0, int nDST=-1);
	void set(time_t atime) {
		if (atime == 0) m_time = time(NULL);
		else m_time = atime;
	}
	void resetToCurrentTime() {
		m_time = time(NULL);
	}

	// time_t time; ������ �׳� CTime  t = time; ������ �ٷ� ���԰���.
	// time_t ���� �������� time_t time = t.time_t(); �� ��.
	OSTime& operator=( OSTime& time ) {
		 m_time = time.m_time;
		 return *this;
	};
	OSTime& operator+=( OSTime& time ) {
		m_time += time.m_time;
		return *this;
	};
	OSTime& operator-=( OSTime& time ) {
		m_time -= time.m_time;
		return *this;
	};
	OSTime operator-( OSTime& time ) {
		OSTime ostime(m_time - time.m_time);
		return ostime;
	};
	OSTime operator+( OSTime& time ) {
		OSTime ostime(m_time + time.m_time);
		return ostime;
	};
	OSTime& operator ++() {
		++m_time;
		return *this;
	};
	OSTime operator ++(int) {
		OSTime ostime(m_time++);
		return ostime;
	};
	OSTime& operator --() {
		--m_time;
		return *this;
	};
	OSTime operator --(int) {
		OSTime ostime(m_time--);
		return ostime;
	};

	bool operator==( OSTime& time ) {
		return m_time == time.m_time;
	};
	bool operator==( int time ) {
		return m_time == time;
	};
	bool operator!=( OSTime& time ) {
		return m_time != time.m_time;
	};
	bool operator<( OSTime& time ) {
		return m_time < time.m_time;
	};
	bool operator>( OSTime& time ) {
		return m_time > time.m_time;
	};
	bool operator<=( OSTime& time ) {
		return m_time <= time.m_time;
	}
	bool operator>=( OSTime& time )	{
		return m_time >= time.m_time;
	};

	time_t 	getTime() {
		return m_time;
	};

	char *asctime();

	std::string  dateTimeToString(int type = 0);
	std::string  dateToString(int type = 0);
	std::string  timeToString(int type = 0);

	// strftime ��� �ٷ� ����ϱ� ����, ��)strftime(buf, 100, "%y%m%d %H%M%S", time.tm());
	struct tm  *getTimeStruct();

	// strftime �� ���ϴ� ������ ����ؾ� �Ѵ�.
	// ��:String datetime = t.format("%Y.%m.%d %H:%M:%S");
	std::string  format(char *fmt);

private:
	void deleteSeparate(char buf[]);
};

#endif 