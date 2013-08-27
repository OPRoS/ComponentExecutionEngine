/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 9. 30
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : sby.cpp
 *
 */


#include "SymbolVariable.h"


using namespace std;


long find_ch(char *p, long psize, char ch)
{
	long i;
	
	for (i=0;i < psize; i++)
	{
		if (p[i]==ch) return i;
	}

	return -1;
}

long find_ch_esc(char *p, long psize, char ch, char esc)
{
	long i;

	for (i=0;i < psize; i++)
	{
		if (p[i]==esc) 
		{
			i++;
			if (i >=psize) break;
			i++;
			if (i >=psize) break;
		}

		if (p[i]==ch) return i;
	}

	return -1;
}



long find_str(char *p, long psize, char *pattern, long ptsize)
{
	long i;
	long j;
	bool matched = false;

	for (i=0;i<psize;i++)
	{
		if (p[i] == pattern[0])
		{
			matched = true;
			for (j=0;j<ptsize;j++)
			{
				if (i+j >= psize) 
				{
					matched = false;
					break;
				}

				if (p[i+j] != pattern[j])
				{
					matched = false;
					break;
				}
			}

			if (matched)
			{
				return i;
			}
		}
	}

	return -1;

}

SymbolVariable::SymbolVariable()
{
	m_app="default";
}

SymbolVariable::~SymbolVariable()
{
}

void SymbolVariable::parseVarLast(std::string &var, std::string &comName, std::string &symName)
{
	long pos;

	const char *p = var.c_str();
	long psize = var.length();

	// end . detect

	long spos, epos;

	spos = 0;
	epos = 0;

	epos = find_ch_esc((char *)&p[spos], psize - spos,  '.', '\\');
	if (epos <0) {
		return;			
	}
	
	do {
		spos = spos+epos;
		epos = find_ch_esc((char *)&p[spos], psize - spos, '.', '\\');
		if (epos <0) break;
		epos = epos+1;
	}while(epos>=0 && epos <psize);

	comName.assign(p,spos-1);
	symName.assign(&p[spos], psize - spos);

	//std::cout<<"ComponenName is "<< comName << std::endl;
	//std::cout<<"symmbolName is "<< symName << std::endl;
}

void SymbolVariable::parseVarFirst(std::string &var, std::string &comName, std::string &symName)
{
	long pos;

	const char *p = var.c_str();
	long psize = var.length();

	// end . detect

	long spos, epos;

	spos = 0;
	epos = 0;

	epos = find_ch_esc((char *)&p[spos], psize - spos,  '.', '\\');
	if (epos <0) {
		comName.assign(p, psize);	
	}
	else
	{
		comName.assign(p, epos);
		symName.assign(p+epos+1,psize-epos-1); 
	}
	
//	std::cout<<"ComponenName is "<< comName << std::endl;
//	std::cout<<"symmbolName is "<< symName << std::endl;
}

int SymbolVariable::findNameValue(std::string &target,char *pattern, int ptsize, char *p, long psize)
{
	
	long pos;
	// find app
	long spos, epos;

	pos = find_str(p, psize, pattern, ptsize);
	if (pos >0)
	{
		if (p[pos+ptsize]=='\"')
		{
			spos = pos+ptsize+1;

			epos = find_ch_esc(&p[spos], psize - spos, '\"','\\');

			if (epos <0) return -1;
		}
		else
		{
			spos = pos+ptsize;

			epos = find_ch_esc( &p[spos], psize - spos, ';', '\\');

			if (epos < 0) return -1;

		}

		target.assign(&p[spos], epos);

	}
	else
		return -1;

	return 0;
}


void SymbolVariable::setAppName(const std::string &name)
{
	if (name.size()!=0)	m_app = name;
}

std::string &SymbolVariable::getAppName()
{
	return m_app;
}

void SymbolVariable::setVarName(const std::string &name)
{
	m_var = name;
	parseVarLast(m_var, m_componentName, m_symbolName);
}

std::string &SymbolVariable::getVarName()
{
	m_var = m_componentName;

	m_var.append(".");

	m_var.append(m_symbolName);

	return m_var;	
}


void SymbolVariable::setFormat(const std::string &fm)
{
	m_vformat = fm;
}

std::string &SymbolVariable::getFormat()
{
	return m_vformat;
}

void SymbolVariable::setComponentName(const std::string &cname)
{
	m_componentName = cname;
}

std::string &SymbolVariable::getComponentName()
{
	return m_componentName;
}


void SymbolVariable::setValue(const std::string &v)
{
	m_value = v;
}

std::string &SymbolVariable::getValue()
{
	return m_value;
}

void SymbolVariable::setSymbolName(const std::string &v)
{
	m_symbolName = v;
}

std::string &SymbolVariable::getSymbolName()
{
	return m_symbolName;
}


// {app=helloWorld;var=hello.message;valueformat=str;value=\"A{11}Chello world;\";}
int SymbolVariable::parse(char *p, long psize)
{
	char *n_app = "app=";  // 4
	char *n_var = "var=";  // 4
	char *n_vf  = "valueformat=";   // 12
	char *n_vv  = "value="; //6

	int n_app_sz = 4;
	int n_var_sz=4;
	int n_vf_sz=12;
	int n_vv_sz=6;

	findNameValue(m_app,n_app,n_app_sz,p,psize);
	findNameValue(m_var,n_var,n_var_sz,p,psize);
	findNameValue(m_vformat,n_vf,n_vf_sz,p,psize);
	findNameValue(m_value,n_vv,n_vv_sz,p,psize);

	parseVarLast(m_var, m_componentName, m_symbolName);

	return 0;
}

void SymbolFunction::setVarName(const std::string &v)
{
	m_var = v;

	std::string nn;

	parseVarLast(m_var, nn, m_symbolName);
	parseVarLast(nn, m_componentName, m_portName);	
}

std::string& SymbolFunction::getVarName()
{
	m_var = m_componentName;

	m_var.append(".");

	m_var.append(m_portName);

	m_var.append(".");

	m_var.append(m_symbolName);

	return m_var;
}

void SymbolFunction::setPortName(const std::string &name)
{
	m_portName = name;
}

std::string &SymbolFunction::getPortName()
{
	return m_portName;
}

int SymbolFunction::parse(char *p, long psize)
{
	char *n_app = "app=";  // 4
	char *n_var = "var=";  // 4
	char *n_vf  = "valueformat=";   // 12
	char *n_vv  = "value="; //6

	int n_app_sz	= 4;
	int n_var_sz	= 4;
	int n_vf_sz		= 12;
	int n_vv_sz		= 6;

	findNameValue(m_app,n_app,n_app_sz,p,psize);
	findNameValue(m_var,n_var,n_var_sz,p,psize);
	findNameValue(m_vformat,n_vf,n_vf_sz,p,psize);
	findNameValue(m_value,n_vv,n_vv_sz,p,psize);

	parseVarLast(m_var, m_componentName, m_symbolName);
	std::string nn = m_componentName;

	parseVarLast(nn, m_componentName, m_portName);

	return 0;
}

void SymbolFunction::setReturnValue(const std::string &v)
{
	m_ret = v;
}

std::string &SymbolFunction::getReturnValue()
{
	return m_ret;
}
