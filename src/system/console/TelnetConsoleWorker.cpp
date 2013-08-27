/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TelnetConsoleWorker.cpp
 *
 */

#include <iostream>

#include "TelnetConsoleWorker.h"
#include "TelnetConsole.h"
#include "ConsoleProcessor.h"

#include "system/Registry.h"
#include "system/SystemTypes.h"
#include "system/ComponentEngine.h"

#include "system/config/ConfigLoaderFactory.h"
#include "system/util/Properties.h"
#include "system/util/StringUtil.h"
#include "system/util/BasicTypes.h"

#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentManager.h"
#include "system/cm/ComponentAdapter.h"
#include "system/cm/ComponentAdapterAtomic.h"
#include "system/deploy/PackageDeployer.h"
#include "Component.h"
#include "system/os/OSSetting.h"

#include "system/Log.h"
#include "system/Trace.h"


extern bool g_isTrace;
extern bool g_logSync;

using namespace std;

TelnetConsoleWorker::TelnetConsoleWorker(IoConnection *cnn) : IoDirectWorker(cnn)
{
}

TelnetConsoleWorker::~TelnetConsoleWorker()
{
}

void TelnetConsoleWorker::processWork() throw (IOException)
{
	trace_enter();

	char line[128];
	char cmd[128];
	bool isDone = false;

	ConsoleProcessor cei(this, this);
	cei.printHelp();

	while (!isDone) {
		writeString("\r\ncommand >> ");

		readLine(line, 80);

		// 앞쪽 공백 제거
		int idx = 0;
		while (line[idx] != '\0') {
			if (line[idx] == ' ' || line[idx] == '\t' || line[idx] == '\r' || line[idx] == '\n') ;
			else break;
			idx++;
		}

		int pos = 0;
		while (line[idx] != '\0') cmd[pos++] = line[idx++];
		cmd[pos] = '\0';

		// 뒤쪽 공백 제거
		while (pos >= 0) {
			if (cmd[pos] == ' ' || cmd[pos] == '\t' || cmd[pos] == '\r' || cmd[pos] == '\n') {
				cmd[pos] = '\0';
			}
			else {
				break;
			}
			idx--;
		}

		ComponentEngine *engine = (ComponentEngine *) Registry::getRegistry()->getManager(COMPONENT_ENGINE);
		if (engine == NULL || engine->getState() == MS_DESTROYED) {
			*this << "ComponentEngine destroyed...\r\n";
			isDone = true;
		}
		else {
			string strCmd(cmd);
			isDone = cei.doCommand(strCmd);
		}
	}
}
