/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoWorkerManager.cpp
 *
 */

#include "IoWorkerManager.h"

#include "system/os/OSGuard.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoWorkerManager::IoWorkerManager()
{
	m_opened = false;
}

IoWorkerManager::~IoWorkerManager()
{
	close();
}

void IoWorkerManager::close()
{
	trace_enter_msg("opened=" << m_opened);

	if (m_opened) {
		m_nf.lock();
		int t_sz = m_workers.size();
		for (unsigned int i = 0; i < t_sz; i++) {	// size 계산이 가변함을 반영하지 않은 오류임.
			IoWorker *worker = m_workers.back();
			m_workers.pop_back();
			worker->close();

			// worker is deleted by the worker thread after when it is finised
			 delete worker;        // 일단 close 시점에 worker는 finished 됨.
		}

		m_opened = false;
	}
}

// --------------------------------------------
//
// Write IoRequest & Callback
//
// --------------------------------------------

void IoWorkerManager::setup(int workerN)
{
	for (int i = 0; i < workerN; i++) {
		IoWorker *worker = createWorker();
		worker->setWorkerManager(this);
		worker->threadStart();
		m_workers.push_back(worker);
	}
	m_opened = true;
}

IoWorker *IoWorkerManager::getWorker()
{
	trace_enter_msg("workers.size=" << m_workers.size());

	if (m_opened == false) {
		return false;
	}

	m_nf.lock();
	while (m_workers.size() == 0) {
		m_nf.wait();
		if (m_opened == false) {
			m_nf.unlock();
			return false;
		}
	}
	IoWorker *worker = m_workers.back();
	m_workers.pop_back();
	m_nf.unlock();

	return worker;
}

void IoWorkerManager::returnWorker(IoWorker *worker)
{
	 // 문제는 getWorker가 된 worker들이 returnWorker를 쓸때 IoWorkerManager가 제거되었다면 문제가 됨.
	m_nf.lock();
	m_workers.push_back(worker);
	m_nf.unlock();

	m_nf.notify();

}
