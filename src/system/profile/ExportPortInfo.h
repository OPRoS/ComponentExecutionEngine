/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExportPortInfo.h
 *
 */

#ifndef _EXPORT_PORT_INFO_H_
#define _EXPORT_PORT_INFO_H_

#include <string>
#include <list>
#include "Profile.h"

/** 
	컴포짓 컴포넌트가 외부에 노출하는 포트 정보를 나타내는 클래스. 
*/
class ExportPortInfo : public ProfileInfo
{
public:
	/** 컴포넌트 이름 */
	std::string  componentName;

	/** 포트 이름 */
	std::string  portName;

	/** 포트 타입 : service, data, event */
	std::string  portType;

	/** 포트 방향 : required, provided, input, output */
	std::string  portDir;

	/** 외부에 노출되는 포트 이름 */
	std::string  exportPortName;

public:
	ExportPortInfo();
	virtual ~ExportPortInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<ExportPortInfo> ExportPortInfoList;

#endif 
