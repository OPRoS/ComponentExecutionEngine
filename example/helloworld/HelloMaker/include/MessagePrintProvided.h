

#ifndef _MessagePrint_PROVIDED_PORT_H
#define _MessagePrint_PROVIDED_PORT_H

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
		

#ifndef _IMessagePrint_CLASS_DEF
#define _IMessagePrint_CLASS_DEF
/*
 * IMessagePrint
 *
 *  The comonent inherits this class and implements methods. 
 */
class IMessagePrint
{
 public:
    virtual void print(std::string message)=0;
 };
#endif

/*
 * 
 * MessagePrint : public ProvidedServicePort
 *
 */
class MessagePrintProvided
	:public ProvidedServicePort, public IMessagePrint
{
protected:
    IMessagePrint *pcom;


   typedef ProvidedMethod<void> printFuncType;
   printFuncType *printFunc;


public: // default method
   virtual void print(std::string message)
   {
		opros_assert(printFunc != NULL);
		(*printFunc)(message);
   }


public:
    //
    // Constructor
    //
    MessagePrintProvided(IMessagePrint *fn)
    {
        pcom = fn;

        printFunc = NULL;
        

        setup();
    }

    // generated setup function
    virtual void setup()
    {
        Method *ptr_method;
    
        ptr_method = makeProvidedMethod(&IMessagePrint::print,pcom,"print");

        opros_assert(ptr_method != NULL);
        addMethod("print",ptr_method);
        printFunc = reinterpret_cast<printFuncType *>(ptr_method);
        ptr_method = NULL;

    
    }
};
#endif
