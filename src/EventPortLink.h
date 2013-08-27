/*
 * EventPortLink.h
 *
 *  Created on: 2008. 9. 30
 *      Author: sby
 */

#ifndef EVENTPORTLINK_H_
#define EVENTPORTLINK_H_

#include <list>
#include <OPRoSTypes.h>
#include <EventPort.h>
#include <IEventPortLink.h>
#include <ReturnType.h>

class EventPortLinkImpl;
class EventPortLink: public IEventPortLink {

	EventPortLinkImpl *m_impl;

public:
	virtual ReturnType push(Event *data);
	virtual ReturnType setSource(EventPort *src);
	virtual ReturnType addDestination(EventPort *dest);
	virtual ReturnType removeDestination(EventPort *dest);
	virtual ReturnType addDestination(RemoteEventPort *dest);
	virtual ReturnType removeDestination(RemoteEventPort *dest);
	virtual ReturnType clearDestination();
	virtual ReturnType clearSource();
	EventPortLink();
	virtual ~EventPortLink();
};

#endif /* EVENTPORTLINK_H_ */
