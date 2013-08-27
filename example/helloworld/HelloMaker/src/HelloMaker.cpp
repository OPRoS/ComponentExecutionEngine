
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


#include "MessagePrintRequired.h"
	

#include "HelloMaker.h"

//
// constructor declaration
//
HelloMaker::HelloMaker()
		:DataIn(OPROS_LAST,1)
{
	ptrMessagePrint = NULL;
	
	portSetup();
}

//
// constructor declaration (with name)
//
HelloMaker::HelloMaker(const std::string &name):Component(name)
		,DataIn(OPROS_LAST,1)
{
	ptrMessagePrint = NULL;
	
	portSetup();
}

//
// destructor declaration
//

HelloMaker::~HelloMaker() {
}

void HelloMaker::portSetup() {

	// required service port setup


	ptrMessagePrint=new MessagePrintRequired();
	addPort("MessagePrint",ptrMessagePrint);

	// data port setup
	addPort("DataOut", &DataOut);

	addPort("DataIn", &DataIn);

	// event port setup

	addPort("EventOut", &EventOut);

	addPort("EventIn", &EventIn);
	EventIn.setOwner(this);

	// value object setup


}

// Call back Declaration
ReturnType HelloMaker::onInitialize()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onStart()
{
	// user code here
	return OPROS_SUCCESS;
}
	
ReturnType HelloMaker::onStop()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onReset()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onError()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onRecover()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onDestroy()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onEvent(Event *evt)
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onExecute()
{
	std::string msg("Hello");

	
	ptrMessagePrint->print(msg);

	
	std::string outData("hello");

	std::stringstream pData;	

	DataOut.push("Sample Msg");

	EventData<std::string> outEvent;

	outEvent.setId("messageid");
	outEvent.setContentData("hello world");

	EventOut.push(&outEvent);

	return OPROS_SUCCESS;
}
	
ReturnType HelloMaker::onUpdated()
{
	// user code here
	return OPROS_SUCCESS;
}

ReturnType HelloMaker::onPeriodChanged()
{
	// user code here
	return OPROS_SUCCESS;
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
	return new HelloMaker();
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
	return new HelloMaker();
}

void releaseComponent(Component *com)
{
	delete com;
}
#endif
#endif

