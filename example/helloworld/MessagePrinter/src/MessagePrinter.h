
#ifndef _MessagePrinter_COMPONENT_H
#define _MessagePrinter_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>
#include <OPRoSTypes.h>


#include <string>
		
#include "MessagePrintProvided.h"


class MessagePrinter: public Component
	,public IMessagePrint
{
protected:
// data

	InputDataPort< std::string > DataIn;
	OutputDataPort< std::string > DataOut;

//event

	InputEventPort< std::string > EventIn;
	OutputEventPort< std::string > EventOut;

// method for provider
	
// method for required
//	MessagePrintRequired *ptrMessagePrint;
//
	int m_int_value;		
	float m_float_value;		

	std::string m_message;
	int m_k;


// symbol value generation
	

public:
	MessagePrinter();
	MessagePrinter(const std::string &compId);
	virtual ~MessagePrinter();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();


	virtual void print(std::string message);
	virtual double addDouble(double a, double b);
	virtual void print_hello();

};

#endif

