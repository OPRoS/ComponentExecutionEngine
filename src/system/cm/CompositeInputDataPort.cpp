/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 10. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CompositeCompositeInputDataPort.cpp
 *
 *
 */

#include "CompositeInputDataPort.h"
#include "system/util/BasicTypes.h"
class CompositeInputDataPortImpl
{
	typedef std::list<DataPort *> data_port_list_type;

	data_port_list_type m_port_list;

public:	

	virtual ReturnType push(opros_any & data)
	{
		for (data_port_list_type::iterator it = m_port_list.begin(); 
			   it!= m_port_list.end(); ++it) 
		{
			DataPort *peer = *it;
			peer->push(data);
		}

		return OPROS_SUCCESS;
	}

	virtual ReturnType pushData(unsigned char *data, int size)
	{
		for (data_port_list_type::iterator it = m_port_list.begin(); 
				it!= m_port_list.end(); ++it) 
		{
			DataPort *peer = *it;
			peer->pushData(data,size);
		}

		return OPROS_SUCCESS;
	}

	virtual void setArchiveFactory(ArchiveFactory *darc)
	{
		for (data_port_list_type::iterator it = m_port_list.begin(); 
				it!= m_port_list.end(); ++it) 
		{
			DataPort *peer = *it;

			InputDataPortInterface *d_in_port = portable_dynamic_cast(InputDataPortInterface *, peer);
			d_in_port->setArchiveFactory(darc);			
		}
	}


	virtual bool checkType(opros_any & data)
	{
		if (m_port_list.size()==0) return false;

		DataPort *p = m_port_list.front();

		return p->checkType(data);
	}

	virtual void addPort(DataPort *p)
	{
		m_port_list.push_back(p);
		m_port_list.unique();
	}

	virtual void removePort(DataPort *p)
	{
		m_port_list.remove(p);
	}


	virtual bool checkType(DataPort *dst)
	{
		if (m_port_list.size()==0) return false;

		DataPort *p = m_port_list.front();

		return p->checkType(dst);
	}

	virtual ~CompositeInputDataPortImpl()
	{
	}
};

CompositeInputDataPort::CompositeInputDataPort()
{
	m_provider = new CompositeInputDataPortImpl();
}

CompositeInputDataPort::CompositeInputDataPort(const std::string &name)
	:DataPort(name)
{
	m_provider = new CompositeInputDataPortImpl();
}

CompositeInputDataPort::~CompositeInputDataPort()
{
	delete m_provider;
}

DataPortRole CompositeInputDataPort::getRole()
{
	return OPROS_DATA_PORT_INPUT;
}

opros_any *CompositeInputDataPort::peek()
{
	return NULL;
}

opros_any *CompositeInputDataPort::pop()
{
	return NULL;
}

void CompositeInputDataPort::reset()
{
	return;
}

ReturnType CompositeInputDataPort::push(opros_any & data)
{
	return m_provider->push(data);
}

void CompositeInputDataPort::addPort(DataPort *p)
{
	m_provider->addPort(p);
}
void CompositeInputDataPort::removePort(DataPort *p)
{
	m_provider->removePort(p);
}

ReturnType CompositeInputDataPort::pushData(unsigned char *data, int size)
{
	return 	m_provider->pushData(data,size);
}


void CompositeInputDataPort::setArchiveFactory(ArchiveFactory *darc)
{
	return m_provider->setArchiveFactory(darc);
}

bool CompositeInputDataPort::checkType(opros_any & data)
{
	return m_provider->checkType(data);
}

bool CompositeInputDataPort::checkType(DataPort *dst)
{
	return m_provider->checkType(dst);
}
