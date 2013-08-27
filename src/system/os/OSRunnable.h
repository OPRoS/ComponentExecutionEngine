/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSRunnable.h
 *
 */

#ifndef _OS_RUNNABLE_H_
#define _OS_RUNNABLE_H_

#include "OSNotify.h"

/**
	쓰레드 내에서 수행되는 클래스의 최상위 클래스.
	OSThread 에서 수행하고자 하는 객체는 OSRunnable 객체를
	상속해야 하며, 이때 쓰레드에서 OSRunnable 객체의 run() 함수를
	호출한다.
*/
class OSRunnable {
protected :
	/** 쓰레드의 종료 여부를 알리기 위한 notifier 객체. */
	OSNotify m_runNotifier;

	/** 쓰레드가 실행중인지를 나타내는 플래그. */
	bool     m_runFlag;

	/** 쓰레드가 끝날때 해당 객체를 지울지 결정하는 플래그. */
	bool m_deleteOnFinished;

public :

	OSRunnable() : m_runFlag(false), m_deleteOnFinished(false) {};
	virtual ~OSRunnable() {};

	void setOnFinished(bool f) { m_deleteOnFinished = f; };
	bool isOnFinished() { return m_deleteOnFinished; };

	/** 쓰레드의 실행이 시작되었을 때 콜백되는 함수. */
	void runStarted();

	/** 쓰레드의 실행이 종료되기 바로 직전에 콜백되는 함수. */
	void runFinished();

	/** 쓰레드가 종료될 때까지 기다리는 함수. */
	void waitUntilFinished();

	/** 
		쓰레드에서 호출되는 함수로 OSRunnable을 상속한 
		클래스에서 구현해야 함.
	*/
	virtual void run() = 0;
};

#endif 
