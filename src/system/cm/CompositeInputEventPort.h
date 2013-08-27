/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 10. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CompositeInputEventPort.h
 *
 *
 */

#ifndef COMPOSITE_INPUTEVENTPORT_H_
#define COMPOSITE_INPUTEVENTPORT_H_

#include <EventPort.h>
#include <ReturnType.h>
#include <Event.h>
#include <OPRoSTypes.h>
#include <Component.h>
#include <ArchiveFactory.h>
class CompositeInputEventPortImpl;

class CompositeInputEventPort : public EventPort{
	CompositeInputEventPortImpl *m_provider;
public:
	CompositeInputEventPort();
	CompositeInputEventPort(const std::string &name);

	virtual EventPortRole getRole();

	virtual bool checkType(Event *data);
	virtual bool checkType(EventPort *dst);

	virtual void setArchiveFactory(ArchiveFactory *darc);
	virtual ReturnType push(Event *data);
	virtual ReturnType pushEvent(unsigned char *data, int size);

	virtual void addPort(EventPort *p);
	virtual void removePort(EventPort *p);
	virtual ~CompositeInputEventPort();
};
#endif /* COMPOSITE_INPUTEVENTPORT_H_ */
