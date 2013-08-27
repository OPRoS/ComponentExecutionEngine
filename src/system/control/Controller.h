/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Controller.h
 *
 */

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

/**
	원격에서 컴포넌트 실행엔진에 전달된 명령을 처리하는 클래스.	
*/ 
class Controller 
{
public:
	Controller() {};
	virtual ~Controller() {};

	virtual bool init() = 0;
};

#endif  
