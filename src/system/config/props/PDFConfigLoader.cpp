/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ConfigLoader.cpp
 *
 */
#include <string>
#include <iostream>
#include <fstream>

#include "system/util/Exceptions.h"

#include "PDFConfigLoader.h"
#include "PDFConfig.h"

using namespace std;
using namespace opros::engine;

PDFConfigLoader::PDFConfigLoader()
{
}

PDFConfigLoader::~PDFConfigLoader()
{
}

Config *PDFConfigLoader::loadFromStream(istream &is) throw (IOException)
{
	PDFConfig *cfg = new PDFConfig();
	char line[256];
	size_t idx;
	char ch = 'a';
	bool isSkipLine;

	idx = 0;
	isSkipLine = false;
	while (is.get(ch)) {
		// skip blanks
		if (ch == ' ' || ch == '\t') continue;
		if (ch == '#') {
			isSkipLine = true;
			continue;
		}

		// detect end of line
		if (ch == '\n' || ch == '\r') {
			if (!isSkipLine && idx > 3) {
				line[idx] = '\0';
				Config *child = parsingLine(string(line));
				if (child != NULL) cfg->addChild(child);
			}
			idx = 0;
			isSkipLine = false;
		}
		else if (!isSkipLine) line[idx++] = ch;
	}

	// detect end of the last line
	if (!isSkipLine && idx > 3) {
		line[idx] = '\0';
		Config *child = parsingLine(string(line));
		if (child != NULL) cfg->addChild(child);
	}
	return cfg;
}

Config *PDFConfigLoader::parsingLine(string line)
{
	string::size_type pos = line.find('=');
	if (pos == string::npos || pos == 0) return NULL;

	string name = line.substr(0, pos);
	string value = line.substr(pos+1);

	return new PDFConfig(name, value);
}

void PDFConfigLoader::saveToStream(Config *cfg, ostream &os) throw (IOException)
{
	ConfigList children = cfg->getChildren();
	ConfigList::const_iterator iter = children.begin();

	for (; iter != children.end(); ++iter) {
		os << (*iter)->getName() << "=" << (*iter)->getValue() << '\n';
	}
	os.flush();
}
