/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 10. 26
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServiceConsole.cpp
 *
 */


#include "ServiceConsole.h"

#include "ServiceConsoleWorker.h"

#include "system/Registry.h"
#include "system/io/IoManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

#ifdef 	MOD_PYTHON

#include <Python.h>
#include "ServiceConsoleEx.h"
#include <system/monitor/SymbolMonitor.h>
#include <system/monitor/MonitorManager.h>
#include <archive/opros_string_oarchive.h>


SymbolMonitor *m_monitor = NULL;

#endif //	MOD_PYTHON


ServiceConsole::ServiceConsole()
{

#ifdef 	MOD_PYTHON

	Py_Initialize();
	PyEval_InitThreads();

	m_provider = new PyServiceConsoleProvider();
	
#else
	m_provider = NULL;
#endif //	MOD_PYTHON
}

ServiceConsole::~ServiceConsole()
{
#ifdef 	MOD_PYTHON
	delete m_provider;

	PyEval_AcquireLock();
	Py_Finalize();
#endif //	MOD_PYTHON
}

bool ServiceConsole::open(const string &cnnHandler)
{
	IoManager *iom = portable_dynamic_cast(IoManager *, Registry::getRegistry()->getManager(IO_MGNAGER));
	if (iom != NULL) {
		setConnectionHandlerId(cnnHandler);
		iom->addConnectionHandler(this);
	}

	return true;
}

void ServiceConsole::close()
{
}

IoDirectWorker *ServiceConsole::createDirectWorker(IoConnection *cnn)
{
	return new ServiceConsoleWorker(cnn, m_provider);
}

