/*
 * DataPortLink.cpp
 *
 *  Created on: 2008. 9. 30
 *      Author: sby
 */

#include <assert.h>
#include <DataPortLink.h>
#include <iostream>
#include <OutputDataPortInterface.h>
#include <OutputDataPort.h>
#include <RemoteDataPort.h>
#include <CTrace.h>

class DataPortLinkImpl {
protected:
	typedef std::list<DataPort *> data_port_list_type;
	typedef std::list<RemoteDataPort *> remote_data_port_list_type;
	data_port_list_type m_con_list;
	remote_data_port_list_type m_remote_list;
	DataPort *m_src;
	DataPortLink *m_caller;
public:
	DataPortLinkImpl(DataPortLink *caller) :
		m_caller(caller) {
		m_src = NULL;
	}
	virtual ~DataPortLinkImpl() {		
		clearSource();
		m_caller = NULL;
		clearDestination();
	}

	virtual ReturnType push(opros_any & data) {

		if (m_src == NULL) {
			return OPROS_BAD_INPUT_PARAMETER;
		}

		for (data_port_list_type::iterator it = m_con_list.begin(); it
				!= m_con_list.end(); ++it) {
			DataPort *peer = *it;
			peer->push(data);
		}

		if (m_remote_list.size() != 0) {
			OutputDataPortInterface *pout = SAFE_CASTING(OutputDataPortInterface *, m_src);
			std::string s_data;
			s_data.reserve(1);
			pout->marshal(data, s_data);

			for (remote_data_port_list_type::iterator it =
					m_remote_list.begin(); it != m_remote_list.end(); ++it) {
				RemoteDataPort *peer = *it;
				peer->pushData( (unsigned char *) s_data.c_str(), s_data.size());
			}
		}

		return OPROS_SUCCESS;
	}

	virtual ReturnType setSource(DataPort *src) {

		if (src == NULL) {
			clearSource();

			return OPROS_SUCCESS;
		}

		if (src->getRole() != OPROS_DATA_PORT_OUTPUT)
			return OPROS_BAD_INPUT_PARAMETER;

		if (!m_con_list.empty()) {

			DataPort *m = m_con_list.front();

			if (m->checkType(src) == false) {
				return OPROS_BAD_INPUT_PARAMETER;
			}
		}

		clearSource();
		m_src = src;

		OutputDataPortInterface *pout = SAFE_CASTING(OutputDataPortInterface *, m_src);
		pout->addConnection(m_caller);

		return OPROS_SUCCESS;
	}

	//
	// addDestination needs type check routine
	virtual ReturnType addDestination(DataPort *dest) {

		if (dest->getRole() != OPROS_DATA_PORT_INPUT)
			return OPROS_BAD_INPUT_PARAMETER;

		if (m_src != NULL) {

			if (m_src->checkType(dest) == false) {
				return OPROS_BAD_INPUT_PARAMETER;
			}

		} else if (!m_con_list.empty()) {

			DataPort *m = m_con_list.front();

			if (m->checkType(dest) == false) {
				return OPROS_BAD_INPUT_PARAMETER;
			}
		}

		m_con_list.push_back(dest);
		return OPROS_SUCCESS;
	}

	virtual ReturnType addDestination(RemoteDataPort *dest) {
		m_remote_list.push_back(dest);
		return OPROS_SUCCESS;
	}

	virtual ReturnType removeDestination(DataPort *dest) {
		m_con_list.remove(dest);
		return OPROS_SUCCESS;
	}

	virtual ReturnType removeDestination(RemoteDataPort *dest) {
		m_remote_list.remove(dest);
		return OPROS_SUCCESS;
	}

	virtual ReturnType clearDestination() {


		if (!m_con_list.empty())
		{
			for (data_port_list_type::iterator dit = 
				m_con_list.begin(); dit != m_con_list.end(); ++dit) {

				DataPort *dpeer = *dit;
				dpeer->reset();
			}
		}

		m_con_list.clear();

		if (!m_remote_list.empty())
		{
			for (remote_data_port_list_type::iterator it =
					m_remote_list.begin(); it != m_remote_list.end(); ++it) {
				RemoteDataPort *peer = *it;
				delete peer;
			}
		}
		m_remote_list.clear();
		return OPROS_SUCCESS;
	}

	virtual ReturnType clearSource() {

		if (m_src != NULL) {
			OutputDataPortInterface *pout = SAFE_CASTING(OutputDataPortInterface *, m_src);
			if (pout == NULL)
			{
				return OPROS_INTERNAL_FAULT;
			}
			pout->removeConnection(m_caller);
		}

		m_src = NULL;

		return OPROS_SUCCESS;
	}
};

DataPortLink::DataPortLink() :
	m_impl(new DataPortLinkImpl(this)) {
}

DataPortLink::~DataPortLink() {
	delete m_impl;
}

ReturnType DataPortLink::push(opros_any &data) {
	ctrace_enter();
	return m_impl->push(data);
}

ReturnType DataPortLink::setSource(DataPort *src) {
	ctrace_enter();
	return m_impl->setSource(src);
}

ReturnType DataPortLink::addDestination(DataPort *dest) {
	ctrace_enter();
	return m_impl->addDestination(dest);
}

ReturnType DataPortLink::addDestination(RemoteDataPort *dest) {
	ctrace_enter();
	return m_impl->addDestination(dest);
}

ReturnType DataPortLink::removeDestination(DataPort *dest) {
	ctrace_enter();
	return m_impl->removeDestination(dest);
}

ReturnType DataPortLink::removeDestination(RemoteDataPort *dest) {
	ctrace_enter();
	return m_impl->removeDestination(dest);
}

ReturnType DataPortLink::clearDestination() {
	ctrace_enter();
	return m_impl->clearDestination();
}

ReturnType DataPortLink::clearSource() {
	ctrace_enter();
	return m_impl->clearSource();
}

