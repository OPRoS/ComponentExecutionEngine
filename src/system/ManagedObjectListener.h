/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 4
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ManagedObjectListener.h
 *
 */

#ifndef _MANAGED_OBJECT_LISTENER_H_
#define _MANAGED_OBJECT_LISTENER_H_

#include "ManagedObject.h"

/**
	ManagedObject 객체의 상태가 변화되었을 때
	콜백하기 위한 리스너 클래스.

*/
class ManagedObjectListener {
public:
	/**
		ManagedObject 객체의 상태가 변화되었을 때 콜백되는 함수.

		@param mo 상태가 변화된 객체
		@param state 변화된 상태

	*/
	virtual void stateChanged(ManagedObject *mo, ManagedObjectState state) = 0;
};

#endif 
