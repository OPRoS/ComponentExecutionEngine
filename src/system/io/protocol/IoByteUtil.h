/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoByteUtil.h
 *
 */

#ifndef _IO_BYTE_UTIL_H_
#define _IO_BYTE_UTIL_H_

#include "system/util/BasicTypes.h"

typedef union {
	int16	x;
	uint16	ux;
	byte	buf[2];
} bint16;

typedef union {
	int32	x;
	uint32	ux;
	byte	buf[4];
} bint32;

typedef union {
	int64	x;
	uint64	ux;
	byte	buf[8];
} bint64;

typedef union {
	float32	x;
	byte	buf[4];
} bfloat32;

typedef union {
	float64	x;
	byte	buf[8];
} bfloat64;

#define setBuf2EQ(t, ti, s, si) {t[ti+0] = s[si+0]; \
                                 t[ti+1] = s[si+1];}

#define setBuf2NE(t, ti, s, si) {t[ti+1] = s[si+0]; \
                                 t[ti+0] = s[si+1];}

#define setBuf4EQ(t, ti, s, si) {t[ti+0] = s[si+0]; \
                                 t[ti+1] = s[si+1]; \
                                 t[ti+2] = s[si+2]; \
                                 t[ti+3] = s[si+3];}

#define setBuf4NE(t, ti, s, si) {t[ti+3] = s[si+0]; \
                                 t[ti+2] = s[si+1]; \
                                 t[ti+1] = s[si+2]; \
                                 t[ti+0] = s[si+3];}

#define setBuf8EQ(t, ti, s, si) {t[ti+0] = s[si+0]; \
                                 t[ti+1] = s[si+1]; \
                                 t[ti+2] = s[si+2]; \
                                 t[ti+3] = s[si+3]; \
                                 t[ti+4] = s[si+4]; \
                                 t[ti+5] = s[si+5]; \
                                 t[ti+6] = s[si+6]; \
                                 t[ti+7] = s[si+7];}

#define setBuf8NE(t, ti, s, si) {t[ti+7] = s[si+0]; t[ti+6] = s[si+1]; t[ti+5] = s[si+2]; t[ti+4] = s[si+3]; t[ti+3] = s[si+4]; t[ti+2] = s[si+5]; t[ti+1] = s[si+6]; t[ti+0] = s[si+7];}

typedef enum { IO_BIG_ENDIAN = 0, IO_LITTLE_ENDIAN } IoByteOrder;

class IoByteUtil
{
	IoByteOrder			m_host;
	IoByteOrder			m_network;

public :

	IoByteUtil();
	virtual ~IoByteUtil();

	IoByteOrder	getHostByteOrder();
	IoByteOrder	getNetworkByteOrder();
	void		setHostByteOrder(IoByteOrder f);
	void		setNetworkByteOrder(IoByteOrder f);
	IoByteOrder testHostByteOrder();

	//
	// get functions
	//
	byte	get1(byte *buf, int bi);

	int16	getInt16(byte *buf, int bi);
	uint16	getUInt16(byte *buf, int bi);

	int32	getInt32(byte *buf, int bi);
	uint32	getUInt32(byte *buf, int bi);

	int64	getInt64(byte *buf, int bi);
	uint64	getUInt64(byte *buf, int bi);

	float32	getFloat32(byte *buf, int bi);
	float64	getFloat64(byte *buf, int bi);

	//
	// put functions
	//
	void put1(byte *buf, int bi, byte x);

	void putInt16(byte *buf, int bi, int16 x);
	void putUInt16(byte *buf, int bi, uint16 x);

	void putInt32(byte *buf, int bi, int32 x);
	void putUInt32(byte *buf, int bi, uint32 x);

	void putInt64(byte *buf, int bi, int64 x);
	void putUInt64(byte *buf, int bi, uint64 x);

	void putFloat32(byte *buf, int bi, float x);
	void putFloat64(byte *buf, int bi, double x);

	//
	// -- get/put byte --
	//
	byte getByte(byte *buf, int bi);
	void putByte(byte *buf, int bi, byte b);

	//
	// -- get/put bool --
	//
	bool getBool(byte *buf, int off);
	void putBool(byte *buf, int off, bool val);

	//
	// -- get/put char --
	//
	char getChar(byte *buf, int bi);
	void putChar(byte *buf, int bi, char x);

	//
	// -- get/put wchar --
	//

	WideChar getWideChar(byte *buf, int bi);
	void putWideChar(byte *buf, int bi, WideChar x);

	//
	// -- get/put short --
	//
	short getShort(byte *buf, int bi);
	unsigned short getUnsignedShort(byte *buf, int bi);
	void putShort(byte *buf, int bi, short x);
	void putUnsignedShort(byte *buf, int bi, unsigned short x);

	//
	// -- get/put int --
	//
	int getInt(byte *buf, int bi);
	unsigned int getUnsignedInt(byte *buf, int bi);
	void putInt(byte *buf, int bi, int x);
	void putUnsignedInt(byte *buf, int bi, unsigned int x);

	//
	// -- get/put long --
	//
	long getLong(byte *buf, int bi);
	unsigned long getUnsignedLong(byte *buf, int bi);
	void putLong(byte *buf, int bi, long x);
	void putUnsignedLong(byte *buf, int bi, unsigned long x);

	//
	// -- get/put float --
	//
	float getFloat(byte *buf, int bi);
	void putFloat(byte *buf, int bi, float x);

	//
	// -- get/put double --
	//
	double getDouble(byte *buf, int bi);
	void putDouble(byte *buf, int bi, double x);

	//
	// --- get/put strings
	//
	void getString(byte *buf, int pos, char *cbuf, int off, int len);
	void putString(byte *buf, int pos, const char *cbuf,	int off, int len);

	//
	// --- get/put wstrings
	//
	void getWideString(byte *buf, int pos, WideChar *cbuf, int off, int len);
	void putWideString(byte *buf, int pos, const WideChar *cbuf,	int off, int len);

		//
	// Util ----------------------
	//
	//
	void PrintsBytes(byte * data, int off, int size);
	byte hibyte(byte b);
	byte lobyte(byte b);
	char byte2char(byte b);
};

#endif // _IO_BYTE_UTIL_H_
