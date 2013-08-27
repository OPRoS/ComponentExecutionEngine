/*
 * ExecutorStarter.cpp
 *
 *  Created on: 2009. 3. 16
 *      Author: jangcs@etri.re.kr
 */

#include "Executor.h"
#include "ExecutorStarter.h"


void ExecutorStarter::setExecutor(Executor *pExecutor)
{
	m_pExecutor = pExecutor;
}


void ExecutorStarter::operator() ()
{
	m_pExecutor->run();
}
