/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : EngineController.cpp
 *
 */


#include "EngineController.h"


#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"
#include "system/util/BasicTypes.h"

#include "Component.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/os/OSSetting.h"

#include <sstream> 
#include <iostream> 

using namespace std;

extern bool g_isTrace;
extern bool g_logSync;

EngineController::EngineController()
{
}

EngineController::~EngineController()
{
}

bool EngineController::init()
{
	trace_enter();

	m_registry = Registry::getRegistry();
	m_engine = (ComponentEngine *) m_registry->getManager(COMPONENT_ENGINE);
	if (m_engine == NULL) {
		return false;
	}

	return true;
}

bool EngineController::engineInit()
{
	return m_engine->init();
}

bool EngineController::engineStart()
{
	return m_engine->start();
}

bool EngineController::engineStop()
{
	return m_engine->stop();
}

bool EngineController::engineReset()
{
	return m_engine->reset();
}

bool EngineController::engineDestroy()
{
	return m_engine->destroy();
}

void EngineController::traceSet(bool isTrace)
{
	if (isTrace) g_isTrace = true;
	else g_isTrace = false;
}

void EngineController::logSetSync(bool isSync)
{
	if (isSync) g_logSync = true;
	else g_logSync = false;
}

