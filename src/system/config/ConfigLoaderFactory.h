/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ConfigLoaderFactory.h
 *
 */

#ifndef _CONFIG_LOADER_FACTORY_H
#define _CONFIG_LOADER_FACTORY_H

#include <string>

#include "system/util/Exceptions.h"

#include "ConfigLoader.h"
#include "system/config/props/PDFConfigLoader.h"
#include "system/config/tinyxml/TinyXmlConfigLoader.h"

/**
	파일의 확장자를 분석하여 적절한 ConfigLoader 클래스를 생성하는 클래스.
*/
class ConfigLoaderFactory
{
	PDFConfigLoader m_pdfLoader;
	TinyXmlConfigLoader m_tinyXmlLoader;

public:
	ConfigLoaderFactory();
	virtual ~ConfigLoaderFactory();

	/**
	 * Return a ConfigLoaderFactory instance for loading the cfgFile
	 * 확장자가 xml이면 TinyXMLConfigLoader, props이면 PDFConfigLoader 반환.
	 *
	 * @param cfgFile The name of the config file.
	 *
	 * @return ConfigLoaderFactory * the config loader instance
	 */
	ConfigLoader *getConfigLoader(const std::string  &cfgFile) throw (InvalidArgumentException);
};

#endif  
