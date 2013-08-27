/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_iarchive.cpp
 *
 *
 *
 *  OPROS input archive implementation
 */


#include <iostream>
#include <assert.h>
#include "archive/opros_iarchive.h"

using namespace opros::archive;


opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, char &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, wchar_t &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, unsigned char &val)
{
	ar.load(val);
	return ar;
}	

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, bool &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, short &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, unsigned short &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, int &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, unsigned int &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, long &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, unsigned long &val)
{
	ar.load(val);
	return ar;
}


opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, float &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, double &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, long double &val)
{
	ar.load(val);
	return ar;
}

opros::archive::iarchive &operator>>(opros::archive::iarchive &ar, long long &val)
{
	ar.load(val);
	return ar;
}