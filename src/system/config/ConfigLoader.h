/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ConfigLoader.h
 *
 */

#ifndef _CONFIG_LOADER_H
#define _CONFIG_LOADER_H

#include <iostream>
#include <string>

#include "system/util/BasicTypes.h"
#include "system/util/Exceptions.h"

#include "Config.h"

using namespace opros::engine;

/**
	XML 등의 설정 파일을 로딩하거나 파일로 저장하는 클래스.
*/
class ConfigLoader
{

public:
	ConfigLoader();
	virtual ~ConfigLoader();

	/**
	 * Load the config file(cfg) from the given file(cfgFile)
	 * and return the Config instance
	 *
	 * @param cfgFile The name of the config file.
	 *
	 * @return Config* the loaded config instance
	 */
	virtual Config *loadFromFile(const std::string  &cfgFile) throw (IOException);

	/**
	 * Save the config (cfg) to the given file(cfgFile)
	 *
	 * @param cfgFile The name of the config file.
	 * @param cfg     The config instance to be saved.
	 */
	virtual void saveToFile(Config *cfg, const std::string  &cfgFile) throw (IOException);

	/**
	 * Load the config file(cfg) from the given input stream(is)
	 * and return the Config instance
	 *
	 * @param cfgFile The name of the config file.
	 *
	 * @return Config* the loaded config instance
	 */
	virtual Config *loadFromStream(std::istream &is) throw (IOException);

	/**
	 * Save the config (cfg) to the given output stream(os)
	 *
	 * @param cfgFile The name of the config file.
	 * @param cfg     The config instance to be saved.
	 */
	virtual void saveToStream(Config *cfg, std::ostream &os) throw (IOException);

};

#endif  

