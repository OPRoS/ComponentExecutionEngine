/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : StringUtil.cpp
 *
 */

#include <ctype.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>

#include "system/util/BasicTypes.h"
#include "StringUtil.h"

using namespace std;

string StringUtil::upper( const string& str )
{
    string upStr;

    for(uint i = 0; i < str.size(); i++) upStr += toupper(str[i]);

    return upStr;
}

string StringUtil::lower( const string& str )
{
    string lowStr;

    for(uint i = 0; i < str.size(); i++) lowStr += tolower(str[i]);

    return lowStr;
}

bool StringUtil::equalsNoCase( const string& s1, const string& s2 )
{
    string str1(upper(s1));
    string str2(upper(s2));

    return str1 == str2;
}

bool StringUtil::validateTicker( const string& ticker )
{
    if (ticker == "") return false;

    if (ticker.find(" ") != string::npos) return false;

    return true;
}

vector<string> StringUtil::split(const string& source, char separatorChar)
{
	vector<string> items;

	if (!source.size()) return items;

	string element( "" );
	char   ch;

	for (uint i = 0; i < source.size(); i++ ) {
		ch = source[ i ];

		if( ch == separatorChar ) {
			items.push_back( element );
			element = "";
		}
		else {
			element += ch;
		}
   }

   // if there's anything left over - that is, they don't have a
   // separator character for the last entry - add that as well.

   if( element.size() ) items.push_back( element );

   return items;
}

bool StringUtil::strToBool(const string &value) throw (InvalidArgumentException)
{
    if( isTrue(value) ) return true;
    else if( isFalse( value ) ) return false;
    else throw InvalidArgumentException("Invalid bool value : " + value);
}

bool StringUtil::strToBool(const string &value, bool defaultValue)
{
	try {
		return strToBool(value);
	}
	catch (InvalidArgumentException) {
		return defaultValue;
	}
}

bool StringUtil::isTrue(const string &value)
{
	string ivalue = StringUtil::lower(value);
	return ivalue == "true" || ivalue == "yes" 	|| ivalue == "on" || ivalue == "1";
}

bool StringUtil::isFalse(const string &value)
{
	string ivalue = StringUtil::lower(value);
	return ivalue == "false" || ivalue == "no" 	|| ivalue == "off" || ivalue == "0";
}

char StringUtil::strToChar(const string &value) throw (InvalidArgumentException)
{
	if (value.size() < 1) throw InvalidArgumentException("Invalid value");
	return value[0];
}

char StringUtil::strToChar(const string &value, char defaultValue)
{
	try {
		return strToChar(value);
	}
	catch (InvalidArgumentException) {
		return defaultValue;
	}
}

int StringUtil::strToInt(const string &value) throw (InvalidArgumentException)
{
	return atoi(value.c_str());
}

int StringUtil::strToInt(const string &value, int defaultValue)
{
	try {
		return strToInt(value);
	}
	catch (InvalidArgumentException) {
		return defaultValue;
	}
}

long StringUtil::strToLong(const string &value) throw (InvalidArgumentException)
{
	return atol(value.c_str());
}

long StringUtil::strToLong(const string &value, long defaultValue)
{
	try {
		return strToLong(value);
	}
	catch (InvalidArgumentException) {
		return defaultValue;
	}
}

float StringUtil::strToFloat(const string &value) throw (InvalidArgumentException)
{
	return (float) atof(value.c_str());
}

float StringUtil::strToFloat(const string &value, float defaultValue)
{
	try {
		return strToFloat(value);
	}
	catch (InvalidArgumentException) {
		return defaultValue;
	}
}

double StringUtil::strToDouble(const string &value) throw (InvalidArgumentException)
{
	return atof(value.c_str());
}

double StringUtil::strToDouble(const string &value, double defaultValue)
{
	try {
		return strToDouble(value);
	}
	catch (InvalidArgumentException) {
		return defaultValue;
	}
}

string StringUtil::boolToStr(bool value)
{
	if (value) return string("true");
	else return string("false");
}

string StringUtil::charToStr(char value)
{
	return string(1, value);
}

string StringUtil::intToStr(int value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

string StringUtil::longToStr(long value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

string StringUtil::floatToStr(float value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

string StringUtil::doubleToStr(double value)
{
	ostringstream buf;
	buf << value;
	return buf.str();
}

//-----------------------------------------------
// 파일이 확장자가 ext(예, ".xml")로 끝나는지 검사
//
bool StringUtil::findLastOf(const string &fn, const string &ext)
{
	// 파일 및 확장자의  마지막 인덱스 얻기
	int idx = fn.size() - 1;
	int extIdx = ext.size() - 1;

	// 확장자가 공백("")이면 무조건 true 리턴
	if (extIdx < 0) return true;

	// 파일 인덱스 값이 확장자 보다 작으면 확장자를 포함할 수 없으므로 false 리턴
	if (idx < extIdx) return false;

	// 뒤의 공백 제거
	while (fn.at(idx) == ' ' && idx >= 0) --idx;

	// 공백을 제거한 파일 인덱스 값이 확장자 보다 작으면 확장자를 포함할 수 없으므로 false 리턴
	if (idx < extIdx) return false;

	for (; idx >= 0 && extIdx >= 0; --idx, --extIdx) {
		if (fn.at(idx) != ext.at(extIdx)) return false;
	}

	// 파일이  확장자 모두를 포함하고 있지 않으면 false 리턴
	if (extIdx >= 0) return false;

	return true;
}

void StringUtil::replaceAll(string& text, const string& find_token, const string& replace_token)
{
	if (find_token == replace_token) return;

    size_t i = 0;
	while ( (i = text.find(find_token)) != string::npos ) {
        text.replace(i, find_token.size(), replace_token);
	}
}