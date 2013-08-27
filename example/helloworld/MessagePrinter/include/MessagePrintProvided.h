

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
	virtual void print_hello()=0;
	virtual double addDouble(double a, double b)=0;
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

   printFuncType *print_helloFunc;



   typedef ProvidedMethod<double> addDoubleFuncType;
   addDoubleFuncType *addDoubleFunc;


public: // default method
   virtual void print(std::string message)
   {
		opros_assert(printFunc != NULL);
		(*printFunc)(message);
   }

   virtual void print_hello()
   {
	   	opros_assert(printFunc != NULL);
		(*print_helloFunc)();
   }

   virtual double addDouble(double a, double b)
   {
		opros_assert(addDoubleFunc != NULL);
		return (*addDoubleFunc)(a,b);
   }

public:
    //
    // Constructor
    //
    MessagePrintProvided(IMessagePrint *fn)
    {
        pcom = fn;

        printFunc = NULL;
		addDoubleFunc = NULL;
		print_helloFunc = NULL;
        

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


        ptr_method = makeProvidedMethod(&IMessagePrint::print_hello,pcom,"print_hello");

        opros_assert(ptr_method != NULL);
        addMethod("print_hello",ptr_method);
        print_helloFunc = reinterpret_cast<printFuncType *>(ptr_method);
        ptr_method = NULL;


    
		ptr_method = makeProvidedMethod(&IMessagePrint::addDouble,pcom,"addDouble");

        opros_assert(ptr_method != NULL);
        addMethod("addDouble",ptr_method);
        addDoubleFunc = reinterpret_cast<addDoubleFuncType *>(ptr_method);
        ptr_method = NULL;
    }
};
#endif
