/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_archive.h
 *
 *
 *
 *  Archive class for serialization support
 */
#include <sys/types.h>
#include <string>
#include <archive/opros_archive_common.h>
#include <archive/opros_oarchive.h>
#include <archive/opros_iarchive.h>
#include <archive/opros_oarchive_stl.h>
#include <archive/opros_iarchive_stl.h>

namespace opros {
	namespace archive {


char type_code(bool *val)
{
	return BOOL_CODE;
}

char type_code(short *val)
{
	return SHORT_CODE;
}

char type_code(unsigned short *val)
{
	return UNSIGNED_SHORT_CODE;
}

char type_code(int *val)
{
	return INT_CODE;
}

char type_code(unsigned int *val)
{
	return UNSIGNED_INT_CODE;
}

char type_code(long *val)
{
	return LONG_CODE;
}

char type_code(unsigned long *val)
{
	return UNSIGNED_LONG_CODE;
}

char type_code(long long *val)
{
	return LONG_LONG_CODE;
}

char type_code(unsigned long long *val)
{
	return UNSIGNED_LONG_LONG_CODE;
}


char type_code(float *val)
{
	return FLOAT_CODE;
}

char type_code(double *val)
{
	return DOUBLE_CODE;
}

char type_code(long double *val)
{
	return LONG_DOUBLE_CODE;
}

	
char type_code(char *val)
{
	return CHAR_CODE;
}


char type_code(signed char *val)
{
	return SIGNED_CHAR_CODE;
}

char type_code(unsigned char *val)
{
	return UNSIGNED_CHAR_CODE;
}

	} // namespace archive 
} // amespace opros


void save_this(opros::archive::oarchive &ar, std::string &t)
{
	unsigned long sz = t.length();
	ar << OPROS_SERIALIZATION_NVP(sz);


	ar.write_start_tag("str");

	ar.save_binary((char *)t.c_str(), sz);

	ar.write_end_tag("str");
}



void load_this(opros::archive::iarchive &ar, std::string &t)
{
	long sz;

	ar >> OPROS_SERIALIZATION_NVP(sz);

	if (sz <0) throw opros::archive::archive_format_exception();

	char *dp=0;
	
	std::string name;

	ar.read_start_tag(name);
	
	ar.load_binary(&dp, sz);

	ar.read_end_tag(name);

	t.assign(dp,sz);

	delete dp;
}


std::string char_to_hex(char ch)
{
	int upper = (ch >> 4) & 0x0f;
	int lower = ch & 0x0f;
	char chr[3];

	chr[0] = (upper > 9)? 'A' + (upper - 10) : '0' + upper;
	chr[1] = (lower > 9)? 'A' + (lower - 10) : '0' + lower;
	chr[2] = 0;

	std::string out=chr;

	return out;
}


std::string little_endian_hex_string(unsigned char *p, int size)
{
	unsigned short check = 12345;  // 0x3039
	unsigned char *x = (unsigned char *)&check;

	std::string outstring;

	if (x[0] == 0x30) // big endian
	{
		int i;
		for (i=0;i<size;i++)
		{	
			outstring.append(char_to_hex(p[size-1-i]));
		}
	}
	else // little endian
	{
		int i;
		for (i=0;i<size;i++)
		{	
			outstring.append(char_to_hex(p[i]));
		}
	}

	return outstring;
}


char hex_to_ch(char hex[2])
{
	char ch_upper=0, ch_lower = 0;

	if (hex[0]>='0' && hex[0] <='9') ch_upper = hex[0] = '0';
	else if (hex[0] >='A' && hex[0] <='F') ch_upper = hex[0] - 'A' + 10;

	if (hex[1]>='0' && hex[1] <='9') ch_upper = hex[1] = '0';
	else if (hex[1] >='A' && hex[1] <='F') ch_upper = hex[1] - 'A' + 10;

	char result;

	result = (ch_upper << 4) + ch_lower;

	return result;
}

std::string string_to_xml_string(const std::string &str)
{
	const char *p = str.c_str();
	int i;
	int sz = str.size();
	std::string out;
	char ch;

	for (i=0;i < sz; i++)
	{		
		ch = p[i];
		if (ch=='<')
		{
			out.append("&lt;");
		}
		else if (ch== '>')
		{
			out.append("&gt;");
		}
		else if (ch=='&&')
		{
			out.append("&amp;");
		}
		else if (ch=='\'')
		{
			out.append("&apros;");
		}
		else if (ch=='\"')
		{
			out.append("&quot;");
		}
		else
		{
			out+=ch;
		}
	}

	return out;
}




char utf8_to_unicode(char *utf8buf, wchar_t *uc)
{
	wchar_t ox;	
	unsigned char c = (unsigned char)utf8buf[0];

	if (c < 0x7f)
	{
		ox = (wchar_t) utf8buf[0];
		*uc = ox;

		return 1;
	}
	else if ((c & 0xe0)== 0xc0)
	{
		ox = (c & 0x1f);
		ox = ox << 6;
		ox = ox | (wchar_t)( utf8buf[1] & 0x3f);
		*uc = ox;

		return 2;
	}
	else if ((c & 0xf0)== 0xe0)
	{
		ox = (c & 0x0f);
		ox = ox << 12;
		ox = ox | ((wchar_t)( utf8buf[1] & 0x3f) << 6 ) ;
		ox = ox | ((wchar_t)( utf8buf[2] & 0x3f));
		*uc = ox;

		return 3;
	}
#ifdef __GNUC__
	else if ((c & 0xf8)== 0xf0)
	{
		ox = (c & 0x07);
		ox = ox << 18;
		ox = ox | ((wchar_t)( utf8buf[1] & 0x3f) << 12 ) ;
		ox = ox | ((wchar_t)( utf8buf[2] & 0x3f) << 6 ) ;
		ox = ox | ((wchar_t)( utf8buf[3] & 0x3f));
		*uc = ox;

		return 4;
	}
	else if ((c & 0xfc)== 0xf8)
	{
		ox = (c & 0x03);
		ox = ox << 24;
		ox = ox | ((wchar_t)( utf8buf[1] & 0x3f) << 18 ) ;
		ox = ox | ((wchar_t)( utf8buf[2] & 0x3f) << 12 ) ;
		ox = ox | ((wchar_t)( utf8buf[3] & 0x3f) << 6 ) ;
		ox = ox | ((wchar_t)( utf8buf[4] & 0x3f));
		*uc = ox;

		return 5;
	}
	else if ((c & 0xfe)== 0xfc)
	{
		ox = (c & 0x01);
		ox = ox << 30;
		ox = ox | ((wchar_t)( utf8buf[1] & 0x3f) << 24 ) ;
		ox = ox | ((wchar_t)( utf8buf[2] & 0x3f) << 18 ) ;
		ox = ox | ((wchar_t)( utf8buf[3] & 0x3f) << 12 ) ;
		ox = ox | ((wchar_t)( utf8buf[4] & 0x3f) << 6 ) ;
		ox = ox | ((wchar_t)( utf8buf[5] & 0x3f));
		*uc = ox;

		return 6;
	}	
#endif
	return 0;
}


/*
** author : sby
** RFC2044
*/
inline char uncode_to_utf8(wchar_t uc, char utf8buf[6])
{
	if (uc <= 0x7f)  // utf16
	{		
		utf8buf[0] = (char) uc;

		return 1;
	}
	else if (uc <= 0x7ff) 
	{
		utf8buf[0] = (char) 0xc0 | (char)((uc >> 6));
		utf8buf[1] = (char) 0x80 | (char)(uc & 0x3F);
		return 2;
	}
	else if (uc <= 0xffff)
	{
		utf8buf[0] = (char) 0xe0 + (char)(uc >> 12);
		utf8buf[1] = (char) 0x80 + (char)((uc  >> 6) & 0x3f );
		utf8buf[2] = (char) 0x80 + (char)(uc & 0x3f);

		return 3;
	}

#ifdef __GNUC__
	else if (uc <=0x1fffff)
	{
		utf8buf[0] = (char) 0xf0  + (char)(uc >> 18);
		utf8buf[1] = (char) 0x80 + (char)((uc >> 12) & 0x3f);
		utf8buf[2] = (char) 0x80 + (char)((uc >> 6) & 0x3f);
		utf8buf[3] = (char) 0x80 + (char)(uc & 0x3f);

		return 4;
	}
	else if (uc <=0x3ffffff)
	{
		utf8buf[0] = (char) 0xf8  + (char)(uc >> 24);
		utf8buf[1] = (char) 0x80 + (char)((uc >> 18) & 0x3f);
		utf8buf[2] = (char) 0x80 + (char)((uc >> 12) & 0x3f);
		utf8buf[3] = (char) 0x80 + (char)((uc >> 6) & 0x3f);
		utf8buf[4] = (char) 0x80 + (char)(uc & 0x3f);

		return 5;
	}
	else if (uc <=0x7fffffff)
	{
		utf8buf[0] = (char) 0xfC  + (char)(uc >> 30);
		utf8buf[1] = (char) 0xf8  + (char)((uc >> 24) & 0x3f);
		utf8buf[2] = (char) 0x80 + (char)((uc >> 18) & 0x3f);
		utf8buf[3] = (char) 0x80 + (char)((uc >> 12) & 0x3f);
		utf8buf[4] = (char) 0x80 + (char)((uc >> 6) & 0x3f);
		utf8buf[5] = (char) 0x80 + (char)(uc & 0x3f);

		return 6;
	}
#endif

	return 0;
}


std::string UncodeStrToUTF8Str(const wchar_t *buffer, int len)
{
	int i;
	char utf8buf[6];
	char bsz=0;

	std::string outbuf;

	for (i=0;i<len;i++)
	{
		bsz = uncode_to_utf8(buffer[i], utf8buf);

		if (bsz>0) outbuf.append(utf8buf, bsz);
	}

	return outbuf;
}

std::string wstring_to_utf8(const std::wstring &str)
{
	return UncodeStrToUTF8Str(str.c_str(), (int)str.size());
}

// 2012.07.09 wstring 직렬화코드 추가함

void save_this(opros::archive::oarchive &ar, std::wstring &t)
{
	std::string utf8_str;

	utf8_str = wstring_to_utf8(t);

	unsigned long sz = utf8_str.length();

	ar << OPROS_SERIALIZATION_NVP(sz);

	ar.write_start_tag("wstr");

	ar.save_binary((char *)utf8_str.c_str(), sz);

	ar.write_end_tag("wstr");
}


void load_this(opros::archive::iarchive &ar, std::wstring &t)
{
	long sz;

	ar >> OPROS_SERIALIZATION_NVP(sz);

	if (sz <0) throw opros::archive::archive_format_exception();

	char *dp=0;
	
	std::string name;

	ar.read_start_tag(name);
	
	ar.load_binary(&dp, sz);

	ar.read_end_tag(name);

	int i;
	char uxsz;

	for (i=0; i<sz; )
	{
		wchar_t x = 0;

		uxsz = utf8_to_unicode( (char *)(dp+i), &x);

		if (uxsz == 0)
		{
			delete dp;
			throw opros::archive::archive_format_exception();
		}

		i+=uxsz;

		t.push_back(x);
	}

	delete dp;
}
