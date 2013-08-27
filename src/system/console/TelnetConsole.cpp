/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TelnetConsole.cpp
 *
 */


#include "TelnetConsole.h"

#include "TelnetConsoleWorker.h"

#include "system/Registry.h"
#include "system/io/IoManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

TelnetConsole::TelnetConsole()
{
}

TelnetConsole::~TelnetConsole()
{
}

bool TelnetConsole::open(const string &cnnHandler)
{
	IoManager *iom = portable_dynamic_cast(IoManager *, Registry::getRegistry()->getManager(IO_MGNAGER));
	if (iom != NULL) {
		setConnectionHandlerId(cnnHandler);
		iom->addConnectionHandler(this);
	}

	return true;
}

void TelnetConsole::close()
{
}

IoDirectWorker *TelnetConsole::createDirectWorker(IoConnection *cnn)
{
	return new TelnetConsoleWorker(cnn);
}

