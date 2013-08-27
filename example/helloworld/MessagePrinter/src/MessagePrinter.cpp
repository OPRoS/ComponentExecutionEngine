
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0 [Symbol])
 *  
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <OPRoSTypes.h>
#include <EventData.h>
#include <ServiceUtils.h>


#include "MessagePrintProvided.h"
	

#include "MessagePrinter.h"

//
// constructor declaration
//
MessagePrinter::MessagePrinter()
		:DataIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// constructor declaration (with name)
//
MessagePrinter::MessagePrinter(const std::string &name):Component(name)
		,DataIn(OPROS_LAST,1)
{

	
	portSetup();
}

//
// destructor declaration
//

MessagePrinter::~MessagePrinter() {
}

void MessagePrinter::portSetup() {

	// provided service port setup
	
	ProvidedServicePort *pa1;

	pa1=new MessagePrintProvided(this);

	addPort("MessagePrint",pa1);

//	ptrMessagePrint = new MessagePrintRequired();
//	addPort("MessagePrint",ptrMessagePrint);

	// data port setup

	addPort("DataIn", &DataIn);

	addPort("DataOut", &DataOut);

	// event port setup

	addPort("EventIn", &EventIn);
	EventIn.setOwner(this);

	addPort("EventOut", &EventOut);


	m_int_value = 13;
	m_float_value = 22.22;

	m_message = "test variable";

	EXPORT_VARIABLE("int_value",m_int_value);
	EXPORT_VARIABLE("float_value",m_float_value);
	EXPORT_VARIABLE("message",m_message);



}

// Call back Declaration
ReturnType MessagePrinter::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType MessagePrinter::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onEvent(Event *evt)
{
	if (!evt->getId().compare("messageid"))
	{
		EventData<std::string> *pStrEvt = (EventData<std::string> *)(evt);

		std::string *p = pStrEvt->getContentData();

		std::cout<<"Event Data is "<<(*p)<<std::endl;
	}
	else
		if (!evt->getId().compare("android"))
		{
			EventData<std::string> *pStrEvt = (EventData<std::string> *)(evt);

			std::string *p = pStrEvt->getContentData();
			std::cout<<"from android Event Data is "<<(*p)<<std::endl;
		}
	// user code here
	return OPROS_SUCCESS;
}

//ReturnType MessagePrinter::onExecute()
//{
//	std::string msg;
//	int i;
//	static int k=0;
//
//	for (i=0;i<800;i++)
//		msg.append("helo helo ");

//	std::cout<< "called "<<k++<<std::endl;
	

//	ptrMessagePrint->print(msg);

ReturnType MessagePrinter::onExecute()
{
	opros_any *pData = DataIn.pop();

	if (pData != NULL)
	{
		std::string iData = DataIn.getContent(*pData);

		std::cout<<"In Data is "<<iData<<std::endl;

		delete pData;
	}
	
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType MessagePrinter::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType MessagePrinter::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
}



void MessagePrinter::print(std::string message)
{
	std::cout<<"Mes  message is "<<message<<std::endl;
}


void MessagePrinter::print_hello()
{
	std::cout<<"Print Hello"<<std::endl;
}



double MessagePrinter::addDouble(double a, double b)
{
	std::cout<<"addDouble called"<<std::endl;
	return a+b;
}


#ifndef MAKE_STATIC_COMPONENT
#ifdef WIN32

extern "C"
{
__declspec(dllexport) Component *getComponent();
__declspec(dllexport) void releaseComponent(Component *pcom);
}

Component *getComponent()
{
	return new MessagePrinter();
}

void releaseComponent(Component *com)
{
	delete com;
}

#else
extern "C"
{
	Component *getComponent();
	void releaseComponent(Component *com);
}
Component *getComponent()
{
	return new MessagePrinter();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

