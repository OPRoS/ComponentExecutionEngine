/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ServiceConsoleWorker.cpp
 *
 */

#include <iostream>

#include "ServiceConsoleWorker.h"
#include "ServiceConsole.h"
#include "system/console/ConsoleProcessor.h"

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

#ifdef 	MOD_PYTHON
#include "ServiceConsoleEx.h"
#endif

extern bool g_isTrace;
extern bool g_logSync;

using namespace std;

ServiceConsoleWorker::ServiceConsoleWorker(IoConnection *cnn, ServiceConsoleProvider *mp) : IoDirectWorker(cnn)
{
#ifdef 	MOD_PYTHON
	m_provider = new ServiceConsoleWorkerProvider((PyServiceConsoleProvider *)mp);
#endif
}

ServiceConsoleWorker::~ServiceConsoleWorker()
{
#ifdef 	MOD_PYTHON
	delete m_provider;
#endif
}

void ServiceConsoleWorker::processWork() throw (IOException)
{
	trace_enter();
	

	char line[512];
	char cmd[512];
	bool isDone = false;

	ConsoleProcessor cei(this, this);
	cei.printHelp();

	std::string buf;

	int mode=0;

	while (!isDone) {
		if (mode) writeString("\ropros::>");
#ifdef MOD_PYTHON
		else writeString("\rpython::>");
#endif

		readLine(line, 80);

		// 앞쪽 공백 제거
		int idx = 0;
		while (line[idx] != '\0') {
			if (line[idx] == ' ' || line[idx] == '\t' || line[idx] == '\r' || line[idx] == '\n') ;
			else break;
			idx++;
		}

		int pos = 0;
		while (line[idx] != '\0') 
		{
			if (line[idx] == '\b'){
				pos = pos - 1;
				if (pos <=0) pos = 0;
				idx = idx + 1;
			}
			else
			cmd[pos++] = line[idx++];
		}
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


		buf.assign(cmd, strlen(cmd));

		if (!buf.compare("opros::"))
		{
			mode = 1;
		}
#ifdef 	MOD_PYTHON
		else if (!buf.compare("python::"))
		{
			mode = 0;
		}
#endif
		else if (!buf.compare("quit"))
		{
			break;
		}
		else		
		{
			if (mode) // opros engine console mode
			{
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
			else  // python console mode
			{
#ifdef 	MOD_PYTHON
				string strCmd(cmd);
				m_provider->runCode(strCmd);
#endif
			}
		}
	}


}
