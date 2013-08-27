/*
 * ThreadManager.h
 *
 *  Created on: 2009. 3. 9
 *      Author: jangcs@etri.re.kr
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_


#ifdef NO_BOOST
#include <system/os/OSThread.h>
#include "ExecutorStarter.h"

// 향후, ThreadManager를 싱글톤으로 수정해야 함
class ThreadManager {
public:
	OSThread &getThread(ExecutorStarter &starter);
};

#else
#include <boost/thread/thread.hpp>
#include "ExecutorStarter.h"

// 향후, ThreadManager를 싱글톤으로 수정해야 함
class ThreadManager {
public:
	boost::thread getThread(ExecutorStarter &starter);
};
#endif

#endif /* THREADMANAGER_H_ */
