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

#include "ConfigLoader.h"
#include "Config.h"

using namespace std;
using namespace opros::engine;

ConfigLoader::ConfigLoader()
{
}

ConfigLoader::~ConfigLoader()
{
}

Config *ConfigLoader::loadFromFile(const string &cfgFilePath) throw (IOException)
{
	ifstream is;

	is.open(cfgFilePath.c_str(), ios::in);
	if (! is) {
		throw IOException("File not existed","ConfigLoader");
	}

	return loadFromStream(is);
}


void ConfigLoader::saveToFile(Config *cfg, const string &cfgFilePath) throw (IOException)
{
	ofstream os;

	os.open(cfgFilePath.c_str(), ios::out);
	if (! os) {
		throw IOException("File not existed","ConfigLoader");
	}

	saveToStream(cfg, os);
}


Config *ConfigLoader::loadFromStream(istream &is) throw (IOException)
{
	return NULL;
}

void ConfigLoader::saveToStream(Config *cfg, ostream &os) throw (IOException)
{
}
