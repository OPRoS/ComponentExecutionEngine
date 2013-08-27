/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_oarchive.cpp
 *
 *
 *
 *  OPROS output archive implementation
 */

#include <iostream>
#include <assert.h>
#include "archive/opros_oarchive.h"

using namespace opros::archive;

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, char val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, wchar_t val)
{
	ar.save(val);
	return ar;
}


opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, unsigned char val)
{
	ar.save(val);
	return ar;
}	

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, bool val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, short val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, unsigned short val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, int val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, unsigned int val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, long val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, unsigned long val)
{
	ar.save(val);
	return ar;
}


opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, float val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, double val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, long double val)
{
	ar.save(val);
	return ar;
}

opros::archive::oarchive &operator<<(opros::archive::oarchive &ar, long long val)
{
	ar.save(val);
	return ar;
}