/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 26
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventTarget.h
 *
 */

#ifndef _IO_EVENT_TARGET_H_
#define _IO_EVENT_TARGET_H_

#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "IoEventUtil.h"
#include "IoEventTargetListener.h"
#include "IoEventHandler.h"

#include "system/config/Configurable.h"
#include "system/util/Properties.h"
#include "system/os/OSSocket.h"
#include "system/os/OSGuard.h"
#include "system/os/OSMutex.h"

#define INVALID_EVENT_HANDLE NULL

class IoEventProcessor;
class IoConnectionHandler;

typedef enum { 	EVT_TARGET_ACCEPTOR,
				EVT_TARGET_CONNECTOR,
				EVT_TARGET_CONNECTION } EventTargetType;

// IoEventTarget
class IoEventTarget
{
	IoEventProcessor *m_processor;
	IoConnectionHandler *m_cnnHandler;

	IoEventHandler *m_ioh;
	IoEvent m_targetEvent;

	std::string m_id;

	bool m_isOpen;

	IoEventTargetListenerList m_listeners;

	Properties m_props;	

protected:
	virtual bool onOpen() = 0;
	virtual bool onClose() = 0;

public:
	IoEventTarget();
	virtual ~IoEventTarget();

	friend std::ostream & operator<<(std::ostream & os, IoEventTarget &et) {
		os << "IoEventTarget.id=" << et.getId();
		return os;
	};

	Properties *getProperties()
	{		
		return &m_props; 
	};

	void setProperties(Properties &props)
	{	
		m_props = props; 
	};

	void setEventProcessor(IoEventProcessor *p) { m_processor = p; };
	IoEventProcessor *getEventProcessor() { return m_processor; };

	void setConnectionHandler(IoConnectionHandler *h) { m_cnnHandler = h; };
	IoConnectionHandler *getConnectionHandler() { return m_cnnHandler; };

	void setId(const std::string &id) { m_id = id; };
	std::string getId() { return m_id; };

	virtual EventTargetType getType() = 0;

	bool isAcceptor() { return getType() == EVT_TARGET_ACCEPTOR; };
	bool isConnector() { return getType() == EVT_TARGET_CONNECTOR; };
	bool isConnection() { return getType() == EVT_TARGET_CONNECTION; };

	virtual bool open();
	virtual bool close();

	void setOpen(bool isOpen) { m_isOpen = isOpen; };
	bool isOpen() { return m_isOpen; };

	void addListener(IoEventTargetListener *l);
	void removeListener(IoEventTargetListener *l);
	IoEventTargetListenerList getListenerList();
	void fireOpened();
	void fireClosed();

	virtual OSSocketHandle getSocketHandle() = 0;

	void setEventHandler(IoEventHandler *h) { m_ioh = h; };
	IoEventHandler *getEventHandler() { return m_ioh; };

	void setTargetEvent(IoEvent target) { m_targetEvent = target; };
	IoEvent getTargetEvent() { return maskEvent(m_targetEvent); };

	void addEvent(IoEvent evt) { IoEventUtil::setEvent(m_targetEvent, evt); };
	void clearEvent(IoEvent evt) { IoEventUtil::clearEvent(m_targetEvent, evt); };

	virtual IoEvent enabledEvent() { return IoEventUtil::allEvent(); };
	virtual IoEvent maskEvent(IoEvent evt) { return IoEventUtil::maskEvent(enabledEvent(), evt); };
};

typedef std::vector<IoEventTarget *> IoEventTargetList;
typedef std::map<std::string, IoEventTarget *> IoEventTargetMap;

#endif // _IO_EVENT_TARGET_H_
