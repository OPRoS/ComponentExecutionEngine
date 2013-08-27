/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 1
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentLoader.h
 *
 */

#ifndef _COMPONENT_LOADER_H_
#define _COMPONENT_LOADER_H_

#include "system/profile/ComponentProfile.h"
#include <Component.h>

/**
	 컴포넌트를 DLL 혹은 SO 파일에서 로딩하기 위한 클래스. 
 */
class ComponentLoader
{
public:
	ComponentLoader() {};
	virtual ~ComponentLoader() {};

	/**
		DLL 혹은 SO 파일을 open하는 함수.

		@param profile DLL 혹은 SO 파일 정보를 포함한 컴포넌트 프로파일

		@return DLL 혹은 SO 파일 open 성공 여부
	*/
	virtual bool open(ComponentProfile *profile) = 0;

	/**
		open 한 DLL 혹은 SO 파일을 close하는 함수.

		@return DLL 혹은 SO 파일 close 성공 여부
	*/
	virtual bool close() = 0;

	/**
		open 한 DLL 혹은 SO 파일에서 컴포넌트를 생성하는 함수.
		DLL(so)파일에서 컴포넌트를 생성하는 함수는 "getComponent()" 임.

		@return Component * 생성된 컴포넌트 객체
	*/
	virtual Component *loadComponent() = 0;

	/**
		open 한 DLL 혹은 SO 파일에서 컴포넌트를 삭제하는 함수.
		DLL(so)파일에서 컴포넌트를 삭제하는 함수는 "releaseComponent(Component *)" 임.

		@return 컴포넌트 삭제 성공여부
	*/
	virtual bool unloadComponent(Component *component) = 0;
};

#endif 
