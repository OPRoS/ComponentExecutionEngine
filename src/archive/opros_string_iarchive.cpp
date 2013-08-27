/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
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
#include "archive/opros_string_iarchive.h"

using namespace opros::archive;



char hex_to_ch(char hex[2]);

iarchive& string_iarchive::read_start_tag(std::string &name)
{
	return (*this);
}

iarchive& string_iarchive::read_end_tag(std::string &name)
{
	return (*this);
}


string_iarchive::string_iarchive(std::istream *is) : m_is(is)
{
}

iarchive &string_iarchive::load_array(long double **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = LONG_DOUBLE_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new long double[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(double **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = DOUBLE_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new double[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(float **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = FLOAT_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new float[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(unsigned long **val, long &sz)
{
	return load_array( (long **)val, sz);
}

iarchive &string_iarchive::load_array(long **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = LONG_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new long[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(unsigned long long **val, long &sz)
{
	return load_array((long long **)val, sz);
}

iarchive &string_iarchive::load_array(long long **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = LONG_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new long long[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(unsigned int **val, long &sz)
{
	return load_array(( int **)val, sz);
}

iarchive &string_iarchive::load_array(int **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = INT_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new int[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(unsigned short **val, long &sz)
{
	return load_array(( short **)val, sz);
}

iarchive &string_iarchive::load_array(short **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = SHORT_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new short[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_array(bool **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = BOOL_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new bool[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load_binary(char **val, long &sz)
{
	assert(m_is != NULL);

	char c;
	long tsz;

	(*m_is) >>c;

	if (c != BINARY_CODE) throw archive_type_exception();

	(*m_is) >>c;

	if (c != '[') throw archive_format_exception();

	(*m_is) >>tsz;

	if (tsz <=0) throw archive_format_exception();

	if (sz >0)
	{
		if (sz != tsz) throw archive_format_exception();
	}

	(*m_is) >>c;

	if (c != ']') throw archive_format_exception();


	if (*val == NULL)
	{
		*val = new char[sz];

		if (*val == NULL) throw archive_system_exception();
	}

	(*m_is).read(*val, sz);

	(*m_is) >> c;
	if (c != ';') throw archive_format_exception();

	return (*this);
}

iarchive &string_iarchive::load_array(unsigned char **val, long &sz)
{
	return load_array(( char **)val, sz);
}

iarchive &string_iarchive::load_array(char **val, long &sz)
{
		assert(m_is != NULL);

		char c;
		long tsz = sz;
		char code = CHAR_CODE;
		char bsz = sizeof(char);

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		(*m_is) >> c;

		if (c != CHAR_CODE) throw archive_type_exception();

		if (tsz >0)
		{
			if (*val == NULL)
			{
				*val = new char[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			(*m_is).read(*val, tsz);
		}

		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}

/*

iarchive &string_iarchive::read_type(const char *tname)
{
		assert(m_is != NULL);
		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != CLASS_CODE) throw archive_type_exception() ;


		(*m_is) >>c;
		if ( c!= '{') throw archive_format_exception();

		char in_name[300];

		(*m_is).getline(in_name, 300, '}');

#ifdef CLASS_TYPE_CHECK

#ifdef __linux__
		char *eetemp = abi::__cxa_demangle( tname);

		if (eetemp == NULL) throw archive_format_exception();

		if (strcmp(in_name, eetemp))
		{
			free(eetemp);
			throw archive_type_exception();
		}
		else
			free(eetemp);


#else
		if (strcmp(in_name, tname))
		{
			throw archive_type_exception();
		}
#endif

#endif // CLASS_TYPE_CHECK

		(*m_is).unget();

		(*m_is) >>c;
		if (c!= '}') throw archive_format_exception();

		return (*this);
}
*/


iarchive &string_iarchive::read_class_head(const std::string &name)
{
	assert( m_is != NULL);
	char c;

	(*m_is) >> c;
	if (c != CLASS_CODE) throw archive_format_exception();
	(*m_is) >> c;
	if (c != '[') throw archive_format_exception();

	char in_name[300];

	(*m_is).getline(in_name, 300, ']');

	std::string tname;
	tname.assign(in_name);

	if (name.compare(tname))
	{
//		std::cout <<" Class name not matched:"<< tname.c_str()<<":"<<name.c_str()<<std::endl;
		throw archive_type_exception();
	}

//	std::cout <<"read class is "<<in_name << std::endl;


	(*m_is).unget();
	(*m_is) >> c;
	if (c != ']') throw archive_format_exception();

	return (*this);
}

iarchive &string_iarchive::read_class_end(const std::string &name)
{
		assert(m_is != NULL);
		char c;
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}


iarchive &string_iarchive::read_array_head(long &sz, char &tcode, char &tbsz)
{
	assert(m_is != NULL);
	char c;

	(*m_is) >> c;
	if ( c!= ARRAY_CODE) throw archive_format_exception();

	(*m_is) >> c;
	if ( c!= '[') throw archive_format_exception();

	long d_sz = 0;

	(*m_is) >> d_sz;

	if (sz !=0)
	{
		if (sz != d_sz) throw archive_array_size_exception();
	}

	sz = d_sz;

	(*m_is) >> c;

	if ( c!= ']') throw archive_format_exception();

	return (*this);
}

iarchive &string_iarchive::read_array_end(long &sz, char &tcode, char &tbsz)
{
		assert(m_is != NULL);
		char c;
		(*m_is) >> c;
		if (c != ';') throw archive_format_exception();

		return (*this);
}


iarchive &string_iarchive::load(bool &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != BOOL_CODE) throw archive_type_exception();

		(*m_is) >> c;
		if (c != 'T' && c != 'F') throw archive_format_exception();

		if (c == 'T') val = true;
		else val = false;

		(*m_is) >> c;

		if (c != ';') throw archive_format_exception();

		return (*this);
}

iarchive &string_iarchive::load(short &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != SHORT_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[10];

			(*m_is).getline(buf,10,';');
			(*m_is).unget();

			char out_buf[10];

			int len = strlen(buf);			

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			short out_val;

			memcpy(&out_val, out_buf, sizeof(short));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}


iarchive &string_iarchive::load(unsigned short &val)
{
	return this->load((short &)val);
}


iarchive &string_iarchive::load(int &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != INT_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[20];

			(*m_is).getline(buf,20,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			int out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}

iarchive &string_iarchive::load(unsigned int &val)
{
	return this->load((int &)val);
}



iarchive &string_iarchive::load(long &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != LONG_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[20];

			(*m_is).getline(buf,20,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			long out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}


iarchive &string_iarchive::load(unsigned long &val)
{
	return this->load((long &)val);
}

iarchive &string_iarchive::load(long long &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != LONG_LONG_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[40];

			(*m_is).getline(buf,40,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			long long out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}


iarchive &string_iarchive::load(unsigned long long &val)
{
	return this->load((long long &)val);
}

iarchive &string_iarchive::load(float &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != FLOAT_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[20];

			(*m_is).getline(buf,20,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			float out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}

iarchive &string_iarchive::load(double &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != DOUBLE_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[20];

			(*m_is).getline(buf,20,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			double out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}

iarchive &string_iarchive::load(long double &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != LONG_DOUBLE_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[20];

			(*m_is).getline(buf,20,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			long double out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}

iarchive &string_iarchive::load(char &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != CHAR_CODE) throw archive_type_exception();

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[20];

			(*m_is).getline(buf,20,';');
			(*m_is).unget();

			char out_buf[20];

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				char ch = hex_to_ch(&buf[i]);
				out_buf[j] = ch;
				j++;
				i++;				
			}

			char out_val;

			memcpy(&out_val, out_buf, sizeof(out_val));

			val = out_val;

		}
		else
		{
			(*m_is) >> val;
		}

		(*m_is) >> c;

		if (c !=';') throw archive_format_exception();


		return (*this);
}

iarchive &string_iarchive::load(unsigned char &val)
{
	return this->load((unsigned char &)val);
}

char utf8_to_unicode(char *utf8buf, wchar_t *uc);

iarchive &string_iarchive::load(wchar_t &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode;
		if (hcode != WCHAR_T_CODE) throw archive_type_exception();

		char utf8buf[6];

		c = (*m_is).peek();

		if (c=='X') // hexa code
		{
			char buf[25];	// 1char is 3byte and max 6, .. 18byte

			(*m_is).getline(buf,25,';');			
			(*m_is).unget();

			int len = strlen(buf);

			int i,j;

			for (i=1,j=0;i<len;i++) // start buf[0] = 'X'
			{
				if (j > 5) throw archive_format_exception();
				char ch = hex_to_ch(&buf[i]);
				utf8buf[j] = ch;
				j++;
				i++;				
				i++; // skip 'X'
			}
		}
		else
		{			
			char buf[10];

			(*m_is).getline(buf, 10, ';');
			(*m_is).unget();
			int len = strlen(buf);

			if (len > 6)  throw archive_format_exception(); // utf8 max size is 6
			memcpy(utf8buf, buf, len);
		}

		(*m_is) >> c;

		wchar_t ox = 0;

		utf8_to_unicode(utf8buf, &ox);

		val = ox;

		if (c !=';') throw archive_format_exception();		

		return (*this);
}

iarchive &string_iarchive::load_array(wchar_t **val, long &sz)
{
		char c;
		long tsz = sz;
		char code = WCHAR_T_CODE;
		char bsz = sizeof(char);
		

		read_array_head(tsz, code ,bsz);

		if (tsz <0) throw archive_format_exception();

		sz = tsz;

		if (tsz > 0)
		{
			long i;
			if (*val == NULL)
			{
				*val = new wchar_t[tsz];
				
				if (*val == 0) throw archive_system_exception();		
			}

			for(i=0;i<tsz;i++)
				load((*val)[i]);			
		}

		(*m_is) >> c;

		if (c != ';') throw archive_format_exception();

		return (*this);
}