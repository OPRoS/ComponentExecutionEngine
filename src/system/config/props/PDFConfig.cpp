/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 28
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PDFConfig.cpp
 *
 */

#include "PDFConfig.h"

using namespace std;
using namespace opros::engine;

PDFConfig::PDFConfig() {
}

PDFConfig::~PDFConfig() {
}

void PDFConfig::print(ostream &os)
{
	ConfigList children = getChildren();

	for (int i = 0; i < getLevel(); ++i) os << "\t";
	if (children.size() == 0) {
		os << getName() << "=" << getValue() << endl;
	}
	else {
		ConfigList::const_iterator iter = children.begin();
		for (; iter != children.end(); ++iter) {
			(*iter)->print(os);
		}
	}
}
