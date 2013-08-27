/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSFile.cpp
 *
 */

#include "OSFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

bool OSFile::open(const string &filename, string &mode)
{
	//open file
	m_handle = ::fopen(filename.c_str(), mode.c_str());
	if( m_handle == NULL) return false;

	m_filename = filename;
	m_dir = filename;
	m_flag = mode;

	return true;
}

bool OSFile::close(void)
{
	if (m_handle != NULL) {
		::fclose(m_handle);
		m_handle = NULL;
		m_isClosed = true;
	}

	return true;
}

unsigned long OSFile::read(byte *data, unsigned long len)
{
	unsigned int ret = 0;
	unsigned int trial = 0;

	while (ret < len && trial <= FILE_IO_MAX_TRIAL) {
		ret += ::fread(data + ret, sizeof(byte), len - ret, m_handle);
		++trial;
	}
	return ret;
}

unsigned long OSFile::write(byte *data, unsigned long len)
{
	unsigned int trial = 0;
	unsigned int ret = 0;

	while (ret < len && trial <= FILE_IO_MAX_TRIAL) {
		ret += ::fwrite(data + ret, sizeof(byte), len - ret, m_handle);
		++trial;
	}

	return ret;
}
unsigned long OSFile::sizeOf(void)
{
	//get file size
	unsigned long cur;
	unsigned long fileSize;

	cur = ::fseek(m_handle , 0, SEEK_CUR);
	::fseek(m_handle , 0, SEEK_END);
	fileSize = ::ftell(m_handle);
	::fseek(m_handle, cur, SEEK_SET);

	/*
	::fseek(m_handle , 0, SEEK_END);
	fileSize = ::ftell(m_handle);
	::fseek(m_handle, 0, SEEK_SET);
	*/
	
	return fileSize;
}

