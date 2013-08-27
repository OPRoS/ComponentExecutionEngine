/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentRequestWorker.cpp
 *
 */

#include "ComponentRequestWorker.h"

#include "ComponentManager.h"
#include "ComponentRequester.h"

#include "CompositeInputDataPort.h"
#include "CompositeInputEventPort.h"
#include "system/Registry.h"
#include "system/util/BasicTypes.h"
#include "system/io/protocol/rpc/IoResponse.h"
#include "MethodInvokeException.h"

#include "system/Log.h"
#include "system/Trace.h"
#include "system/SystemTypes.h"

#include "Component.h"
#include "../../ArchiveManager.h"

using namespace std;

ComponentRequestWorker::ComponentRequestWorker()
{
	m_requestor = NULL;
}

ComponentRequestWorker::ComponentRequestWorker(ComponentRequester *r)
{
	m_requestor = r;
}

ComponentRequestWorker::~ComponentRequestWorker()
{
}

void ComponentRequestWorker::processWork()
{
	trace_enter();

	if (m_requestor == NULL) {
		log_error("ComponentRequester is NULL");
		return;
	}


	ArchiveManager *armgr = portable_dynamic_cast(ArchiveManager *, Registry::getRegistry()->getManager(ARCHIVE_MANAGER));
	if (armgr == NULL) {
		log_error("ArchiveManager is NULL");
		return;
	}

	m_ios.resetBuffer(m_buf, m_size);
	std::string compName;
	std::string portName;
	int		portType;

	m_ios >> compName;
	m_ios >> portName;
	m_ios >> portType;

	if (portType == CA_PORT_SERVICE)
	{
		std::string methodName;
		int paramSize;
		m_ios >> methodName;
		m_ios >> paramSize;

		byte *param = NULL;

		if (paramSize < 0)
		{
			log_error("Illegal Request Protocol error");
			std::string msg("Internal Error");

			m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
			return;
		}

		if (paramSize != 0)
		{
			param= new byte[paramSize];
			if (param==NULL)
			{
				log_error("CA_PORT_SERVICE memory allocation error");
				std::string msg("Internal Error");

				m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
				return;
			}
			

			m_ios.getBytes(param, paramSize);
		}

		ComponentManager *cmgr = portable_dynamic_cast( ComponentManager *,(Registry::getRegistry()->getManager(COMPONENT_MANAGER)));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			std::string msg("ComponentManager is NULL");
			if (param != NULL) delete param;

			m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
			
			return;
		}

		cmgr->lock();
		ComponentAdapterPtr ca = cmgr->getComponentAdapter(compName);
		if (ca.isNULL()) {
			log_error("Component does not exist : compName=" << compName);
			std::string msg("Component does not exist");
			if (param != NULL) delete param;
			m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
			
			cmgr->unlock();
			return;
		}
		else if (ca->getComponentAdapterStatus()==CA_CONNECTED)
		{
			std::string msg("Component is not activated");
			if (param != NULL) delete param;
			m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());			
			cmgr->unlock();
			return;
		}
		ca->IncUseCount();

			cmgr->unlock();

		if (ca->getType()==CA_ATOMIC)
		{
			Component *comp = ca->getComponent();

			if (comp == NULL) {
				log_error("Component does not exist : compName=" << compName);
				std::string msg("Component does not exist");
				if (param != NULL) delete param;
				m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
				ca->decUseCount();
				
				return;
			}

			Port *tport = comp->getPort(portName);
			if (tport == NULL)
			{
				log_error("Port does not exist : compName=" << compName << " portName =" << portName);
				std::string msg("Port does not exist");
				if (param != NULL) delete param;
				m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
				ca->decUseCount();
				
				return;
			}

			if (tport->getType() != OPROS_SERVICE)
			{
				log_error("Port type does not match : compName=" << compName << " portName =" << portName);
				std::string msg("Port type does not match");
				if (param != NULL) delete param;
				m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *) msg.c_str(), msg.length());
				ca->decUseCount();
				
				return;
			}

			ServicePortInterface * ts_port = portable_dynamic_cast(ServicePortInterface *, tport);

			std::string s_param((const char *)param, paramSize);
			std::string r_data;

			r_data.reserve(1);
			int ret;

			ts_port->setArchiveFactory(armgr->getDefaultArchiveFactory());

			try {
			ret = ts_port->callService(methodName, s_param, r_data);
			}catch (MethodInvokeException x)
			{
				ret = OPROS_INTERNAL_FAULT;
			}


			ca->decUseCount();
			//int l= r_data.length();
			if (ret !=OPROS_SUCCESS)
			{
				log_error("Method Calling Error : compName=" << compName << " portName =" << portName << " methodName =" << methodName << " retValue="<<ret);
				std::string msg("Method Calling Error");
				if (param != NULL) delete param;
				m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
				
				return;
			}
			else
			{
				if (r_data.length() ==0)
				{
					m_requestor->methodResponse(getCnnId(), m_reqId, true, compName, portName, methodName, (byte *)"V",1);
				}
				else
				m_requestor->methodResponse(getCnnId(), m_reqId, true, compName, portName, methodName, (byte *)r_data.c_str(), r_data.length() );
			}

			if (param != NULL) delete param;
		}
		else
		if (ca->getType()==CA_COMPOSITE)
		{
			//cmgr->unlock();
				//OSGuard g(ca->getLock());
			

				ProvidedServicePort *ts_port = ca->getProvidedServicePort(portName);

				std::string s_param((const char *)param, paramSize);
				std::string r_data;
				int ret;

				r_data.reserve(1);

				ts_port->setArchiveFactory(armgr->getDefaultArchiveFactory());
			try {
				ret = ts_port->callService(methodName, s_param, r_data);
			}catch (MethodInvokeException x)
			{
				ret = OPROS_INTERNAL_FAULT;
			}
				ca->decUseCount();
				//int l= r_data.length();
				if (ret !=OPROS_SUCCESS)
				{
					log_error("Method Calling Error : compName=" << compName << " portName =" << portName << " methodName =" << methodName << " retValue="<<ret);
					std::string msg("Method Calling Error");
					if (param != NULL) delete param;
					m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
					
					return;
				}
				else
				{
					if (param != NULL) delete param;

					if (r_data.length() ==0)
					{
						m_requestor->methodResponse(getCnnId(), m_reqId, true, compName, portName, methodName, (byte *)"V",1);
					}
					else
					m_requestor->methodResponse(getCnnId(), m_reqId, true, compName, portName, methodName, (byte *)r_data.c_str(), r_data.length() );
				}


		}else
		{
			ca->decUseCount();
			//cmgr->unlock();
					log_error("Component does not exist : compName=" << compName);
					std::string msg("Component does not exist");
					if (param != NULL) delete param;
					m_requestor->methodResponse(getCnnId(), m_reqId, false, compName, portName, methodName, (byte *)msg.c_str(), msg.length());
					

					return;
		}
	}
	else
	if (portType == CA_PORT_DATA)
	{
		int dataSize;

		m_ios >> dataSize;

		byte *databuf = 0;

		if (dataSize==0)
			return;

		databuf=new byte[dataSize];
		if (databuf == NULL)
		{
			log_error("CA_PORT_DATA memory allocation error");
			return;
		}


		m_ios.getBytes(databuf, dataSize);


		ComponentManager *cmgr = portable_dynamic_cast( ComponentManager *,(Registry::getRegistry()->getManager(COMPONENT_MANAGER)));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			if (databuf != NULL) delete databuf;
			return;
		}

		cmgr->lock();
		ComponentAdapterPtr ca = cmgr->getComponentAdapter(compName);
		if (ca.isNULL()) {
			log_error("Component does not exist : compName=" << compName);
			if (databuf != NULL) delete databuf;
			cmgr->unlock();
			return;
		}
		else if (ca->getComponentAdapterStatus()==CA_CONNECTED)
		{
			log_error("Component is not activated : compName=" << compName);
			if (databuf != NULL) delete databuf;
			cmgr->unlock();
			return;
		}
		ca->IncUseCount();

		cmgr->unlock();

		if (ca->getType()==CA_ATOMIC)
		{

			Component *comp = ca->getComponent();

			if (comp == NULL) {
				log_error("Component does not exist : compName=" << compName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			Port *tport = comp->getPort(portName);
			if (tport == NULL)
			{
				log_error("Port does not exist : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			if (tport->getType() != OPROS_DATA)
			{
				log_error("Port type does not match : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			DataPort *d_port = portable_dynamic_cast(DataPort *, tport);
			if (d_port == NULL)
			{
				log_error("Port casting error (to DataPort): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			if (d_port->getRole()!= OPROS_DATA_PORT_INPUT)
			{
				log_error("Only input data port can receive data from outside : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			InputDataPortInterface *d_in_port = portable_dynamic_cast(InputDataPortInterface *, d_port);
			if (d_in_port == NULL)
			{
				log_error("DataPort casting error (to InputDataPortInterface): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			d_in_port->setArchiveFactory(armgr->getDefaultArchiveFactory());
			d_in_port->pushData(databuf, dataSize);
			ca->decUseCount();

			if (databuf != NULL) delete databuf;
		}
		else
		if (ca->getType()==CA_COMPOSITE)
		{
			//OSGuard g(ca->getLock());
			//cmgr->unlock();
			//OSGuard g(ca->getLock());

			DataPort *d_port = ca->getInputDataPort(portName);
			if (d_port == NULL)
			{
				log_error("Port casting error (to DataPort): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			if (d_port->getRole()!= OPROS_DATA_PORT_INPUT)
			{
				log_error("Only input data port can receive data from outside : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			CompositeInputDataPort *d_in_port = portable_dynamic_cast(CompositeInputDataPort *, d_port);
			if (d_port == NULL)
			{
				log_error("DataPort casting error (to InputDataPortInterface): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			d_in_port->setArchiveFactory(armgr->getDefaultArchiveFactory());
			d_in_port->pushData(databuf, dataSize);
			ca->decUseCount();


			if (databuf != NULL) delete databuf;
		}
		else
		{
			ca->decUseCount();
			//cmgr->unlock();
				log_error("Component does not exist : compName=" << compName);
				if (databuf != NULL) delete databuf;
				return;
		}
	}
	else
	if (portType == CA_PORT_EVENT)
	{
		int dataSize;

		m_ios >> dataSize;
		byte *databuf = 0;

		if (dataSize==0)
			return;

		databuf=new byte[dataSize];
		if (databuf == NULL)
		{
			log_error("CA_PORT_EVENT memory allocation error");
			return;
		}

		m_ios.getBytes(databuf, dataSize);


		ComponentManager *cmgr = portable_dynamic_cast( ComponentManager *,(Registry::getRegistry()->getManager(COMPONENT_MANAGER)));
		if (cmgr == NULL) {
			log_error("ComponentManager is NULL");
			if (databuf != NULL) delete databuf;
			return;
		}

		cmgr->lock();
		ComponentAdapterPtr ca = cmgr->getComponentAdapter(compName);
		if (ca.isNULL()) {
			log_error("Component does not exist : compName=" << compName);
			if (databuf != NULL) delete databuf;
			cmgr->unlock();
			return;
		}
		else if (ca->getComponentAdapterStatus()==CA_CONNECTED)
		{
			log_error("Component is not activated : compName=" << compName);
			if (databuf != NULL) delete databuf;
			cmgr->unlock();
			return;
		}
		ca->IncUseCount();
		cmgr->unlock();

		if (ca->getType()==CA_ATOMIC)
		{

		//	OSGuard g(ca->getLock());
		//	cmgr->unlock();

			Component *comp = ca->getComponent();

			if (comp == NULL) {
				log_error("Component does not exist : compName=" << compName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			Port *tport = comp->getPort(portName);
			if (tport == NULL)
			{
				log_error("Port does not exist : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			if (tport->getType() != OPROS_EVENT)
			{
				log_error("Port type does not match : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			EventPort *d_port = portable_dynamic_cast(EventPort *, tport);
			if (d_port == NULL)
			{
				log_error("Port casting error (to DataPort): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				return;
			}

			if (d_port->getRole()!= OPROS_EVENT_PORT_INPUT)
			{
				log_error("Only input event port can receive data from outside : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			InputEventPortInterface *d_in_port = portable_dynamic_cast(InputEventPortInterface *, d_port);
			if (d_port == NULL)
			{
				log_error("EventPort casting error (to InputEventPortInterface): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			d_in_port->setArchiveFactory(armgr->getDefaultArchiveFactory());
			d_in_port->pushEvent(databuf, dataSize);
			ca->decUseCount();
			if (databuf != NULL) delete databuf;
		}else
		if (ca->getType()==CA_COMPOSITE)
		{
		//	OSGuard g(ca->getLock());
		//	cmgr->unlock();

			EventPort *d_port = ca->getInputEventPort(portName);
			if (d_port == NULL)
			{
				log_error("Port casting error (to DataPort): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			if (d_port->getRole()!= OPROS_EVENT_PORT_INPUT)
			{
				log_error("Only input event port can receive data from outside : compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			CompositeInputEventPort *d_in_port = portable_dynamic_cast(CompositeInputEventPort *, d_port);
			if (d_port == NULL)
			{
				log_error("DataPort casting error (to InputEventPortInterface): compName=" << compName << " portName =" << portName);
				if (databuf != NULL) delete databuf;
				ca->decUseCount();
				return;
			}

			d_in_port->setArchiveFactory(armgr->getDefaultArchiveFactory());
			d_in_port->pushEvent(databuf, dataSize);
			ca->decUseCount();
			if (databuf != NULL) delete databuf;
		}
		else
		{
			ca->decUseCount();
		//	cmgr->unlock();
				log_error("Component does not exist : compName=" << compName);
				if (databuf != NULL) delete databuf;
				return;
		}
	}
 }
