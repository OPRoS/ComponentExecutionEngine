/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : StringConverter.cpp
 *
 */

#include "StringConverter.h"

using namespace std;

StringConverter::StringConverter() {
}

StringConverter::~StringConverter() {
}

void StringConverter::test()
{
	StringConverter sm;

	try {
		string str("123");
		cout << "122 : " << sm.strToInt(str) << endl;
		cout << "true : " << sm.strToBool("true") << endl;
		cout << "12.3 : " << sm.strToFloat("12.3") << endl;
		cout << "xxx (int, default=456) : " << sm.strToInt(string("xxx"), 456) << endl;
		cout << "zzz (int) : " << sm.strToInt(string("xxx")) << endl;

		cout << "*********** end of StringManipulator::test()" << endl;
	}
	catch (InvalidArgumentException ex) {
		cout << "Exception : " << ex.getMessage() << endl;
	}
}

