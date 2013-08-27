/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoByteUtil.cpp
 *
 */

#include "IoByteUtil.h"

#include <iostream>

using namespace std;

IoByteUtil::IoByteUtil()
{
	m_host		= testHostByteOrder();
	m_network	= IO_BIG_ENDIAN;
}

IoByteUtil::~IoByteUtil()
{
}

IoByteOrder IoByteUtil::getHostByteOrder()
{
	return m_host;
}

IoByteOrder IoByteUtil::getNetworkByteOrder()
{
	return m_network;
}

void IoByteUtil::setHostByteOrder(IoByteOrder f)
{
	m_host = f;
}

void IoByteUtil::setNetworkByteOrder(IoByteOrder f)
{
	m_network = f;
}

IoByteOrder IoByteUtil::testHostByteOrder()
{
	short w = 0x4321;
	if((*(char *)& w) != 0x21 )
		return IO_BIG_ENDIAN;
	else
		return IO_LITTLE_ENDIAN;
}

// --- get/put

byte IoByteUtil::get1(byte *buf, int bi)
{
	return (byte) buf[bi];
}

int16 IoByteUtil::getInt16(byte *buf, int bi)
{
	bint16 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf2EQ(bx.buf, 0, buf, bi);
		return bx.x;
	}
	else {
		setBuf2NE(bx.buf, 0, buf, bi);
		return bx.x;
	}
}

uint16 IoByteUtil::getUInt16(byte *buf, int bi)
{
	bint16 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf2EQ(bx.buf, 0, buf, bi);
		return bx.ux;
	}
	else {
		setBuf2NE(bx.buf, 0, buf, bi);
		return bx.ux;
	}
}

int32 IoByteUtil::getInt32(byte *buf, int bi)
{
	bint32 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf4EQ(bx.buf, 0, buf, bi);
		return bx.x;
	}
	else {
		setBuf4NE(bx.buf, 0, buf, bi);
		return bx.x;
	}
}

uint32 IoByteUtil::getUInt32(byte *buf, int bi)
{
	bint32 bx;

	if (getHostByteOrder() == getNetworkByteOrder())
	{
		setBuf4EQ(bx.buf, 0, buf, bi);
		return bx.ux;
	}
	else
	{
		setBuf4NE(bx.buf, 0, buf, bi);
		return bx.ux;
	}
}

int64 IoByteUtil::getInt64(byte *buf, int bi)
{
	bint64 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf8EQ(bx.buf, 0, buf, bi);
		return bx.x;
	}
	else {
		setBuf8NE(bx.buf, 0, buf, bi);
		return bx.x;
	}
}

uint64 IoByteUtil::getUInt64(byte *buf, int bi)
{
	bint64 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf8EQ(bx.buf, 0, buf, bi);
		return bx.ux;
	}
	else {
		setBuf8NE(bx.buf, 0, buf, bi);
		return bx.ux;
	}
}

float32 IoByteUtil::getFloat32(byte *buf, int bi)
{
	bfloat32 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf4EQ(bx.buf, 0, buf, bi);
		return bx.x;
	}
	else {
		setBuf4NE(bx.buf, 0, buf, bi);
		return bx.x;
	}
}

float64 IoByteUtil::getFloat64(byte *buf, int bi)
{
	bfloat64 bx;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf8EQ(bx.buf, 0, buf, bi);
		return bx.x;
	}
	else {
		setBuf8NE(bx.buf, 0, buf, bi);
		return bx.x;
	}
}

void IoByteUtil::put1(byte *buf, int bi, byte x)
{
	buf[bi] = x;
}

void IoByteUtil::putInt16(byte *buf, int bi, int16 x)
{
	bint16 bx;
	bx.x = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf2EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf2NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putUInt16(byte *buf, int bi, uint16 x)
{
	bint16 bx;
	bx.ux = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf2EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf2NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putInt32(byte *buf, int bi, int32 x)
{
	bint32 bx;
	bx.x = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf4EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf4NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putUInt32(byte *buf, int bi, uint32 x)
{
	bint32 bx;
	bx.ux = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf4EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf4NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putInt64(byte *buf, int bi, int64 x)
{
	bint64 bx;
	bx.x = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf8EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf8NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putUInt64(byte *buf, int bi, uint64 x)
{
	bint64 bx;
	bx.ux = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf8EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf8NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putFloat32(byte *buf, int bi, float32 x)
{
	bfloat32 bx;
	bx.x = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf4EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf4NE(buf, bi, bx.buf, 0);
	}
}

void IoByteUtil::putFloat64(byte *buf, int bi, float64 x)
{
	bfloat64 bx;
	bx.x = x;

	if (getHostByteOrder() == getNetworkByteOrder()) {
		setBuf8EQ(buf, bi, bx.buf, 0);
	}
	else {
		setBuf8NE(buf, bi, bx.buf, 0);
	}
}


//
// -- get/put byte --
//

byte IoByteUtil::getByte(byte *buf, int bi)
{
	return buf[bi];
}

void IoByteUtil::putByte(byte *buf, int bi, byte b)
{
	buf[bi] = b;
}

//
// -- get/put bool --
//

bool IoByteUtil::getBool(byte *buf, int off)
{
	return buf[off] != 0;
}

void IoByteUtil::putBool(byte *buf, int off, bool val)
{
	buf[off] = (byte) (val ? 1 : 0);
}

//
// -- get/put char --
//
char IoByteUtil::getChar(byte *buf, int bi)
{
	return buf[bi];
}

void IoByteUtil::putChar(byte *buf, int bi, char x)
{
	buf[bi] = x;
}

//
// -- get/put wchar --
//

WideChar IoByteUtil::getWideChar(byte *buf, int bi)
{
	return (WideChar) getUInt16(buf, bi);
}

void IoByteUtil::putWideChar(byte *buf, int bi, WideChar x)
{
	putUInt16(buf, bi, (unsigned int) x);
}

//
// -- get/put short --
//
short IoByteUtil::getShort(byte *buf, int bi)
{
	return getInt16(buf, bi);
}

unsigned short IoByteUtil::getUnsignedShort(byte *buf, int bi)
{
	return getUInt16(buf, bi);
}

void IoByteUtil::putShort(byte *buf, int bi, short x)
{
	putInt16(buf, bi, x);
}

void IoByteUtil::putUnsignedShort(byte *buf, int bi, unsigned short x)
{
	putUInt16(buf, bi, x);
}

//
// -- get/put int --
//
int IoByteUtil::getInt(byte *buf, int bi)
{
	return getInt32(buf, bi);
}

unsigned int IoByteUtil::getUnsignedInt(byte *buf, int bi)
{
	return getUInt32(buf, bi);
}

void IoByteUtil::putInt(byte *buf, int bi, int x)
{
	putInt32(buf, bi, x);
}

void IoByteUtil::putUnsignedInt(byte *buf, int bi, unsigned int x)
{
	putInt32(buf, bi, x);
}

//
// -- get/put long --
//
long IoByteUtil::getLong(byte *buf, int bi)
{
	return (long) getInt32(buf, bi);
}

unsigned long IoByteUtil::getUnsignedLong(byte *buf, int bi)
{
	return (unsigned long) getUInt32(buf, bi);
}

void IoByteUtil::putLong(byte *buf, int bi, long x)
{
	putInt32(buf, bi,x);
}

void IoByteUtil::putUnsignedLong(byte *buf, int bi, unsigned long x)
{
	putUInt32(buf, bi, x);
}

//
// -- get/put float --
//
float IoByteUtil::getFloat(byte *buf, int bi)
{
	return (float) getFloat32(buf, bi);
}

void IoByteUtil::putFloat(byte *buf, int bi, float x)
{
	putFloat32(buf, bi, x);
}

//
// -- get/put double --
//

double IoByteUtil::getDouble(byte *buf, int bi)
{
	return getFloat64(buf, bi);
}

void IoByteUtil::putDouble(byte *buf, int bi, double x)
{
	putFloat64(buf, bi,x);
}

//
// --- get/put strings
//
void IoByteUtil::getString(byte *buf, int pos, char *cbuf, int off, int len)
{
	for (int i = 0; i < len; i++) {
		cbuf[off++] = getChar(buf, pos);
		pos++;
	}
}

void IoByteUtil::putString(byte *buf, int pos, const char *cbuf, int off, int len)
{
	int endoff = off + len;
	for (; off < endoff; off++) {
		putChar(buf, pos, cbuf[off]);
		pos++;
	}
}

//
// --- get/put wstrings
//
void IoByteUtil::getWideString(byte *buf, int pos, WideChar *cbuf, int off, int size)
{
	for (int i = 0; i < size; i++) {
		cbuf[off++] = getWideChar(buf, pos);
		pos += 2;
	}
}

void IoByteUtil::putWideString(byte *buf, int pos, const WideChar *cbuf,	int off, int size)
{
	int endoff = off + size;
	for (; off < endoff; off++)	{
		putWideChar(buf, pos, cbuf[off]);
		pos += 2;
	}
}

void IoByteUtil::PrintsBytes(byte * data, int off, int size)
{
    if (data != NULL)   {
        for (int i = 0; i < size; i++) {
            cout << byte2char(hibyte(data[off+i])) << byte2char(lobyte(data[off+i]));
        }
		cout << "(size=" << size << ")\n";
    }
}

byte IoByteUtil::hibyte(byte b)
{
    return (byte) (((b & 0xf0) >> 4));
}

byte IoByteUtil::lobyte(byte b)
{
    return (byte) ((b & 0x0f));
}

char IoByteUtil::byte2char(byte b)
{
    if (b >= 0 && b <= 9)
        return (char) (b + '0');

    switch (b)
    {
        case (byte) 10: return 'A';
        case (byte) 11: return 'B';
        case (byte) 12: return 'C';
        case (byte) 13: return 'D';
        case (byte) 14: return 'E';
        case (byte) 15: return 'F';
    }

    return 'X';
}
