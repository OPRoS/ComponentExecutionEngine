/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 9. 30
 *  @Author  : sby(sby@etri.re.kr)
 *
 *  @File    : SymbolVariable.h
 *
 */

#ifndef _SYMBOL_VARIABLE_H_
#define _SYMBOL_VARIABLE_H_

#include <string>

class SymbolVariable
{
protected:
	static int findNameValue(std::string &target,char *pattern, int ptsize, char *p, long psize);

	std::string m_app;
	std::string m_var;
	std::string m_vformat;
	std::string m_value;

	std::string m_componentName;
	std::string m_symbolName;

public:
	SymbolVariable();
	virtual ~SymbolVariable();	

	virtual void setAppName(const std::string &name);
	virtual std::string &getAppName();

	virtual void setVarName(const std::string &name);
	virtual std::string &getVarName();


	virtual void setFormat(const std::string &fm);
	virtual std::string &getFormat();

	virtual void setComponentName(const std::string &cname);
	virtual std::string &getComponentName();
	
	virtual void setValue(const std::string &v);
	virtual std::string &getValue();

	virtual void setSymbolName(const std::string &v);
	virtual std::string &getSymbolName();
	
	virtual int parse(char *p, long psize);	
	static void parseVarLast(std::string &var, std::string &comName, std::string &symName);
	static void parseVarFirst(std::string &var, std::string &comName, std::string &symName);	
};

class SymbolFunction : public SymbolVariable
{
protected:
	std::string m_portName;
	std::string m_ret;

public:
	virtual void setVarName(const std::string &v);
	virtual std::string& getVarName();
	virtual void setPortName(const std::string &name);
	virtual std::string &getPortName();
	virtual int parse(char *p, long psize);
	virtual void setReturnValue(const std::string &v);
	virtual std::string &getReturnValue();

};

#endif  
