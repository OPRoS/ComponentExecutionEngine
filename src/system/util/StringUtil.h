/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : StringUtil.h
 *
 */

#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <vector>

#include "Exceptions.h"

/**
	스트링을 다른 기본 타입으로 변환하는 등 스트링 관련 유틸리티 클래스.
*/ 
class StringUtil {
public:
	StringUtil();
	virtual ~StringUtil();

	/** the function converting the std::string to upper-case. **/
	static std::string upper(const std::string& str);

	/** the function converting the std::string to lower-case */
	static std::string lower(const std::string& str);

	/**
		the function returning true if the two strings are equal, but
		doesn't care about upper or lower case.
	*/
	static bool equalsNoCase(const std::string& s1, const std::string& s2);

	/**
		This function makes sure that Ticker is a reasonable ticker name. 
		Can't have spaces, be blank, etc.
	*/
	static bool validateTicker(const std::string& ticker);

	/**
		This function breaks a std::string up into the pieces separated by the
		supplied character. (e.g, StrSplit( "a,b", ',' ) would return
		a vector with "a" and "b" elements).
	*/
	static std::vector<std::string> split(const std::string& source, char separatorChar);

	/** 스트링을 소문자로 변환하는 함수. */
	static std::string toLowercase(const std::string &value);

	/** 스트링을 대문자로 변환하는 함수. */
	static std::string toUppercase(const std::string &value);

	/** 
		스트링이 true인지 판단하는 함수. 
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)
	*/
	static bool isTrue(const std::string &value);

	/** 
		스트링이 false인지 판단하는 함수. 
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)
	*/
	static bool isFalse(const std::string &value);

	/** bool 값을 string으로 변환하는 함수 */
	static std::string boolToStr(bool value);

	/** char 값을 string으로 변환하는 함수 */
	static std::string charToStr(char value);

	/** int 값을 string으로 변환하는 함수 */
	static std::string intToStr(int value);

	/** long 값을 string으로 변환하는 함수 */
	static std::string longToStr(long value);

	/** float 값을 string으로 변환하는 함수 */
	static std::string floatToStr(float value);

	/** double 값을 string으로 변환하는 함수 */
	static std::string doubleToStr(double value);

	/** 
		string을 bool 값으로 변환하는 함수.
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return bool 변환된 값
	*/
	static bool strToBool(const std::string &value) throw (InvalidArgumentException);

	/** 
		string을 bool 값으로 변환하는 함수.
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return bool 변환된 값
	*/
	static bool strToBool(const std::string &value, bool defaultValue);

	/** 
		string을 char 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return char 변환된 값
	*/
	static char strToChar(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string을 char 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return char 변환된 값
	*/
	static char strToChar(const std::string &value, char defaultValue);

	/** 
		string을 int 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return int 변환된 값
	*/
	static int strToInt(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string을 int 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return int 변환된 값
	*/
	static int strToInt(const std::string &value, int defaultValue);

	/** 
		string을 long 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return long 변환된 값
	*/
	static long strToLong(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string을 long 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return long 변환된 값
	*/
	static long strToLong(const std::string &value, long defaultValue);

	/** 
		string을 float 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return float 변환된 값
	*/
	static float strToFloat(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string을 float 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return float 변환된 값
	*/
	static float strToFloat(const std::string &value, float defaultValue);

	/** 
		string을 double 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return double 변환된 값
	*/
	static double strToDouble(const std::string &value) throw (InvalidArgumentException) ;

	/** 
		string을 double 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return double 변환된 값
	*/
	static double strToDouble(const std::string &value, double defaultValue);

	/** 
		std::string의 마지막에 특정문자로 끝나는 지 판단하는 함수 

		@param fn 검사할 원본 데이터 스트링
		@param ext 포함하고 있는 지 검사할 스트링
	*/
	static bool findLastOf(const std::string &fn, const std::string &ext);

	static void replaceAll(std::string& text, const std::string& find_token, const std::string& replace_token);

};

#endif 
