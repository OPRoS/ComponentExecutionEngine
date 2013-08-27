/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : OSRunnable.cpp
 *
 */

#include "OSRunnable.h"

void OSRunnable::runStarted()
{
	m_runNotifier.lock();
	m_runFlag = true;
	m_runNotifier.unlock();
}

void OSRunnable::runFinished()
{
	m_runNotifier.lock();
	m_runFlag = false;
	m_runNotifier.unlock();

	m_runNotifier.notifyAll();
}
void OSRunnable::waitUntilFinished()
{
	m_runNotifier.lock();
	while (m_runFlag) {
		m_runNotifier.wait();
	}
	m_runNotifier.unlock();

}
