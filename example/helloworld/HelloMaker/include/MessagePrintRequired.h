

#ifndef _MessagePrint_REQUIRED_PORT_H
#define _MessagePrint_REQUIRED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>


#include <string>
		



/*
 * 
 * MessagePrint : public RequiredServicePort
 *
 */
class MessagePrintRequired
   :public RequiredServicePort
{
protected:

	typedef RequiredMethod<void> printFuncType;
	printFuncType *printFunc;

public:
	//
	// Constructor
	//
	MessagePrintRequired()
	{
            printFunc = NULL;
            
	   setup();
	}

	// method implementation for required method
	void print(std::string message)
	{
            opros_assert(printFunc != NULL);
	
            (*printFunc)(message);
		
	}

	

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeRequiredMethod(&MessagePrintRequired::print,"print");
        opros_assert(ptr_method != NULL);
        addMethod("print",ptr_method);
        printFunc = reinterpret_cast<printFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
