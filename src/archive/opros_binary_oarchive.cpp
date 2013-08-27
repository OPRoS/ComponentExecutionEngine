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
#include "archive/opros_binary_oarchive.h"

using namespace opros::archive;


binary_oarchive::binary_oarchive(std::ostream *is) : m_is(is)
{
}

/*
oarchive &binary_oarchive::write_type(const char *tname)
{
		assert(m_is != NULL);
		(*m_is) << CLASS_CODE;
		(*m_is) << '{';

#ifdef __linux__		
		char *nnname = abi::__cxa_demangle( typeid(t).name()); 
		if (nnname == NULL) throw archive_format_exception();
		(*m_is) << nnname;
		free(nnname);
#else		
		(*m_is) << tname;
#endif

		(*m_is) << '}';	

		return (*this);
}
*/

 oarchive& binary_oarchive::write_start_tag(const std::string &name)
{
	return (*this);
}

 oarchive& binary_oarchive::write_end_tag(const std::string &name)
{
	return (*this);
}



oarchive &binary_oarchive::save_array(bool *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << BOOL_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(bool);
		int i;

		for (i=0;i<sz;i++)
		{
			if (val[i]==true)
				(*m_is).put('1');
			else
				(*m_is).put('0');
		}
			
		return (*this);
}
oarchive &binary_oarchive::save_array(unsigned char *val, const long sz)
{
	return save_array((char *)val, sz);
}

oarchive &binary_oarchive::save_binary(char *val, const long sz)
{
		assert(m_is != NULL);

		(*m_is) << BINARY_CODE;
		
		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of binary
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		(*m_is).write(val, sz);

		return (*this);
}


oarchive &binary_oarchive::save_array(char *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << CHAR_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(char);
		(*m_is).write(val, sz);
			
		return (*this);
}
oarchive &binary_oarchive::save_array(unsigned short *val, const long sz)
{
	return save_array((short *)val, sz);
}

oarchive &binary_oarchive::save_array(short *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << SHORT_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(short);
		long i=0;
		short pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(short));
			(*m_is).write( (char *)&pdata, sizeof(short));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save_array(unsigned int *val, const long sz)
{
	return save_array((int *)val, sz);
}

oarchive &binary_oarchive::save_array(int *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << INT_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(int);
		long i=0;
		int pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(int));
			(*m_is).write( (char *)&pdata, sizeof(int));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save_array(unsigned long *val, const long sz)
{
	return save_array((long *)val, sz);
}


oarchive &binary_oarchive::save_array(long *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << LONG_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(long);
		long i=0;
		long pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(long));
			(*m_is).write( (char *)&pdata, sizeof(long));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save_array(unsigned long long *val, const long sz)
{
	return save_array((long long *)val, sz);
}

oarchive &binary_oarchive::save_array(long long *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << LONG_LONG_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(long long);
		long i=0;
		long long pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(long long));
			(*m_is).write( (char *)&pdata, sizeof(long long));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save_array(float *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << FLOAT_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(float);
		long i=0;
		float pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(float));
			(*m_is).write( (char *)&pdata, sizeof(float));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save_array(double *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << DOUBLE_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(double);
		long i=0;
		double pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(double));
			(*m_is).write( (char *)&pdata, sizeof(double));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save_array(long double *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// write siz of array
		long d_sz = sz;
		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << LONG_DOUBLE_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(long double);
		long i=0;
		long double pdata;
		for (i=0;i<sz;i++)
		{
			pdata = val[i];
			change_endian((unsigned char *)&pdata, sizeof(long double));
			(*m_is).write( (char *)&pdata, sizeof(long double));
		}
			
		return (*this);
}

oarchive &binary_oarchive::save(char val)
{
		assert(m_is != NULL);
		(*m_is) << CHAR_CODE;
		
		char bsz = sizeof(val);

		(*m_is) << bsz;

		(*m_is) << val;

		return (*this);
}

oarchive &binary_oarchive::save(unsigned char val)
{
	return this->save( (char )val);	
}	

oarchive &binary_oarchive::save(bool val)
{
		assert(m_is != NULL);
		(*m_is) << BOOL_CODE;

		char bsz = sizeof(val);

		(*m_is) << bsz;

		if (val==true)
			(*m_is)<<(char)'1';
		else
			(*m_is)<<(char)'0';		

		return (*this);
}

oarchive &binary_oarchive::save(short val)
{
		assert(m_is != NULL);
		(*m_is) << SHORT_CODE;

		char bsz = sizeof(val);

		(*m_is) << bsz;

		short tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));

		return (*this);
}

oarchive &binary_oarchive::save(unsigned short val)
{
	this->save( (short )val);

	return (*this);
}

oarchive &binary_oarchive::save(int val)
{
		assert(m_is != NULL);
		(*m_is) << INT_CODE;
		char bsz = sizeof(val);

		(*m_is) << bsz;

		int tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));

		return (*this);
}

oarchive &binary_oarchive::save(unsigned int val)
{
		this->save( (int )val);
		return (*this);
}

oarchive &binary_oarchive::save(long val)
{
		assert(m_is != NULL);
		(*m_is) << LONG_CODE;
		char bsz = sizeof(val);

		(*m_is) << bsz;

		long tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));

		return (*this);
}

oarchive &binary_oarchive::save(unsigned long val)
{
		this->save( (long )val);

		return (*this);
}

oarchive &binary_oarchive::save(long long val)
{
		assert(m_is != NULL);
		(*m_is) << LONG_LONG_CODE;
		char bsz = sizeof(val);

		(*m_is) << bsz;

		long long tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));		

		return (*this);
}

oarchive &binary_oarchive::save(unsigned long long val)
{
		this->save( (long long )val);

		return (*this);
}


oarchive &binary_oarchive::save(float val)
{
		assert(m_is != NULL);
		(*m_is) << FLOAT_CODE;
		char bsz = sizeof(val);

		(*m_is) << bsz;

		float tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));

		return (*this);
		return (*this);
}

oarchive &binary_oarchive::save(double val)
{
		assert(m_is != NULL);
		(*m_is) << DOUBLE_CODE;
		char bsz = sizeof(val);

		(*m_is) << bsz;

		double tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));

		return (*this);
		return (*this);
}

oarchive &binary_oarchive::save(long double val)
{
		assert(m_is != NULL);
		(*m_is) << LONG_DOUBLE_CODE;
		char bsz = sizeof(val);

		(*m_is) << bsz;

		long double tval = val;

		change_endian((unsigned char *)&tval, sizeof(tval));

		(*m_is).write( (const char *)&tval, sizeof(tval));

		return (*this);

		return (*this);
}



oarchive &binary_oarchive::write_class_head(const std::string &name)
{
	assert(m_is != NULL);
	(*m_is) << CLASS_CODE;

	unsigned short d_sz = name.size();
	unsigned short sz = d_sz;
	change_endian((unsigned char *)&d_sz, sizeof(sz));
	(*m_is).write( (const char *)&d_sz, sizeof(sz));
	(*m_is).write(name.c_str(), sz);

	return (*this);
}


oarchive &binary_oarchive::write_class_end(const std::string &name)
{
		return (*this);
}

oarchive &binary_oarchive::write_array_head(long sz, char code, char bsz )
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		long d_sz = sz;

		(*m_is).put( (char)sizeof(d_sz));


		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));
		(*m_is).put(code);
		if (code != CLASS_CODE)
		{
			(*m_is).put(bsz);
		}

		return (*this);
}

oarchive &binary_oarchive::write_array_end(long sz, char code, char bsz )
{
		return (*this);
}


char uncode_to_utf8(wchar_t uc, char utf8[6]);

oarchive& binary_oarchive::save(wchar_t val)
{
		assert(m_is != NULL);
		(*m_is) << WCHAR_T_CODE;		

		// change wchar_t to utf8
		char utf8[6];
		char bsz = uncode_to_utf8(val, utf8);

		(*m_is) << bsz;

		int i;
		
		for (i=0;i<bsz;i++)
			(*m_is) << utf8[i];

		return (*this);
}

std::string UncodeStrToUTF8Str(const wchar_t *buffer, int len);

oarchive& binary_oarchive::save_array(wchar_t *val, const long sz)
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;

		char bsz = sizeof(sz);

		// write size byte of array size data
		(*m_is).put(bsz);

		// utf8 converting

		std::string utf8str = UncodeStrToUTF8Str(val, (int)sz);		

		// write siz of array
		long d_sz = utf8str.size();

		change_endian((unsigned char *)&d_sz, sizeof(d_sz));

		(*m_is).write( (const char *)&d_sz, sizeof(d_sz));

		// write array item code
		(*m_is) << WCHAR_T_CODE;

		// byte size of item
		(*m_is) << (char) sizeof(char); // utf8 
		(*m_is).write( utf8str.c_str(), utf8str.size());

		return (*this);
}