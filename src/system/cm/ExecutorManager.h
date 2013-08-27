/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutorManager.h
 *
 */

#ifndef _ExecutorManager_H_
#define _ExecutorManager_H_

#include "ComponentAdapter.h"

/** 
	컴포넌트가 할당된 Executor를 관리하는 객체 
*/
class ExecutorManager
{
public:
	ExecutorManager() {};
	virtual ~ExecutorManager() {};

	/**
		컴포넌트를 실행기에 추가.
		컴포넌트가 passive 인 경우는 추가하지 않음.

		@param comp 추가할 컴포넌트 객체

		@return bool 추가 성공 여부
	*/
	virtual bool addComponent(ComponentAdapter *comp) = 0;

	/** 
		실행기에서 컴포넌트를 삭제함. 

		@param comp 삭제할 컴포넌트 객체

		@return bool 삭제 성공 여부
	*/
	virtual bool removeComponent(ComponentAdapter *comp) = 0;

	/** 
		실행기에 할당된 컴포넌트를 시작시킴. 

		@param comp 시작할 컴포넌트 객체

		@return bool 성공 여부
	*/
	virtual bool startComponent(ComponentAdapter *comp) = 0;

	/** 
		실행기에 할당된 컴포넌트를 중지시킴. 

		@param comp 중지할 컴포넌트 객체

		@return bool 성공 여부
	*/
	virtual bool stopComponent(ComponentAdapter *comp) = 0;

	/** 
		실행기에 할당된 컴포넌트를 리셋시킴.

		@param comp 리셋할 컴포넌트 객체

		@return bool 성공 여부
	*/
	virtual bool resetComponent(ComponentAdapter *comp) = 0;
};

#endif
