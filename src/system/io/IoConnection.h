/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoConnection.h
 *
 */

#ifndef _IO_CONNECTION_H_
#define _IO_CONNECTION_H_

#include <map>
#include <string>

#include "IoEventTarget.h"

#include "system/util/BasicTypes.h"
#include "system/util/DataObjectMap.h"
#include "system/os/OSNotify.h"

class IoPacketHandler;

class IoConnection : public IoEventTarget
{
	IoPacketHandler *m_ph;
	DataObjectMap m_userObjectMap;
	OSNotify m_notify;

	// IoAcceptor or IoConnector which creates this connection
	IoEventTarget *m_parent;

public:
	IoConnection() : m_parent(NULL) { setTargetEvent(IoEventUtil::connectionEvent()); };
	virtual ~IoConnection() {  };

	virtual EventTargetType getType() { return EVT_TARGET_CONNECTION; };

	IoEventTarget *getParent() { return m_parent; };
	void setParent(IoEventTarget *parent) { m_parent = parent; };

	void setPacketHandler(IoPacketHandler *ph) { m_ph = ph; };
	IoPacketHandler *getPacketHandler() { return m_ph; };

	void setUserObject(const std::string &name, DataObject *obj) { m_userObjectMap.setData(name, obj); };
	DataObject *getUserObject(const std::string &name) { return m_userObjectMap.getData(name); };
	DataObject *removeUserObject(const std::string &name) {
		DataObject *obj = getUserObject(name);
		m_userObjectMap.removeData(name);
		return obj;
	};

	OSNotify &getNotify();

	virtual IoEvent enabledEvent() { return IO_EVT_READ | IO_EVT_WRITE | IO_EVT_CLOSE; };

	virtual bool onOpen() { return true; };
	virtual bool onClose() = 0;
	virtual int write(const byte *buf, size_t len) = 0;
	virtual int read(byte *buf, size_t len) = 0;
};

typedef std::map<std::string, IoConnection *> IoConnectionMap;

#endif  //_IO_CONNECTION_H_
