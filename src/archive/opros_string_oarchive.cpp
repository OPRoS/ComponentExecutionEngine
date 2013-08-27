/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_string_oarchive.cpp
 *
 *
 *
 *  OPROS output archive implementation
 */


#include <iostream>
#include <assert.h>
#include "archive/opros_oarchive.h"
#include "archive/opros_string_oarchive.h"

using namespace opros::archive;
std::string little_endian_hex_string(unsigned char *p, int size);

string_oarchive::string_oarchive(std::ostream *is, int mode) : m_is(is),m_mode(mode)
{
}

 oarchive& string_oarchive::write_start_tag(const std::string &name)
{
	return (*this);
}

 oarchive& string_oarchive::write_end_tag(const std::string &name)
{
	return (*this);
	}


oarchive &string_oarchive::save_array(bool *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

/**
** save character array
**/
oarchive &string_oarchive::save_array(unsigned char *val, const long sz)
{
	return save_array((char *)val, sz);
}

oarchive &string_oarchive::save_binary(char *val, const long sz)
{
	assert(m_is != NULL);
	(*m_is) << BINARY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	(*m_is).write(val, sz);
	(*m_is) << ';';	

	return (*this);
}

oarchive &string_oarchive::save_array(char *val, const long sz)
{
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	(*m_is) << CHAR_CODE;
	(*m_is).write(val, sz);
	(*m_is) << ';';	

	return (*this);
}

oarchive &string_oarchive::save_array(unsigned short *val, const long sz)
{
	return save_array((short *)val, sz);
}

oarchive &string_oarchive::save_array(short *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::save_array(unsigned int *val, const long sz)
{
	return save_array((int *)val, sz);
}

oarchive &string_oarchive::save_array(int *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::save_array(unsigned long *val, const long sz)
{
	return save_array((long *)val, sz);
}


oarchive &string_oarchive::save_array(long *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::save_array(unsigned long long *val, const long sz)
{
	return save_array((long long *)val, sz);
}


oarchive &string_oarchive::save_array(long long *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::save_array(float *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::save_array(double *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::save_array(long double *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}


oarchive &string_oarchive::save(char val)
{
		assert(m_is != NULL);
		(*m_is) << CHAR_CODE;

		int tval = val;

		if (m_mode)  // hexa
		{
			(*m_is) << 'X'<<std::hex << val;
		}
		else
		{
			(*m_is) << val;
		}		
		(*m_is) << ';';	


		return (*this);
}

oarchive &string_oarchive::save(unsigned char val)
{
	return this->save( (char)val);	
}	

oarchive &string_oarchive::save(bool val)
{
		assert(m_is != NULL);
		(*m_is) << BOOL_CODE;		

		if (val ==true)
			(*m_is) << 'T';
		else
			(*m_is) << 'F';	
			
		(*m_is) << ';';	


		return (*this);
}

oarchive &string_oarchive::save(short val)
{
		assert(m_is != NULL);
		(*m_is) << SHORT_CODE;

		if (m_mode)  // hexa
		{
			short temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';

		return (*this);
}

oarchive &string_oarchive::save(unsigned short val)
{
	this->save( (short)val);

	return (*this);
}

oarchive &string_oarchive::save(int val)
{
		assert(m_is != NULL);
		(*m_is) << INT_CODE;	

		if (m_mode)  // hexa
		{
			int temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';

		return (*this);
}

oarchive &string_oarchive::save(unsigned int val)
{
		this->save( (int )val);
		return (*this);
}

oarchive &string_oarchive::save(long val)
{
		assert(m_is != NULL);
		(*m_is) << LONG_CODE;

		if (m_mode)  // hexa
		{
			long temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';		

		return (*this);
}

oarchive &string_oarchive::save(unsigned long val)
{
		this->save( (long )val);

		return (*this);
}



oarchive &string_oarchive::save(long long val)
{
		assert(m_is != NULL);
		(*m_is) << LONG_LONG_CODE;

		if (m_mode)  // hexa
		{
			long long temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';		

		return (*this);
}

oarchive &string_oarchive::save(unsigned long long val)
{
		this->save( (long long)val);

		return (*this);
}


oarchive &string_oarchive::save(float val)
{
		assert(m_is != NULL);
		(*m_is) << FLOAT_CODE;

		if (m_mode)  // hexa
		{
			float temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';		

		return (*this);
}

oarchive &string_oarchive::save(double val)
{
		assert(m_is != NULL);
		(*m_is) << DOUBLE_CODE;

		if (m_mode)  // hexa
		{
			double temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';		

		return (*this);
}

oarchive &string_oarchive::save(long double val)
{
		assert(m_is != NULL);
		(*m_is) << LONG_DOUBLE_CODE;
		if (m_mode)  // hexa
		{
			long double temp = val;

			(*m_is) <<'X'<< little_endian_hex_string((unsigned char *)&temp, sizeof(temp)).c_str() << val;
		}
		else
		{
			(*m_is) << val;
		}
		
		(*m_is) << ';';		


		return (*this);
}

oarchive &string_oarchive::write_class_head(const std::string &name)
{
	assert(m_is != NULL);
	(*m_is) << CLASS_CODE;
	(*m_is) << '[';
	(*m_is) << name;
	(*m_is) << ']';

	return (*this);
}

oarchive &string_oarchive::write_class_end(const std::string &name)
{
	assert(m_is != NULL);
	(*m_is) << ';';

	return (*this);
}

oarchive &string_oarchive::write_array_head(long sz, char code, char bsz )
{
		assert(m_is != NULL);
		(*m_is) << ARRAY_CODE;
		(*m_is) << '[';
		(*m_is) << sz;
		(*m_is) << ']';

		return (*this);
}

oarchive &string_oarchive::write_array_end(long sz, char code, char bsz )
{
		assert(m_is != NULL);
		(*m_is) << ';';

		return (*this);
}

char uncode_to_utf8(wchar_t uc, char utf8[6]);

oarchive& string_oarchive::save(wchar_t val)
{
		assert(m_is != NULL);
		(*m_is) << WCHAR_T_CODE;		

		// change wchar_t to utf8
		char utf8[6];
		char sz = uncode_to_utf8(val, utf8);
		int i;
		if (m_mode)  // hexa
		{
			for (i=0;i<sz;i++)
				(*m_is) << std::hex << utf8[i];
		}
		else
		{
			for (i=0;i<sz;i++)
				(*m_is) << utf8[i];
		}
		
		(*m_is) << ';';		


		return (*this);
}

oarchive& string_oarchive::save_array(wchar_t *val, const long sz)
{
	long i;
	assert(m_is != NULL);
	(*m_is) << ARRAY_CODE;		
	(*m_is) << '[';
	(*m_is) << sz;
	(*m_is) << ']';
	for (i=0;i<sz;i++)
	{
		save(val[i]);
	}
	(*m_is) << ';';

	return (*this);
}