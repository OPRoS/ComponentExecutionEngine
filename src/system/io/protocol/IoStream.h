/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStream.h
 *
 */

#ifndef _IO_STREAM_H_
#define _IO_STREAM_H_

#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>

#include <vector>
#include <map>

#include "system/util/BasicTypes.h"

class IoStream
{
public :
	IoStream() {};
	virtual ~IoStream() {};

	//
	// null checking
	//
	virtual bool isNull() = 0;

	//
	// byte
	//
	virtual byte getByte() = 0;
	virtual void putByte(byte x) = 0;

	friend IoStream & operator>>(IoStream &ios, byte &v) {
		v = ios.getByte();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, byte &v) {
		ios.putByte(v);
		return ios;
	};


	//
	// bytes
	//
	virtual void getBytes(byte *dst, int size) = 0;
	virtual void getBytes(byte *dst, int off, int size) = 0;

	virtual void putBytes(byte *src, int size) = 0;
	virtual void putBytes(byte *src, int off, int size) = 0;

	//
	// char
	//
	virtual char getChar() = 0;
	virtual void putChar(char x) = 0;

	friend IoStream & operator>>(IoStream &ios, char &v) {
		v = ios.getChar();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, char &v) {
		ios.putChar(v);
		return ios;
	};

	//
	// wchar
	//
	virtual WideChar getWideChar() = 0;
	virtual void putWideChar(WideChar x) = 0;

	friend IoStream & operator>>(IoStream &ios, WideChar &v) {
		v = ios.getWideChar();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, WideChar &v) {
		ios.putWideChar(v);
		return ios;
	};


	//
	// bool
	//
	virtual bool getBool() = 0;
	virtual void putBool(bool x) = 0;

	friend IoStream & operator>>(IoStream &ios, bool &v) {
		v = ios.getBool();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, bool &v) {
		ios.putBool(v);
		return ios;
	};

	//
	// short
	//
	virtual short getShort() = 0;
	virtual void putShort(short x) = 0;

	friend IoStream & operator>>(IoStream &ios, short &v) {
		v = ios.getShort();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, short &v) {
		ios.putShort(v);
		return ios;
	};

	//
	// int
	//
	virtual int getInt() = 0;
	virtual void putInt(int x) = 0;

	friend IoStream & operator>>(IoStream &ios, int &v) {
		v = ios.getInt();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, int &v) {
		ios.putInt(v);
		return ios;
	};

	//
	// long
	//
	virtual long getLong() = 0;
	virtual void putLong(long x) = 0;

	friend IoStream & operator>>(IoStream &ios, long &v) {
		v = ios.getLong();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, long &v) {
		ios.putLong(v);
		return ios;
	};

	//
	// float
	//
	virtual float getFloat() = 0;
	virtual void putFloat(float x) = 0;

	friend IoStream & operator>>(IoStream &ios, float &v) {
		v = ios.getFloat();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, float &v) {
		ios.putFloat(v);
		return ios;
	};

	//
	// double
	//
	virtual double getDouble() = 0;
	virtual void putDouble(double x) = 0;

	friend IoStream & operator>>(IoStream &ios, double &v) {
		v = ios.getDouble();
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, double &v) {
		ios.putDouble(v);
		return ios;
	};

	//
	// string
	//
	virtual char *getStringToBuf() = 0;
	virtual void getString(char *cbuf) = 0;
	virtual void putString(const char *x) { putString(x, strlen(x)); };
	virtual void putString(const char *x, int len) {
		putInt(len);
		putBytes((byte *) x, 0, len);
	};

	friend IoStream & operator>>(IoStream &ios, char *v) {
		ios.getString(v);
		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, const char *v) {
		ios.putString(v);
		return ios;
	};
	friend IoStream & operator>>(IoStream &ios, std::string &v) {
		int size;
		ios >> size;

		byte *buf = new byte[size];
		if (buf != NULL)
		{
			ios.getBytes(buf,0, size);
		//buf[size+1] = 0;

			v.assign( (const char *)buf, size);


/*		std::ostringstream oss;
		for (int i = 0; i < size; i++) oss << buf[i];

		v = oss.str();

		int nsize;

		nsize = v.size(); */

			delete buf;
		}

		return ios;
	};
	friend IoStream & operator<<(IoStream &ios, const std::string &v) {
		ios.putString(v.c_str(), v.size());
		return ios;
	};

	//--------------------
	// vector
	template <typename T> friend IoStream & operator>>(IoStream &ios, std::vector<T> &v) {
		ios.putInt(v.size());

		for (int idx = 0; idx < v.size(); ++idx) {
			ios << v.at(idx);
		}

		return ios;
	};

	template <typename T> friend IoStream & operator<<(IoStream &ios, std::vector<T> &v) {
		int size;
		ios >> size;

		for (int idx = 0; idx << size; ++idx) {
			T data;
			ios >> data;
			v.push_back(data);
		}
		return ios;
	};

	//--------------------
	// map
	template <typename T, typename V> friend IoStream & operator>>(IoStream &ios, std::map<T, V> &m) {
		typedef std::map<T, V> MapType;
		ios.putInt(m.size());
		typename std::map<T, V>::iterator iter = m.begin();
		for (; iter != m.end(); ++iter) {
			ios << iter->first;
			ios << iter->second;
		}
		return ios;
	};

	template <typename T, typename V> friend IoStream & operator<<(IoStream &ios, std::map<T, V> &m) {
		int size;
		ios >> size;

		for (int idx = 0; idx << size; ++idx) {
			T name;
			V value;

			ios >> name;
			ios >> value;

			m.insert(std::make_pair(name, value));
		}
		return ios;
	};

	//----------------------------------------------------------------------------------
	// any pointer type implements operator << overloading (template specialization)
	template <typename T> friend IoStream & operator>>(IoStream &ios, T *m) {
		ios >> *m;
		return ios;
	};

	template <typename T> friend IoStream & operator<<(IoStream &ios, T *m) {
		ios << *m;
		return ios;
	};

	//-----------------------------------------------------------
	// any type implements operator << overloading
	template <typename T> friend IoStream & operator>>(IoStream &ios, T m) {
		ios >> m;
		return ios;
	};

	template <typename T> friend IoStream & operator<<(IoStream &ios, T m) {
		ios << m;
		return ios;
	};


};

#endif



