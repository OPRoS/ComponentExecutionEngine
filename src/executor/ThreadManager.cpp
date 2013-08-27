/*
 * ThreadManager.cpp
 *
 *  Created on: 2009. 3. 16
 *      Author: jangcs@etri.re.kr
 */


#include "ThreadManager.h"
#include <iostream>
#ifdef NO_BOOST
#include <system/os/OSRunnable.h>
#include <list>

class ManagedExecutorThread:public OSThread
{
protected:
	ExecutorStarter &m_starter;
public:
	ManagedExecutorThread(ExecutorStarter &starter):m_starter(starter){};

	virtual void run()
	{
		m_starter();
	}
};

class ThreadManagerList
{
protected:
	typedef std::list<OSThread *> m_os_thread_list_type;
	m_os_thread_list_type m_list;

public:
	ThreadManagerList() {};

	virtual ~ThreadManagerList()
	{
		for (m_os_thread_list_type::iterator rit = m_list.begin(); rit != m_list.end(); ++rit)
		{
			OSThread *p = *rit;

			delete p;
		}		
	}

	OSThread& getThread(ExecutorStarter &starter)
	{
		OSThread *ptm = new ManagedExecutorThread(starter);

		m_list.push_back(ptm);

		ptm->threadStart();

		return (*ptm);
	}
};

ThreadManagerList m_threadListMgr;

OSThread &ThreadManager::getThread(ExecutorStarter &starter)
{
	return m_threadListMgr.getThread(starter);
}

#else

// 향후, ThreadManager를 싱글톤으로 수정해야 함
boost::thread ThreadManager::getThread(ExecutorStarter &starter)
{
	try {
		return boost::thread(starter);
	}
	catch (boost::thread_resource_error tre)
	{
		std::cerr << "Can not allocate a new thread...." << std::endl;
		exit(1);
	}
};

#endif
