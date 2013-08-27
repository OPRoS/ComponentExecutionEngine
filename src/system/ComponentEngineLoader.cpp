/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentEngineLoader.cpp
 *
 */

#include <iostream>

#include "ComponentEngineLoader.h"

#include "Registry.h"
#include "SystemTypes.h"
#include "ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"

#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/deploy/PackageDeployer.h"

#include "Component.h"

#include "system/control/ControllerManager.h"
#include "system/control/EngineController.h"
#include "system/control/AppController.h"
#include "system/control/ComponentController.h"

#include "system/os/OSSetting.h"
#include "system/util/LifecycleObject.h"

#include "system/console/ConsoleProcessor.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

extern bool g_isTrace;
extern bool g_logSync;

ComponentEngineLoader::ComponentEngineLoader()
{
	trace_enter();
	m_appName = "LaserApp";
}

ComponentEngineLoader::~ComponentEngineLoader()
{
}

void ComponentEngineLoader::toProperties(Properties &props, int argc, char *argv[])
{
	//--------------------------------------------------
	// convert the command line arguments to properties
	//
	cout << "\ncommand line---------------------" << "\r\n";
	for (int i = 0; i < argc; ++i) {
		string str(argv[i]);

		cout << (i+1) << ". " << str << "\r\n";

		size_t idx = str.find_first_of('=');
		if (idx != string::npos) {
			string name = str.substr(0, idx);
			string value = str.substr(idx+1);
			if (name == "engine.path") {
				props.removeProperty("engine.dir");
				props.setProperty("engine.dir", value);
			}
			else {
				props.removeProperty(name);
				props.setProperty(name, value);
			}
		}
	}
	cout << "----------------------------------------" << "\r\n";
}

bool ComponentEngineLoader::load(int argc, char *argv[])
{
	cout << "[ComponentEngineLoader::load] ComponentEngine Initing..." << "\r\n";

	Properties props;
	props.setProperty("engine.dir", ".");
	props.setProperty("cfg.file", "system.xml");
	toProperties(props, argc, argv);

	return load(props);
}

bool ComponentEngineLoader::load(Properties &props)
{

	//-------------------------
	// Registry »ý¼º

	Registry *reg = Registry::getRegistry();
	reg->setComponentEngineLoader(this);

	//-------------------------------
	// setting system properties
	//
	reg->setSystemProperties(props);

	//cout << "\nsystem properties---------------------\n";
	//cout << props;
	//cout << "----------------------------------------\n";

	cout << "\nsystem properties---------------------\n";
	cout << "engine dir : " << reg->getEngineDir() << '\n';
	cout << "deploy dir : " << reg->getDeployDir() << '\n';
	cout << "log    dir : " << reg->getLogDir() << '\n';
	cout << "config dir : " << reg->getConfigDir() << '\n';
	cout << "system config filename  : " << reg->getSysConfigFileName() << '\n';
	cout << "system config file path : " << reg->getSysConfigFilePath() << '\n';
	cout << "----------------------------------------\n\n";

	//------------------------
	// load system.xml
	//
	cout << "[ComponentEngineLoader::load] ConfigLoaderFactory loading...\n";
	ConfigLoaderFactory *clFac = new ConfigLoaderFactory();
	reg->setConfigLoaderFactory(clFac);
	ConfigLoader *loader = clFac->getConfigLoader(reg->getSysConfigFileName());
	if (loader == NULL) {
		cout << "[ComponentEngineLoader::load] ConfigLoader is NULL : cfg.path=" << reg->getSysConfigFilePath() << '\n';
		return false;
	}
	cout << "[ComponentEngineLoader::load] ConfigLoaderFactory loaded...\n";

	cout << "[ComponentEngineLoader::load] Config loading : cfg.path=" << reg->getSysConfigFilePath() << '\n';
	Config *cfg = loader->loadFromFile(reg->getSysConfigFilePath());
	if (cfg == NULL) {
		cout << "[***ComponentEngineLoader::load***] Cannot load system config : cfg.path=" << reg->getSysConfigFilePath() << '\n';
		return false;
	}
	// trace config
	//cout << "\nsystem config ----------------------------------------\n";
	// trace(cfg);
	//cout << cfg;
	//cout << "--------------------------------------------------------\n";

	//-------------------
	// create Engine
	//
	ComponentEngine *engine = new ComponentEngine();
	engine->setName(COMPONENT_ENGINE);
	reg->setManager(COMPONENT_ENGINE, engine);
	engine->setConfig(cfg);
	cfg = engine->getSystemConfig();
	if (cfg == NULL) {
		cout << "[ComponentEngineLoader::load] Config is not system config : cfg.path=" << reg->getSysConfigFilePath() << '\n';
		return false;
	}
	reg->setNodeId(cfg->getChildValue("loca_node_id", "__local_node__"));

	//---------------------------------------------------
	// load user defined system properties in system.xml
	//
	Config *propsCfg = cfg->getChild("system_props");
	if (propsCfg != NULL) {
		propsCfg->toProperties(*(reg->getSystemProperties()));
	}
	g_isTrace = reg->getSystemProperties()->getBool("trace", false);
	m_appName = reg->getSystemProperties()->getProperty("app.default", m_appName);
	bool sysConsole = reg->getSystemProperties()->getBool("sys.console", true);

	string dir = reg->getSystemProperties()->getProperty("deploy.dir", "");
	if (dir != "") {
		reg->toDeployDir(dir);
	}

	dir = reg->getSystemProperties()->getProperty("config.dir", "");
	if (dir != "") {
		reg->toConfigDir(dir);
	}

	dir = reg->getSystemProperties()->getProperty("log.dir", "");
	if (dir != "") {
		reg->toLogDir(dir);
	}

	cout << "\nsystem properties---------------------\n";
	cout << "engine dir : " << reg->getEngineDir() << '\n';
	cout << "deploy dir : " << reg->getDeployDir() << '\n';
	cout << "log    dir : " << reg->getLogDir() << '\n';
	cout << "config dir : " << reg->getConfigDir() << '\n';
	cout << "system config filename  : " << reg->getSysConfigFileName() << '\n';
	cout << "system config file path : " << reg->getSysConfigFilePath() << '\n';
	cout << "----------------------------------------\n\n";

	//------------------------
	// logger setting
	//
	cout << "[ComponentEngineLoader::load] LoggerFactory loading...\n";
	// LoggerFactory loading
	LoggerFactory *loggerFac = new LoggerFactory();
	reg->setLoggerFactory(loggerFac);
	loggerFac->init(cfg->getChild("logger_factory"));
	loggerFac->setDefaultLoggerName("system.logger");
	reg->setSystemLogger(loggerFac->getLogger("system.logger"));
	cout << "[ComponentEngineLoader::load] LoggerFactory loaded...\n\n";

	//------------------------
	// engine initialize
	//
	if (! engine->init()) {
		cout << "Engine init failed. Exit the program..." << "\r\n";
		return false;
	}
	else {
		cout << "Engine init success..." << "\r\n";
	}

	//------------------------
	// engine start
	//
	if (! engine->start()) {
		cout << "Engine start failed. Exit the program..." << "\r\n";
		return false;
	}
	else {
		cout << "Engine start success..." << "\r\n";
	}

	//------------------------------
	// console command processing
	//
	if (sysConsole) {
		ConsoleProcessor cei;
		cei.processConsoleCommand();
	}
	else {
		bool waitFinished = reg->getSystemProperties()->getBool("sys.waitFinished", true);
		
		if (waitFinished) {
			// wait until the engine is finished
			engine->waitFinished();
		}
	}

	return true;
}
