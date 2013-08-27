/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : StringManipulator.h
 *
 */

#ifndef _STRING_CONVERTER_H_
#define _STRING_CONVERTER_H_

#include <string>
#include "Exceptions.h"
#include "StringUtil.h"

/**
	스트링을 다른 기본 타입으로 변환하거나 
	다른 기본 타입을 스트링으로 변환하는 클래스.
*/ 
class StringConverter
{
public:
	StringConverter();
	virtual ~StringConverter();

	/** 스트링을 소문자로 변환하는 함수. */
	std::string toLowercase(const std::string &value) { return StringUtil::lower(value); };

	/** 스트링을 대문자로 변환하는 함수. */
	std::string toUppercase(const std::string &value) { return StringUtil::upper(value); };

	/** 
		스트링이 true인지 판단하는 함수. 
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)
	*/
	bool isTrue(const std::string &value) { return StringUtil::isTrue(value); };

	/** 
		스트링이 false인지 판단하는 함수. 
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)
	*/
	bool isFalse(const std::string &value) { return StringUtil::isFalse(value); };

	/** bool 값을 string으로 변환하는 함수 */
	std::string boolToStr(bool value) 	{ return StringUtil::boolToStr(value); };

	/** char 값을 string으로 변환하는 함수 */
	std::string charToStr(char value) 	{ return StringUtil::charToStr(value); };

	/** int 값을 string으로 변환하는 함수 */
	std::string intToStr(int value) 		{ return StringUtil::intToStr(value); };

	/** long 값을 string으로 변환하는 함수 */
	std::string longToStr(long value) 	{ return StringUtil::longToStr(value); };

	/** float 값을 string으로 변환하는 함수 */
	std::string floatToStr(float value) 	{ return StringUtil::floatToStr(value); };

	/** double 값을 string으로 변환하는 함수 */
	std::string doubleToStr(double value) { return StringUtil::doubleToStr(value); };

	/** 
		string을 bool 값으로 변환하는 함수.
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return bool 변환된 값
	*/
	bool strToBool(const std::string &value) throw (InvalidArgumentException) { return StringUtil::strToBool(value); };

	/** 
		string을 bool 값으로 변환하는 함수.
		- true : "true", "on", "1", "yes" (대소문자 구별 안함)
		- false : "false", "off", "1", "no" (대소문자 구별 안함)

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return bool 변환된 값
	*/
	bool strToBool(const std::string &value, bool defaultValue) { return StringUtil::strToBool(value, defaultValue); };

	/** 
		string을 char 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return char 변환된 값
	*/
	char strToChar(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToChar(value); };

	/** 
		string을 char 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return char 변환된 값
	*/
	char strToChar(const std::string &value, char defaultValue) { return StringUtil::strToChar(value, defaultValue); };

	/** 
		string을 int 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return int 변환된 값
	*/
	int strToInt(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToInt(value); };

	/** 
		string을 int 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return int 변환된 값
	*/
	int strToInt(const std::string &value, int defaultValue) { return StringUtil::strToInt(value, defaultValue); };

	/** 
		string을 long 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return long 변환된 값
	*/
	long strToLong(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToLong(value); };

	/** 
		string을 long 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return long 변환된 값
	*/
	long strToLong(const std::string &value, long defaultValue)  { return StringUtil::strToLong(value, defaultValue); };

	/** 
		string을 float 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return float 변환된 값
	*/
	float strToFloat(const std::string &value) throw (InvalidArgumentException)  { return StringUtil::strToFloat(value); };

	/** 
		string을 float 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return float 변환된 값
	*/
	float strToFloat(const std::string &value, float defaultValue)  { return StringUtil::strToFloat(value, defaultValue); };

	/** 
		string을 double 값으로 변환하는 함수.
		인자가 잘못되었으면 InvalidArgumentException 예외 발생.

		@param value 변환할 string 값

		@return double 변환된 값
	*/
	double strToDouble(const std::string &value) throw (InvalidArgumentException) { return StringUtil::strToDouble(value); };

	/** 
		string을 double 값으로 변환하는 함수.

		@param value 변환할 string 값
		@param defaultValue 인자가 잘못되어 변환이 불가능할 때 반환할 값.

		@return double 변환된 값
	*/
	double strToDouble(const std::string &value, double defaultValue)  { return StringUtil::strToDouble(value, defaultValue); };

	void test();
};

#endif 
