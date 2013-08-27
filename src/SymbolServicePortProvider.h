/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 2. 18
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : SymbolServicePortProvider.h
 *
 *
 */

#ifndef _SymbolServicePortProvider_PROVIDED_PORT_H
#define _SymbolServicePortProvider_PROVIDED_PORT_H

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <ProvidedServicePort.h>
#include <RequiredServicePort.h>
#include <ProvidedMethod.h>
#include <RequiredMethod.h>
#include <SymbolService.h>
#include <ArchiveFactory.h>

class SymbolServicePort
{
 public:
	 virtual ~SymbolServicePort(){};
	 virtual std::vector<std::string> getSymbolList()=0;
	 virtual void addSymbol(const std::string name, SymbolObject *p)=0;
	 virtual std::string getSymbolValue(const std::string name)=0;
	 virtual int setSymbolValue(const std::string name, std::string encoded_value)=0;
	 virtual void setEncodingRule(SymbolEncodingRule val)=0;
	 virtual SymbolEncodingRule getEncodingRule()=0;
};

/*
 * 
 * SymbolServicePortProvider : public ProvidedServicePort
 *
 */
class SymbolServicePortProvider
	:public ProvidedServicePort, public SymbolServicePort
{
protected:
    SymbolServicePort *pcom;
	ArchiveFactory *m_fac;	
	Component *m_comp;


   typedef ProvidedMethod< std::vector<std::string> > getSymbolListFuncType;
   getSymbolListFuncType *getSymbolListFunc;

   typedef ProvidedMethod<void> addSymbolFuncType;
   addSymbolFuncType *addSymbolFunc;

   typedef ProvidedMethod<std::string> getSymbolValueFuncType;
   getSymbolValueFuncType *getSymbolValueFunc;

   typedef ProvidedMethod<int> setSymbolvalueFuncType;
   setSymbolvalueFuncType *setSymbolvalueFunc;

   typedef ProvidedMethod<void> setEncodingRuleFuncType;
   setEncodingRuleFuncType *setEncodingRuleFunc;

   typedef ProvidedMethod<SymbolEncodingRule> getEncodingRuleFuncType;
   getEncodingRuleFuncType *getEncodingRuleFunc;


public: // default method
   virtual std::vector<std::string> getSymbolList();
   virtual void addSymbol(const std::string name, SymbolObject * pobj);
   virtual std::string getSymbolValue(const std::string name);
   virtual int setSymbolValue(const std::string name,std::string encoded_value);
   virtual void setEncodingRule(SymbolEncodingRule value);
   virtual SymbolEncodingRule getEncodingRule();
   virtual int call(std::string &portName,std::string &funcName, std::string &params, std::string &retvalue);
   virtual void setArchiveFactory(ArchiveFactory *p);


public:
    //
    // Constructor
    //
    SymbolServicePortProvider(Component *comp);
    virtual ~SymbolServicePortProvider();

    // generated setup function
    virtual void setup();
};
#endif
