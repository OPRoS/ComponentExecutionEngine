/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 9. 30
 *  @Author  : sby(sby@etri.re.kr)
 *
 *  @File    : SymbolMonitorInterface.h
 *
 */

#ifndef _SYMBOL_MONITOR_INTERFACE_H_
#define _SYMBOL_MONITOR_INTERFACE_H_

#include <string>
#include <system/monitor/SymbolVariable.h>

class SymbolMonitorInterface
{
public:
	virtual int symbolGet(std::vector<SymbolVariable> &rqlist) = 0;
	virtual int symbolSet(std::vector<SymbolVariable> &rqlist) = 0;
	virtual int symbolCall(std::vector<SymbolFunction> &rqlist) = 0;
	virtual int symbolGet(SymbolVariable &rqlist) = 0;
	virtual int symbolSet(SymbolVariable &rqlist) = 0;
	virtual int symbolCall(SymbolFunction &rqlist) = 0;
};

#endif  //_SYMBOL_MONITOR_INTERFACE_H_
