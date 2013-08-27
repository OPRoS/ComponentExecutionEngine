/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : StringStream.cpp
 *
 */

#include "StringStream.h"

using namespace std;

StringStreamBuf::int_type StringStreamBuf::overflow(int_type i)
{
	if(!traits_type::eq_int_type(i, traits_type::eof())) {
		char_type c = traits_type::to_char_type(i);
		m_buf.push_back(c);
		if(c == '\n') flush();
	}

	return traits_type::not_eof(i);

}

StringStreamBuf::int_type StringStreamBuf::underflow()
{
	int result(traits_type::eof()) ;
	if (gptr() < egptr()) {
		result = *gptr() ;
	}
	else {
		result = m_stream->input_char();
		if (result != traits_type::eof()) {
			m_ibuf = result ;
			setg(&m_ibuf , &m_ibuf, &m_ibuf + 1) ;
		}
	}

	return result ;
}

void StringStreamBuf::flush()
{
	m_stream->output_string(m_buf);
	m_buf.clear();
}
