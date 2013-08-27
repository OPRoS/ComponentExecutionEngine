/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Exceptions.cpp
 *
 */

#include "Exceptions.h"

using namespace std;

void Exception::print(ostream &os, int level)
{
	printLevel(os, level);
	os << m_msg << endl;
};
