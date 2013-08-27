/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Printable.cpp
 *
 */

#include "Printable.h"

#include <fstream>

using namespace std;

Printable::Printable()
{
}

Printable::~Printable()
{
}

void Printable::print(int level)
{
	ostream os(cout.rdbuf());
	print(os, level);
}

void Printable::print(const string &filename, int level)
{
	fstream fs(filename.c_str(), fstream::out);
	print(fs, level);
	fs.close();
}

void Printable::printLevel(ostream &os, int level)
{
	for (int i=0; i < level; ++i) os << '\t';
}

void Printable::printNV(ostream &os, int level, const string &name, const string &value)
{
	printLevel(os, level);
	os << name << "=" << value << '\n';
}

/*
void Printable::print(int level, ostream &os, char *name, string &value)
{
	printLevel(level, os);
	os << name << "=" << value << '\n';
}

void Printable::print(int level, ostream &os, char *name, char *value)
{
	printLevel(level, os);
	os << name << "=" << value << '\n';
}
*/
