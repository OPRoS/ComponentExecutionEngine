/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoWorkerManager.h
 *
 */

#ifndef IO_WORKER_MANAGER_H
#define IO_WORKER_MANAGER_H

#include <vector>

#include "IoWorker.h"
#include "system/io/IoConnection.h"

#include "system/os/OSNotify.h"

class IoWorkerManager
{
	typedef std::vector<IoWorker *> IoWorkerList;

protected :

	OSNotify m_nf;
	IoWorkerList m_workers;
	bool m_opened;

public :

	IoWorkerManager();
	virtual ~IoWorkerManager();

	//---------------------------------------------------------------
	// create a worker
	//  : This method should be implemented to the sub concrete class
	virtual IoWorker *createWorker() = 0;

	//-------------------------------------------
	// setup method : create "workerN" workers
	virtual void setup(int workerN);

	//-------------------------------
	// close the worker manager
	// close all workers
	virtual void close();

	//-----------------------------------------
	// get a worker for processing a request
	virtual IoWorker *getWorker();

	//---------------------------------------------
	// return a worker after finishing the work
	virtual void returnWorker(IoWorker *worker);
};

#endif


