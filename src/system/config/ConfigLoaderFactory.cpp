/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ConfigLoaderFactory.cpp
 *
 */
#include <string>

#include "system/util/Exceptions.h"

#include "ConfigLoaderFactory.h"
#include "ConfigLoader.h"

using namespace std;

ConfigLoaderFactory::ConfigLoaderFactory()
{
}

ConfigLoaderFactory::~ConfigLoaderFactory()
{
}

ConfigLoader *ConfigLoaderFactory::getConfigLoader(const string &cfgFile) throw (InvalidArgumentException)
{
	// XML loader : XMLConfigLoader
	// Property loader : PDFConfigLoader
	if (cfgFile.rfind(".props") != string::npos) {
		return &m_pdfLoader;
	}
	else if(cfgFile.rfind(".xml") != string::npos) {
		return &m_tinyXmlLoader;
	}
	else {
		return &m_tinyXmlLoader;
	}
}


