/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 10. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CompositeInputEventPort.cpp
 *
 *
 */

#include <list>
#include "CompositeInputEventPort.h"
#include "InputEventPortInterface.h"
#include "system/util/BasicTypes.h"
class CompositeInputEventPortImpl
{
	typedef std::list<EventPort *> event_port_list_type;

	event_port_list_type m_port_list;

public:
	
	
	virtual ReturnType push(Event *data)
	{
		for (event_port_list_type::iterator it = m_port_list.begin(); 
			   it!= m_port_list.end(); ++it) 
		{
			EventPort *peer = *it;
			peer->push(data);
		}

		return OPROS_SUCCESS;
	}

	virtual ReturnType pushEvent(unsigned char *data, int size)
	{
		for (event_port_list_type::iterator it = m_port_list.begin(); 
				it!= m_port_list.end(); ++it) 
		{
			EventPort *peer = *it;
			peer->pushEvent(data,size);
		}

		return OPROS_SUCCESS;
	}

	virtual bool checkType(Event *data)
	{
		if (m_port_list.size()==0) return false;

		EventPort *p = m_port_list.front();

		return p->checkType(data);
	}

	virtual void setArchiveFactory(ArchiveFactory *darc)
	{
		for (event_port_list_type::iterator it = m_port_list.begin(); 
			   it!= m_port_list.end(); ++it) 
		{
			EventPort *peer = *it;

			InputEventPortInterface *d_in_port = portable_dynamic_cast(InputEventPortInterface *, peer);
			d_in_port->setArchiveFactory(darc);			
		}
	}


	virtual bool checkType(EventPort *dst)
	{
		if (m_port_list.size()==0) return false;

		EventPort *p = m_port_list.front();

		return p->checkType(dst);
	}

	virtual void addPort(EventPort *p)
	{
		m_port_list.push_back(p);
		m_port_list.unique();
	}

	virtual void removePort(EventPort *p)
	{
		m_port_list.remove(p);
	}


	virtual ~CompositeInputEventPortImpl()
	{
	}
};


CompositeInputEventPort::CompositeInputEventPort()
{
	m_provider = new CompositeInputEventPortImpl();
}

CompositeInputEventPort::CompositeInputEventPort(const std::string &name)
	:EventPort(name)
{
	m_provider = new CompositeInputEventPortImpl();
}

CompositeInputEventPort::~CompositeInputEventPort()
{
	delete m_provider;
}
EventPortRole CompositeInputEventPort::getRole()
{
	return OPROS_EVENT_PORT_INPUT;
}

bool CompositeInputEventPort::checkType(Event *data)
{
	return m_provider->checkType(data);
}

bool CompositeInputEventPort::checkType(EventPort *dst)
{
	return m_provider->checkType(dst);
}

void CompositeInputEventPort::setArchiveFactory(ArchiveFactory *darc)
{
	return m_provider->setArchiveFactory(darc);
}

ReturnType CompositeInputEventPort::push(Event *data)
{
	return m_provider->push(data);
}

ReturnType CompositeInputEventPort::pushEvent(unsigned char *data, int size)
{
	return m_provider->pushEvent(data,size);
}


void CompositeInputEventPort::addPort(EventPort *p)
{
	m_provider->addPort(p);
}

void CompositeInputEventPort::removePort(EventPort *p)
{
	m_provider->removePort(p);
}
