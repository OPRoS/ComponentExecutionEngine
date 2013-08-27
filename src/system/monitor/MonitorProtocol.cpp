/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : MonitorProtocol.cpp
 *
 */


#include "MonitorProtocol.h"

#include "system/util/Exceptions.h"
#include "system/util/Properties.h"
#include "system/io/IoManager.h"

#include "MonitorManager.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

MonitorProtocol::MonitorProtocol()
{
}

MonitorProtocol::~MonitorProtocol()
{
}

bool MonitorProtocol::open(const string &cnnHandler)
{
	IoManager *iom = portable_dynamic_cast(IoManager *, Registry::getRegistry()->getManager(IO_MGNAGER));
	if (iom != NULL) {
		setConnectionHandlerId(cnnHandler);
		return iom->addConnectionHandler(this);
	}

	return false;
}

IoDirectWorker *MonitorProtocol::createDirectWorker(IoConnection *cnn)
{
	return new MonitorWorker(this, cnn);
}

bool MonitorProtocol::processCommand(MonitorWorker *mw, const char *line, int size)
{
	string str(line);

	Properties props;
	props.toProperties(line,';');

	try {
		string target = props.getProperty("target");
		Monitor *m = m_monitorMgr->getMonitor(target);
		if (m != NULL) {

			if (!props.isExist("pkg.name") && props.isExist("app.name")) {
				props.setProperty("pkg.name", props.getProperty("app.name"));
			}
			else if (props.isExist("pkg.name") && !props.isExist("app.name")) {
				props.setProperty("app.name", props.getProperty("pkg.name"));
			}

			m->processCommand(mw, props);
		}
		else {
			mw->returnFail(props, "There is no target : target=" + target);
		}
	}
	catch (Exception &ex) {
		log_error(ex);
		mw->returnFail(props, ex.what());
	}

	return false;
}
