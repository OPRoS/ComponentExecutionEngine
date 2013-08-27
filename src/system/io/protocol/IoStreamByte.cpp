/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoStreamByte.cpp
 *
 */

#include "IoStreamByte.h"

#include "system/util/Exceptions.h"

#include "system/Log.h"
#include "system/Trace.h"

#include <cstring>

IoStreamByte::IoStreamByte()
{
	setup(0);
}

IoStreamByte::IoStreamByte(int size)
{
	setup(size);
}

IoStreamByte::~IoStreamByte()
{
	deleteBuffer();
}

void IoStreamByte::setup(int size)
{
	m_buf = NULL;

	m_position	= 0;
	m_limit = 0;
	m_offset = 0;

	m_IsHostBigEndian		= false;
	m_IsNetworkBigEndian	= true;

	if (size <= 0) reserve(BUF_INIT_SIZE);
	else reserve(size);
}

//
// Buffer Management
//
bool IoStreamByte::incBuffer(int nb)
{
	if (m_size <= 0) {
		m_buf = new byte[BUF_INC_SIZE];
		if (m_buf == NULL)
		{
			return false;
			m_size = 0;
		}
		m_size = BUF_INC_SIZE;
		reset();
	}
	else if (m_limit - m_position < nb) {
		if ((m_offset + m_limit + nb) < m_size) {
			m_limit = m_size - m_offset;
		}
		else {
			int size;
			if (nb < BUF_INC_SIZE) size = m_offset + m_limit + BUF_INC_SIZE;
			else size = m_offset + m_limit + BUF_INC_SIZE + nb;

			byte *buf = new byte[size];
			if (buf == NULL)
			{
				return false;
			}

			memcpy(buf, m_buf, m_offset + m_limit);
			m_limit = size - m_offset;

			deleteBuffer();
			m_buf = buf;
			m_size = size;
		}
	}

	return true;
}


int IoStreamByte::checkGetIndex(int i)
{
	if ((i < 0) || (i >= m_limit))	throw IOException("Invalide idx");

	return i;
}

int IoStreamByte::checkGetIndex(int i, int nb)
{
	if ((i < 0) || (nb > m_limit - i))
		throw IOException("Invalide idx");

	return i;
}

int IoStreamByte::checkPutIndex(int i)
{
	if ((i < 0) || (i >= m_limit))
		throw IOException("Invalide idx");

	return i;
}

int IoStreamByte::checkPutIndex(int i, int nb)
{
	if ((i < 0) || (nb > m_limit - i))
		throw IOException("Invalide idx");

	return i;
}

int IoStreamByte::nextGetIndex()
{
	if (m_position >= m_limit)
		throw IOException("Invalide idx");

	return m_position++;
}

int IoStreamByte::nextGetIndex(int nb)
{
	if (m_limit - m_position < nb)
		throw IOException("Invalide idx");

	int p = m_position;
	m_position += nb;
	return p;
}

int IoStreamByte::nextPutIndex()
{
	if (incBuffer(1) == false)
		throw IOException("Invalide idx");

	return m_position++;
}

int IoStreamByte::nextPutIndex(int nb)
{
	if (incBuffer(nb) == false)
		throw IOException("Invalide idx");

	int p = m_position;
	m_position += nb;
	return p;
}

int IoStreamByte::checkBounds(int off, int len, int size)
{
	if (incBuffer(len) == false) return 0;

	if ((off | len | (off + len) | (size - (off + len))) < 0)
		return 0;

	return 1;
}

int IoStreamByte::ix(int i)
{
	return i + m_offset;
}

int IoStreamByte::ix()
{
	return position() + m_offset;
}


// --------------------------------------------------------------------------
//

//
// null checking
//
bool IoStreamByte::isNull()
{
	return !(getBool());
}


//
// byte & bytes
//

byte IoStreamByte::getByte()
{
	return m_buf[ix(nextGetIndex())];
}

void IoStreamByte::putByte(byte x)
{
	int idx = ix(nextPutIndex());
	m_buf[idx] = x;
}

void IoStreamByte::getBytes(byte * dst, int len)
{
	getBytes(dst, 0, len);
}

void IoStreamByte::getBytes(byte * dst, int off, int len)
{
	if (len <= 0) return;

	if (checkBounds(off, len, len) <= 0) throw IOException("Invalide idx");
	if (len > remaining()) throw IOException("Invalide idx");


	copyByteBuffer(m_buf, ix(position()), dst, off, len);
	position(position() + len);
}

void IoStreamByte::putBytes(byte * src, int len)
{
	putBytes(src, 0, len);
}

void IoStreamByte::putBytes(byte * src, int off, int len)
{
	if (len <= 0) return;

	if (checkBounds(off, len, len) <= 0) throw IOException("Invalide idx");
	if (len > remaining()) throw IOException("Invalide idx");

	copyByteBuffer(src, off, m_buf, ix(position()), len);
	position(position() + len);
}

void IoStreamByte::copyByteBuffer(byte *src, int si, byte *dst, int di, int sz)
{
	memcpy(dst + di, src + si, sz);
}

//
// char
//
char IoStreamByte::getChar()
{
	return m_byteUtil.getChar(m_buf, ix(nextGetIndex(1)));
}

void IoStreamByte::putChar(char x)
{
	int idx = ix(nextPutIndex(1));
	m_byteUtil.putChar(m_buf, idx, x);
}

//
// wchar
//
WideChar IoStreamByte::getWideChar()
{
	return m_byteUtil.getWideChar(m_buf, ix(nextGetIndex(sizeof(WideChar))));
}

void IoStreamByte::putWideChar(WideChar x)
{
	int idx = ix(nextPutIndex(sizeof(WideChar)));
	m_byteUtil.putWideChar(m_buf, idx, x);
}

//
// bool
//

bool IoStreamByte::getBool()
{
	return m_byteUtil.getBool(m_buf, ix(nextGetIndex(1)));
}

void IoStreamByte::putBool(bool x)
{

	int idx = ix(nextPutIndex(1));
	m_byteUtil.putBool(m_buf, idx, x);

}

//
// short
//

short IoStreamByte::getShort()
{
	return m_byteUtil.getShort(m_buf, ix(nextGetIndex(2)));
}

void IoStreamByte::putShort(short x)
{
	int idx = ix(nextPutIndex(2));
	m_byteUtil.putShort(m_buf, idx, x);

}

//
// int
//

int IoStreamByte::getInt()
{
	return m_byteUtil.getInt(m_buf, ix(nextGetIndex(4)));
}

void IoStreamByte::putInt(int x)
{
	int idx = ix(nextPutIndex(4));

	m_byteUtil.putInt(m_buf, idx, x);
}

//
// long
//

long IoStreamByte::getLong()
{
	return m_byteUtil.getLong(m_buf, ix(nextGetIndex(8)));
}

void IoStreamByte::putLong(long x)
{
	int idx = ix(nextPutIndex(8));
	m_byteUtil.putLong(m_buf, idx, x);

}

//
// float
//

float IoStreamByte::getFloat()
{
	return m_byteUtil.getFloat(m_buf, ix(nextGetIndex(4)));
}

void IoStreamByte::putFloat(float x)
{
	int idx = ix(nextPutIndex(4));
	m_byteUtil.putFloat(m_buf, idx, x);

}

//
// double
//

double IoStreamByte::getDouble()
{
	return m_byteUtil.getDouble(m_buf, ix(nextGetIndex(8)));
}

void IoStreamByte::putDouble(double x)
{
	int idx = ix(nextPutIndex(8));
	m_byteUtil.putDouble(m_buf, idx, x);

}

//
// string
//


char *IoStreamByte::getStringToBuf()
{
	int len = getInt();
	int idx = ix(nextGetIndex(len));

	char *cbuf = new char[len+1];
	if (cbuf == NULL) return NULL;
	if (len > 0)
		m_byteUtil.getString(m_buf, idx, cbuf, 0, len);
	cbuf[len] = '\0';

	return cbuf;
}

void IoStreamByte::getString(char *cbuf)
{
	int len = getInt();
	int idx = ix(nextGetIndex(len));

	if (len > 0)
		m_byteUtil.getString(m_buf, idx, cbuf, 0, len);
	cbuf[len] = '\0';

//	trace("len=" << len << "; str=" << cbuf);
}

void IoStreamByte::putString(const char *x)
{
//	trace_enter_msg("str=" << x << "; pos=" << position() << "; limit=" << m_limit);;

	int len = 0;
	if (x != NULL) len = strlen(x);

//	trace("str=" << x << "; pos=" << position() << "; limit=" << m_limit << "; len=" << len);

	putInt(len);

	if (len > 0) {
		int idx = ix(nextPutIndex(len));
		m_byteUtil.putString(m_buf, idx , x, 0, len);
	}

//	trace("str=" << x << "; pos=" << position() << "; limit=" << m_limit << "; len=" << len);
}


