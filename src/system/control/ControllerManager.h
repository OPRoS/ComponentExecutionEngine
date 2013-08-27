/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ControllerManager.h
 *
 */

#ifndef _CONTROLLER_MANAGER_H_
#define _CONTROLLER_MANAGER_H_

#include <string>

#include "system/Manager.h"

#include "Controller.h"


/**
	원격에서 컴포넌트 실행엔진에 전달된 명령을 처리하는 클래스.	
*/ 
class ControllerManager : public Manager
{
	typedef std::map<std::string, Controller *> ControllerMap;

	/** Controller 객체를 저장하기 위한 맵 */
	ControllerMap m_controllerMap;

public:
	ControllerManager();
	virtual ~ControllerManager();

	/** 
		ControllerManager 초기화 함수.

		@return 성공여부
	*/
	virtual bool onInit();

	/** 
		ControllerManager 실행 시작 함수.

		@return 성공여부
	*/
	virtual bool onStart();

	/** 
		ControllerManager 실행 중지 함수.

		@return 성공여부
	*/
	virtual bool onStop();

	/** 
		ControllerManager 삭제 함수.

		@return 성공여부
	*/
	virtual bool onDestroy();

	/** 
		Controller 객체를 검색하는 함수. 

		@param name	검색하고자 하는 Controller 객체 이름

		@return Controller*	검색한 Controller 객체. 
							존재하지 않을 경우 NULL 반환.
	*/
	Controller *getController(const std::string &name);

	/** 
		Controller 객체를 추가하는 함수. 

		@param name	추가하고자 하는 Controller 객체 이름
		@param Controller*	추가하고자 하는 Controller 객체. 
	*/
	void setController(const std::string &name, Controller *monitor);

	/** 
		Controller 객체를 삭제하는 함수.
		메모리에서도 삭제함.

		@param name	삭제하고자 하는 Controller 객체 이름
	*/
	void removeController(const std::string &name);

	/** 
		모든 Controller 객체를 삭제하는 함수. 
	*/
	void clearControllers();

};

#endif  
