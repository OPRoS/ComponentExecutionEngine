/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Service.h
 *
 */

#ifndef _SERVICE_H_
#define _SERVICE_H_

/**
	Service 객체의 최상위 클래스.
	현재 구현되어 있지 않음.
*/

namespace opros {
	namespace engine {

class Service {
	bool m_destroy;
public:
	Service(bool destroy_flag=false) : m_destroy(destroy_flag){};
	virtual ~Service(){ stop(); destroy();};
	virtual bool getDestroyFlag() { return m_destroy; }	
	virtual int init(){return 0;};
	virtual int start(){return 0;};
	virtual int stop(){return 0;};
	virtual int destroy(){return 0;};

//	template <typename RetType, typename T> RetType send_call(const std::string &serviceName, const std::string &methodName, T args);
//	template <typename T> send_msg(const std::string &serviceName, const std::string &msgId, T args);
};

	} // namespace engine

} // namespace opros

#endif
