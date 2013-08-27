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
#include "archive/opros_binary_iarchive.h"

using namespace opros::archive;


iarchive& binary_iarchive::read_start_tag(std::string &name)
{
	return (*this);
}

iarchive& binary_iarchive::read_end_tag(std::string &name)
{
	return (*this);
}


iarchive &binary_iarchive::load_array(long double **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = DOUBLE_CODE;
		char bsz = 0;
		char mdata[40];
		long double pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new long double[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			memset(mdata, 0, 40); 
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(long double));
			(*val)[i] = pdata;		
		}

		return (*this);
}

iarchive &binary_iarchive::load_array(double **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = DOUBLE_CODE;
		char bsz = 0;
		char mdata[20];
		double pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new double[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			memset(mdata, 0, 20); 
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(double));
			(*val)[i] = pdata;		
		}

		return (*this);
}


iarchive &binary_iarchive::load_array(float **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = FLOAT_CODE;
		char bsz = 0;
		char mdata[20];
		float pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new float[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			memset(mdata, 0, 20); 
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(float));
			(*val)[i] = pdata;		
		}

		return (*this);
}

iarchive &binary_iarchive::load_array(unsigned long **val, long &sz)
{
	return load_array((long **)val,sz);
}

iarchive &binary_iarchive::load_array(long **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = LONG_CODE;
		char bsz = 0;
		char mdata[20];
		long pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new long[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			memset(mdata, 0, 20); 
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(long));
			(*val)[i] = pdata;		
		}

		return (*this);
}

iarchive &binary_iarchive::load_array(unsigned long long **val, long &sz)
{
	return load_array((long long **)val,sz);
}
iarchive &binary_iarchive::load_array(long long **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = LONG_LONG_CODE;
		char bsz = 0;
		char mdata[20];
		long long pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new long long[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			memset(mdata, 0, 20); 
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(long long));
			(*val)[i] = pdata;		
		}

		return (*this);
}

iarchive &binary_iarchive::load_array(unsigned int **val, long &sz)
{
	return load_array((int **)val, sz);
}

iarchive &binary_iarchive::load_array(int **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = INT_CODE;
		char bsz = 0;
		char mdata[10];
		int pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new int[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			memset(mdata, 0, 10); 
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(int));
			(*val)[i] = pdata;		
		}

		return (*this);
}

iarchive &binary_iarchive::load_array(unsigned short **val, long &sz)
{
	return load_array((short **)val, sz);
}

iarchive &binary_iarchive::load_array(short **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = SHORT_CODE;
		char bsz = sizeof(short);
		char mdata[10];
		short pdata;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new short[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}
		for (long i=0;i<tsz;i++)
		{
			(*m_is).read(mdata, bsz);
			change_endian((unsigned char *)mdata, bsz);
			memcpy(&pdata, mdata, sizeof(short));
			(*val)[i] = pdata;		
		}

		return (*this);

}

iarchive &binary_iarchive::load_array(bool **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;
		char code = BOOL_CODE;
		char bsz = sizeof(bool);
		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (sz==0 && *val==NULL)
		{
			*val = new bool[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		char ch;

		int i;
		for(i=0;i<tsz;i++) {
			ch = (*m_is).get();
			if (ch=='1') (*val)[i] = true;
			else (*val)[i] = false;

		}		

		return (*this);

}




iarchive &binary_iarchive::load_array(unsigned char **val, long &sz)
{
	return load_array((char **)val, sz);
}

iarchive &binary_iarchive::load_array(char **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=sz;		
		char code = CHAR_CODE;
		char bsz = sizeof(char);
		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();

		if (*val==NULL)
		{
			*val = new char[tsz];

			if (*val == 0) throw archive_system_exception();

			sz = tsz;
		}

		(*m_is).read(*val, tsz);		

		return (*this);

}


iarchive &binary_iarchive::load_binary(char **val, long &sz)
{
	assert(m_is != NULL);
	char c;
	char bsz=0;

	(*m_is) >> c;
	if ( c!= BINARY_CODE) throw archive_format_exception();

	(*m_is).get(bsz);

	long d_sz= 0;
	(*m_is).read((char *)&d_sz, bsz);

	change_endian( (unsigned char *)&d_sz, sizeof(d_sz));

	if (sz !=0)
	{
		if (sz != d_sz) throw archive_array_size_exception();
	}

	sz = d_sz;

	if (*val==NULL)
	{
			*val = new char[sz];

			if (*val == 0) throw archive_system_exception();
	}

	(*m_is).read(*val, sz);

	return (*this);
}

/*

iarchive &binary_iarchive::read_type(const char *tname)
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



iarchive &binary_iarchive::read_class_head(const std::string &name)
{
	assert( m_is != NULL);
	char c;

	(*m_is) >> c;
	if (c != CLASS_CODE) throw archive_format_exception();

	unsigned short nsz = 0;

	(*m_is).read( ( char *)&nsz, sizeof(short));

	change_endian( (unsigned char *)&nsz, sizeof(nsz));

	char in_name[300];

	(*m_is).read(in_name, nsz);
	in_name[nsz]=0;

	std::string tname;
	tname.assign(in_name);

	if (name.compare(tname))
	{		
		throw archive_type_exception();
	}

	return (*this);
}

iarchive &binary_iarchive::read_class_end(const std::string &name)
{
		assert(m_is != NULL);
		return (*this);
}

iarchive &binary_iarchive::read_array_head(long &sz, char &tcode, char &tbsz)
{
	assert(m_is != NULL);
	char c;

	char code;

	char bsz=0;

	(*m_is) >> c;
	if ( c!= ARRAY_CODE) throw archive_format_exception();

	(*m_is).get(bsz);

	long d_sz= 0;
	(*m_is).read((char *)&d_sz, bsz);

	change_endian( (unsigned char *)&d_sz, sizeof(d_sz));

	if (sz !=0)
	{
		if (sz != d_sz) throw archive_array_size_exception();
	}

	sz = d_sz;

	(*m_is).get(code);
	if (tcode !=0)
	{
		if (tcode != code) throw archive_format_exception();
	}

	tcode = code;

	if (code != CLASS_CODE)
	{
		(*m_is).get(bsz);
		if (tbsz !=0)
		{
			if (tbsz != bsz) throw archive_format_exception();			
		}
		
		tbsz = bsz;
	}	

	return (*this);
}

iarchive &binary_iarchive::read_array_end(long &sz, char &tcode, char &tbsz)
{
		assert(m_is != NULL);
		return (*this);
}


binary_iarchive::binary_iarchive(std::istream *is) : m_is(is)
{	
}


iarchive &binary_iarchive::load(bool &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;
		(*m_is) >> hcode; 
		if (hcode != BOOL_CODE) throw archive_type_exception();

		(*m_is) >> c; // byte size

		(*m_is) >> c;

		if (c=='1') val = true;
		else val = false;

		return (*this);
}

iarchive &binary_iarchive::load(short &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != SHORT_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		short pval;

		memcpy(&pval, mval, sizeof(short));

		val = pval;

		return (*this);
}

iarchive &binary_iarchive::load(unsigned short &val)
{
	return this->load((short &)val);
}


iarchive &binary_iarchive::load(int &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != INT_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		int pval;

		memcpy(&pval, mval, sizeof(int));

		val = pval;

		return (*this);
}

iarchive &binary_iarchive::load(unsigned int &val)
{
		return this->load((int &)val);
}


iarchive &binary_iarchive::load(long &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != LONG_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		long pval;

		memcpy(&pval, mval, sizeof(long));

		val = pval;

		return (*this);
}


iarchive &binary_iarchive::load(unsigned long &val)
{
		return this->load((long &)val);
}

iarchive &binary_iarchive::load(long long &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != LONG_LONG_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		long long pval;

		memcpy(&pval, mval, sizeof(long long));

		val = pval;

		return (*this);
}


iarchive &binary_iarchive::load(unsigned long long &val)
{
		return this->load((long long&)val);
}


iarchive &binary_iarchive::load(float &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != FLOAT_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		float pval;

		memcpy(&pval, mval, sizeof(float));

		val = pval;
		

		return (*this);
}

iarchive &binary_iarchive::load(double &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != DOUBLE_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		double pval;

		memcpy(&pval, mval, sizeof(double));

		val = pval;

		return (*this);
}

iarchive &binary_iarchive::load(long double &val)
{
		assert(m_is != NULL);

		char hcode;
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != LONG_DOUBLE_CODE) throw archive_type_exception();

		(*m_is) >> bsz;
		if (bsz > 20) throw archive_type_exception();

		char mval[20];
		memset(mval, 0, 20);

		(*m_is).read(mval, (int)bsz);

		change_endian((unsigned char *)mval,bsz);
		long double pval;

		memcpy(&pval, mval, sizeof(long double));

		val = pval;


		return (*this);
}

iarchive &binary_iarchive::load(char &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;		
		(*m_is) >> hcode; 
		if (hcode != CHAR_CODE) throw archive_type_exception();

		(*m_is) >> c;

		char vval=0;


		(*m_is).read( (char *)&vval, 1);
		val = vval;

		return (*this);
}

iarchive &binary_iarchive::load(unsigned char &val)
{
		return this->load((char &)val);
}

char utf8_to_unicode(char *utf8buf, wchar_t *uc);

iarchive &binary_iarchive::load(wchar_t &val)
{
		assert(m_is != NULL);

		char hcode;
		char c;		
		char bsz;
		(*m_is) >> hcode; 
		if (hcode != WCHAR_T_CODE) throw archive_type_exception();

		(*m_is) >> bsz;

		char utf8[6];

		(*m_is).read(utf8, bsz);

		wchar_t ox = 0;

		utf8_to_unicode(utf8, &ox);

		val = ox;

		return (*this);
}

iarchive &binary_iarchive::load_array(wchar_t **val, long &sz)
{
		assert(m_is != NULL);
		long tsz=0;		
		char code = WCHAR_T_CODE;
		char bsz = 0;
		
		
		int i;
		char uxsz;

		read_array_head(tsz, code, bsz);

		if (tsz <0) throw archive_format_exception();
	
		// tsz means size of char (utf8 string)

		char *utf8buf =  new char[tsz];

		if (utf8buf==0) throw archive_system_exception();

		(*m_is).read(utf8buf, tsz);  // read utf8 data 

		std::vector<wchar_t> outstr;

		for (i=0;i<tsz;)
		{
			wchar_t x = 0;

			uxsz= utf8_to_unicode( &utf8buf[i], &x);

			if (uxsz==0){
				delete utf8buf;
				throw archive_format_exception();
			}

			i+=uxsz;

			outstr.push_back(x);
		}

		delete utf8buf;

		if (outstr.size()!=0)
		{
				if (*val==NULL)
				{
					*val = new wchar_t[outstr.size()];

					if (*val == 0) throw archive_system_exception();

					sz = outstr.size();
				}
				else
				{
					if (sz != outstr.size()) throw archive_format_exception();
				}

				for (i=0;i< outstr.size();i++)
				{
					(*val)[i] = outstr[i];
				}
		}

		return (*this);
}