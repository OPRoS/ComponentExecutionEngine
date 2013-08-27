/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 30
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PortConnectionInfo.h
 *
 */

#ifndef _PORT_CONNECTION_INFO_H_
#define _PORT_CONNECTION_INFO_H_

#include <string>
#include <list>

#include "Profile.h"

/** 
	두 컴포넌트 간의 포트 연결 정보를 나타내는 클래스.
*/
class PortConnectionInfo : public ProfileInfo
{
public:
	/** 포트 타입 : service, data, event */
	std::string  portType;
	/** 
		두 컴포넌트가 서로 원격 노드에 있을 경우,
		포트를 연결할 프로토콜 타입.
		ex) tcp, upnp, ...
	*/
	std::string  ioType;

	/** 포트 연결에서 소스(Required Service Port, Output Data/Event Port) 컴포넌트 이름 */
	std::string  sourceComponentName;

	/** 포트 연결에서 소스 컴포넌트의 포트이름 */
	std::string  sourcePortName;

	/** 포트 연결에서 타겟(Provided Service Port, Input Data/Event Port) 컴포넌트 이름 */
	std::string  targetComponentName;

	/** 포트 연결에서 타겟 컴포넌트의 포트이름 */
	std::string  targetPortName;

public:
	PortConnectionInfo();
	virtual ~PortConnectionInfo();

	virtual void parse(Config *cfg) throw (IOException);
	virtual void print(std::ostream &os, int level = 0);
};

typedef std::list<PortConnectionInfo> PortConnectionInfoList;

#endif 
