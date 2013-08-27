/*
 * Executor.cpp
 *
 *  Created on: 2009. 3. 16
 *      Author: jangcs@etri.re.kr
 */

#include "Executor.h"
#include <system/Log.h>
#include <OPRoSTypes.h>
#include <ArgumentNumberException.h>
#include <MethodInvokeException.h>
#include <system/os/OSFunctions.h>
#ifdef USE_QUEUE_TIMER
#else
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#endif

#define NO_EXECUTOR_MESSAGE

#define INFINITE_LOOP_COUNTER 5000

#ifndef NO_BOOST
using namespace std;
using namespace boost::posix_time;
#endif

Executor::Executor(int priority, int period, int sched_policy) :
	m_executorStarter()
{
	m_priority = priority;
	m_period = period;
	m_bExecuting = false;
	m_sched_policy = sched_policy;
	m_runningState = OPROS_ES_CREATED;
	m_executorId = executorId++;
	m_runActState = false;

#ifdef USE_QUEUE_TIMER
	m_hTimerHandle =NULL;
#endif

#ifdef ANDROID
	::pthread_mutex_init(&m_rt_mutex, NULL);
	::pthread_cond_init(&m_rt_cond, NULL);
#endif
}

Executor::Executor() :
	m_executorStarter()
{
	m_priority = 0;
	m_period = 0;
	m_bExecuting = false;
	m_sched_policy = OPROS_SCHED_OTHER;
	m_runActState = false;

#ifdef USE_QUEUE_TIMER
	m_hTimerHandle =NULL;
#endif

	m_runningState = OPROS_ES_CREATED;
	m_executorId = executorId++;
#ifdef ANDROID
	::pthread_mutex_init(&m_rt_mutex, NULL);
	::pthread_cond_init(&m_rt_cond, NULL);
#endif
}

Executor::~Executor()
{
	if (m_runningState == OPROS_ES_CREATED)
	{
#ifndef NO_EXECUTOR_MESSAGE
		cout << "1. Executor terminated." << endl;
#endif

		return;
	}

	destroy();
//	stop();
//	destroyComponents();

	do {
		OS::sleepMS(1);
	}while(m_runningState != OPROS_ES_DESTROYED);



#ifndef NO_EXECUTOR_MESSAGE
	cout << "2. Executor terminated." << endl;
#endif
	#ifdef ANDROID
	::pthread_mutex_destroy(&m_rt_mutex);
	::pthread_cond_destroy(&m_rt_cond);
#endif
}


bool Executor::isEmpty()
{
	return m_compList.empty();
}

ReturnType Executor::add(ComponentAdapter *pComp)
{
	if (pComp == NULL)
		return OPROS_INPUT_NULL;
#ifdef NO_BOOST
	OSGuard lock(&m_mutexForList);
#else
	boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif

#ifndef NO_EXECUTOR_MESSAGE
	cout << "Executor::add : component name=" << pComp->getId() << endl;
#endif

	m_compList.push_back(pComp);

	//�߰��� �� ������ ���� ��Ȳ�� ����ؼ� �ڵ带 �����ؾ� ��.
	//  1. �߰��Ϸ��� ������Ʈ�� ���°� READY���� Ȯ�� (== �ʱ�ȭ������ �������� Ȯ��)
	//  2. �̹� ������ ������Ʈ�� ��ϵǾ� �ִ��� Ȯ��
	//  3. period�� 0�� ���, �ϳ��� ��ϵǵ��� �ؾ���.

	return OPROS_SUCCESS;
}

ReturnType Executor::remove(ComponentAdapter *pComp)
{
	if (pComp == NULL)
		return OPROS_INPUT_NULL;

#ifdef NO_BOOST
	OSGuard lock(&m_mutexForList);
#else
	boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif

	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);

		if (p == pComp)
		{
			m_compList.erase(itr);
			return OPROS_SUCCESS;
		}

	}

	return OPROS_BAD_INPUT_PARAMETER;
}

ReturnType Executor::remove(const string &compID)
{
#ifdef NO_BOOST
	OSGuard lock(&m_mutexForList);
#else
	boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif

	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getId() == compID)
		{
			m_compList.erase(itr);
			return OPROS_SUCCESS;
		}
	}

	return OPROS_BAD_INPUT_PARAMETER;
}

#ifdef USE_QUEUE_TIMER
VOID CALLBACK ExecutorCallbackHandler(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	long startTime,endTime;
	// parameter�� executor �ڽ��� �����͸� �޴´�.
	Executor *executor = (Executor *) lpParam;

	// QueueTimer callback�� ���� �������̸�,
	// QueueTimer�� ���� ���ο� callback ������ �������Ѿ� ��
	if(executor->m_bExecuting)
	{
		return;
	}


	executor->m_bExecuting = true;


	startTime = timeGetTime();
	executor->run();
	endTime = timeGetTime();
	executor->m_bExecuting = false;

}

#endif

ReturnType Executor::start()
{
	// ��ϵ� ������Ʈ�� ������ ���ܻ�Ȳ �߻�
	if (m_compList.empty())
		return OPROS_PRECONDITION_NOT_MET;

	// ������ CREATED, ACTIVE, INACTIVE ���¿����� start �� �� ����
	if (m_runningState == OPROS_ES_CREATED || m_runningState == OPROS_ES_ACTIVE
			|| m_runningState == OPROS_ES_INACTIVE)
	{
		// ��ϵ� ������Ʈ�� ���۽�Ŵ (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
#ifdef NO_BOOST
		OSGuard lock(&m_mutexForList);
#else
		boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif

		list<ComponentAdapter *>::iterator itr;

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			ComponentAdapter *c = (*itr);
			try
			{
				c->getComponent()->start();
			} catch (ArgumentNumberException &anEx)
				{
					c->getComponent()->stopOnError();
					log_error("ArgumentNumberException : id=" << c->getId() << "reason:"<<anEx.getMessage());
				} catch (MethodInvokeException &miEx)
				{
					//c->getComponent()->stopOnError();
					log_error ("MethodInvokeException id="	<< c->getId() << ",msg=" << miEx.getMessage());

				} catch (...)
				{
					c->getComponent()->stopOnError();
					log_error ("UnknowmException : id=" << c->getId());
				}
		}
	}

	// ����⸦ �����Ŵ
	// ������� ���� ���¿� ���� ó�� ����� �޶���.
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	//   2. ���� ������� ���°� ó�� ������� �����̸� ���� �����带 �����ϰ� ������.
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��
	//   ������ ���´� ������ �߻���

	// -->
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	if (m_runningState == OPROS_ES_ACTIVE)
		return OPROS_SUCCESS;
	//   2. ���� ������� ���°� ó�� ������� �����̸� ���� �����带 �����ϰ� ������.
	else if (m_runningState == OPROS_ES_CREATED)
	{
		// ���¸� ACTIVE ���·� �����ϰ�, �����带 �����Ѵ�.
		m_runningState = OPROS_ES_ACTIVE;
		m_executorStarter.setExecutor(this);


#ifdef USE_QUEUE_TIMER
		if (m_period == 0) // non-periodic (dedicated) mode
		{

#ifdef NO_BOOST
			ThreadManager threadMgr;
			OSThread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.threadDetach();			
#else
			ThreadManager threadMgr; // ����, ThreadManager�� �̱������� �����ؾ� ��
			boost::thread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.detach();
#endif
		}
		else // periodic mode
		{
			m_hTimerHandle = NULL;

			BOOL success = ::CreateTimerQueueTimer(
				&m_hTimerHandle,
				NULL,
				ExecutorCallbackHandler,
				this,
				0,
				m_period,
				WT_EXECUTELONGFUNCTION);
		}

#else
#ifdef NO_BOOST
			ThreadManager threadMgr;
			OSThread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.threadDetach();			
#else
			ThreadManager threadMgr; // ����, ThreadManager�� �̱������� �����ؾ� ��
			boost::thread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.detach();
#endif
#endif
		return OPROS_SUCCESS;
	}
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��
	else if (m_runningState == OPROS_ES_INACTIVE)
	{
#ifdef USE_QUEUE_TIMER
		// PLEASE CHECK AGAIN,
		// �Ʒ��� �Ϲ��� period ��常 ����Ͽ���,
		// if period == 0 then, ��� �ؾ��ϴ��� �ٽ��ѹ� üũ�ٶ�... (JANGCS)
		m_runningState = OPROS_ES_ACTIVE;
		m_hTimerHandle = NULL;

		BOOL success = ::CreateTimerQueueTimer(
			&m_hTimerHandle,
			NULL,
			ExecutorCallbackHandler,
			this,
			0,
			m_period,
			WT_EXECUTELONGFUNCTION);
#else
		// INACTIVE ���¿����� ������ �ߴ��ϰ� �ִ� �����带 �ٽ� �����Ų��.
#ifdef NO_BOOST
		OSGuard lock(&m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.broadcast();
#else
		boost::unique_lock<boost::mutex> lock(m_mutex);

		m_runningState = OPROS_ES_ACTIVE;
		m_cond.notify_all();
#endif
#endif
		return OPROS_SUCCESS;
	}
	else
		return OPROS_PRECONDITION_NOT_MET;
}

// Ư�� ������Ʈ ���� �����Ŵ.
ReturnType Executor::start(ComponentAdapter *pComp)
{
	// �ش� ������Ʈ�� �ִ����� ã��

#ifdef NO_BOOST
		OSGuard lock(&m_mutexForList);
#else
		boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif

	list<ComponentAdapter *>::iterator itr;

	// ������ CREATED, ACTIVE, INACTIVE ���¿����� start �� �� ����
	ComponentAdapter *pFoundComp = NULL;
	if (m_runningState == OPROS_ES_CREATED || m_runningState == OPROS_ES_ACTIVE
			|| m_runningState == OPROS_ES_INACTIVE)
	{
		// ������Ʈ�� ã�Ƽ� ���۽�Ŵ (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			ComponentAdapter *c = (*itr);
			if (c->getUniqueID() == pComp->getUniqueID())
			{
				pFoundComp = c;

				try
				{
					ReturnType retT = pFoundComp->getComponent()->start();
					if (retT != OPROS_SUCCESS)
					{
						return retT;
					}
				} catch (ArgumentNumberException &anEx)
				{
					pFoundComp->getComponent()->stopOnError();
					log_error("ArgumentNumberException : id=" << pFoundComp->getId() << "reason:"<<anEx.getMessage());
				} catch (MethodInvokeException &miEx)
				{
					//pFoundComp->getComponent()->stopOnError();
					log_error ("MethodInvokeException id="	<< pFoundComp->getId() << ",msg=" << miEx.getMessage());

				} catch (...)
				{
					pFoundComp->getComponent()->stopOnError();
					log_error ("UnknowmException : id=" << pFoundComp->getId());
				}

				break;
			}
		}
	}

	// �ش� ������Ʈ�� �� ã������, ������ ������
	if (pFoundComp == NULL)
		return OPROS_BAD_INPUT_PARAMETER;

	// ������� ���� ���¿� ���� ó�� ����� �޶���.
	//   1. ���� ������� ���°� ó�� ������� �����̸� �����带 ���� �����ϰ� ������Ʈ �ϳ��� ������.
	//   2. ���� ������� ���°� ACTIVE �����̸� �׳� ���� (������ �������� ����Ⱑ ���¸� ���� ó����)
	//   3. ���� ������� ���°� INACTIVE �����̸�, �ش� ������Ʈ�� ���¸�  ACTIVE ���·� �����ϰ� �����带 ������ϵ��� mutex�� �뺸��

	// -->
	//  ��� 1. ���� ������� ���°� ó�� ������� �����̸� �����带 ���� �����ϰ� ������Ʈ �ϳ��� ������.
	if (m_runningState == OPROS_ES_CREATED)
	{
		// ���¸� ACTIVE ���·� �����ϰ�, �����带 �����Ѵ�.
		m_runningState = OPROS_ES_ACTIVE;
		m_executorStarter.setExecutor(this);


#ifdef USE_QUEUE_TIMER
		if (m_period == 0) // non-periodic (dedicated) mode
		{
#ifdef NO_BOOST
			ThreadManager threadMgr;
			OSThread &executorThread = threadMgr.getThread(m_executorStarter); 
			  // threadManager���� pointer �� �����ϵ��� �ؾ���. value ���Ϲ���� cond�� critical section�� ���ִ� ������ ����Ŵ
			executorThread.threadDetach();			
#else
			ThreadManager threadMgr; // ����, ThreadManager�� �̱������� �����ؾ� ��
			boost::thread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.detach();
#endif

		}
		else // periodic mode
		{
			m_hTimerHandle = NULL;

			BOOL success = ::CreateTimerQueueTimer(
				&m_hTimerHandle,
				NULL,
				ExecutorCallbackHandler,
				this,
				0,
				m_period,
				WT_EXECUTELONGFUNCTION);
		}
#else
#ifdef NO_BOOST
			ThreadManager threadMgr;
			OSThread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.threadDetach();			
#else
			ThreadManager threadMgr; // ����, ThreadManager�� �̱������� �����ؾ� ��
			boost::thread executorThread = threadMgr.getThread(m_executorStarter);
			executorThread.detach();
#endif
#endif
		return OPROS_SUCCESS;
	}

	//  ��� 2. ���� ������� ���°� ACTIVE �����̸� �׳� ���� (������ �������� ����Ⱑ ���¸� ���� ó����)
	else if (m_runningState == OPROS_ES_ACTIVE)
		return OPROS_SUCCESS;

	//  ��� 3. ���� ������� ���°� INACTIVE �����̸�, �ش� ������Ʈ�� ���¸�  ACTIVE ���·� �����ϰ� �����带 ������ϵ��� mutex�� �뺸��
	else if (m_runningState == OPROS_ES_INACTIVE)
	{
#ifdef USE_QUEUE_TIMER
		// PLEASE CHECK AGAIN,
		// �Ʒ��� �Ϲ��� period ��常 ����Ͽ���,
		// if period == 0 then, ��� �ؾ��ϴ��� �ٽ��ѹ� üũ�ٶ�... (JANGCS)
		m_runningState = OPROS_ES_ACTIVE;
		m_hTimerHandle = NULL;

		BOOL success = ::CreateTimerQueueTimer(
			&m_hTimerHandle,
			NULL,
			ExecutorCallbackHandler,
			this,
			0,
			m_period,
			WT_EXECUTELONGFUNCTION);
#else
#ifdef NO_BOOST
		OSGuard lock(&m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.broadcast();
#else
		boost::unique_lock<boost::mutex> lock(m_mutex);

		m_runningState = OPROS_ES_ACTIVE;
		m_cond.notify_all();
#endif
#endif
		return OPROS_SUCCESS;
	}
	else
		return OPROS_PRECONDITION_NOT_MET;
}

ReturnType Executor::stop()
{
	// ACTIVE ���¿����� INACTIVE ���·� �����ؾ� �Ѵ�.
	// �ֳ��ϸ�, executeComponents()�� �����߿� destroy()���� �ҷ��� ���°� PREPARE_TO_STOP ���°� �� ��  �ִµ�,
	// �� ��쿡��, �ٽ� ���¸� INACTIVE�� ����� �ȵǱ� �����̴�.
	if (m_runningState == OPROS_ES_ACTIVE)
	{
	// ������� ���¸� INACTIVE�� �ٲٰ�,
	m_runningState = OPROS_ES_INACTIVE;

	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		p->getComponent()->stop();
	}
	}

	while(m_runActState)  // ������ inactive �ٲ�ȿ���� m_runActState�� false�� �ɶ����� ��ٸ�.
	{		
#ifdef NO_BOOST
		OS::sleepMS(1);
#else
		boost::posix_time::millisec waitAMoment(1); // ��� ��ٷȴٰ� �ٽ� ���¸� üũ�Ѵ�.
		boost::this_thread::sleep<boost::posix_time::millisec>(waitAMoment);
#endif		

	}

#ifdef USE_QUEUE_TIMER
	if( m_hTimerHandle != NULL) {
		DeleteTimerQueueTimer(NULL, m_hTimerHandle, NULL);
		m_hTimerHandle = NULL;
	}
#endif
	
	return OPROS_SUCCESS;
}

ReturnType Executor::stop(ComponentAdapter *pComp)
{
	// ACTIVE ���¿����� INACTIVE ���·� �����ؾ� �Ѵ�.
	// �ֳ��ϸ�, executeComponents()�� �����߿� destroy()���� �ҷ��� ���°� PREPARE_TO_STOP ���°� �� ��  �ִµ�,
	// �� ��쿡��, �ٽ� ���¸� INACTIVE�� ����� �ȵǱ� �����̴�.
	if (m_runningState != OPROS_ES_ACTIVE && m_period!=0)
	{
		return OPROS_PRECONDITION_NOT_MET;
	}

	// Ư�� ������Ʈ �� ������ ���� ���̹Ƿ� ����� ��ü�� ���¸� INACTIVE�� �ٲٸ� �� ��
	// m_runningState = INACTIVE;
	//boost::lock_guard<boost::mutex> lock(m_mutexForList);

	// ������Ʈ�� ã�Ƽ� ���� (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
	ComponentAdapter *pFoundComp = NULL;
	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getUniqueID() == pComp->getUniqueID())
		{
			pFoundComp = p;
			pFoundComp->getComponent()->stop();
			break;
		}
	}

	// �ش� ������Ʈ�� �� ã������, ������ ������
	if (pFoundComp == NULL)
	{
		return OPROS_BAD_INPUT_PARAMETER;
	}

	long ttx = 0;

	while(pFoundComp->getExecuteState())
	{		
#ifdef NO_BOOST
		OS::sleepMS(1);
#else
		boost::posix_time::millisec waitAMoment(1); // ��� ��ٷȴٰ� �ٽ� ���¸� üũ�Ѵ�.
		boost::this_thread::sleep<boost::posix_time::millisec>(waitAMoment);
#endif	

		if (ttx > INFINITE_LOOP_COUNTER)
		{
			cout << "Component "<< pFoundComp->getId() << " is unterminated : it may be under infinite loop "<< endl;			
		}
		else
			ttx++;
	}

	// �ϳ��� ������Ʈ�� ����(INACTIVE)���״µ�,
	// ����, ACTIVE�� ������Ʈ�� �ϳ��� ���ٸ� ������� ���µ� INACTIVE�� �ٲ۴�.
	int active_count = 0;
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE) // CHECK A CONSTANT
		{
			active_count++;
			break;
		}
	}

	// ACTIVE ������Ʈ�� �ϳ��� ���ٸ� ������� ���¸� INACTIVE�� �ٲ�
	if (active_count == 0 && m_period != 0) {
		m_runningState = OPROS_ES_INACTIVE;

		#ifdef USE_QUEUE_TIMER
			if( m_hTimerHandle != NULL) {
				DeleteTimerQueueTimer(NULL, m_hTimerHandle, NULL);
				m_hTimerHandle = NULL;
			}
		#endif
	}
	else if (m_period==0)
	{
		do {
#ifdef NO_BOOST
		OS::sleepMS(1);
#else
		boost::posix_time::millisec waitAMoment(1); // ��� ��ٷȴٰ� �ٽ� ���¸� üũ�Ѵ�.
		boost::this_thread::sleep<boost::posix_time::millisec>(waitAMoment);
#endif
		}while(m_runningState != OPROS_ES_INACTIVE);
	}

	return OPROS_SUCCESS;
}

ReturnType Executor::reset()
{
	// DESTROYED ���¸� ������ ��� ���¿��� reset() �Լ��� ȣ�� ����
	if (m_runningState == OPROS_ES_DESTROYED)
		return OPROS_PRECONDITION_NOT_MET;

	// ��ϵ� ������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �����ϵ��� �� (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
#ifdef NO_BOOST
	OSGuard lock(&m_mutexForList);
#else
	boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif

	list<ComponentAdapter *>::iterator itr;

	//    1. initialize�� �����ϰ�,
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() == OPROS_CS_DESTROYED) // CHECK A CONSTANT
			return OPROS_OUT_OF_RESOURCES;

		p->getComponent()->initialize();
	}

	//    2. start�� ��.
	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getComponent()->getStatus() != OPROS_CS_READY) // CHECK A CONSTANT
			return OPROS_INTERNAL_FAULT;

		p->getComponent()->start();
	}

	// ����⸦ �����Ŵ
	// ������� ���� ���¿� ���� ó�� ����� �޶���.
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	//   2. ���� ������� ���°� ó�� ������� �����̸� �����带 �����ϰ� ������.
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��

	// -->
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	if (m_runningState == OPROS_ES_ACTIVE)
		return OPROS_SUCCESS;
	//   2. ���� ������� ���°� ó�� ������� �����̸� �����带 �����ϰ� ������.
	else if (m_runningState == OPROS_ES_CREATED)
	{
		// ���¸� ACTIVE ���·� �����ϰ�, �����带 �����Ѵ�.
		m_runningState = OPROS_ES_ACTIVE;
		m_executorStarter.setExecutor(this);

#ifdef NO_BOOST
		ThreadManager threadMgr;
		OSThread executorThread = threadMgr.getThread(m_executorStarter);
		executorThread.threadDetach();			
#else
		ThreadManager threadMgr; // ����, ThreadManager�� �̱������� �����ؾ� ��
		boost::thread executorThread = threadMgr.getThread(m_executorStarter);
		executorThread.detach();
#endif
		return OPROS_SUCCESS;
	}
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��
	else if (m_runningState == OPROS_ES_INACTIVE)
	{
		// INACTIVE ���¿����� ������ �ߴ��ϰ� �ִ� �����带 �ٽ� �����Ų��.
#ifdef NO_BOOST
		OSGuard lock(&m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.broadcast();
#else
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.notify_all();
#endif		

		

		return OPROS_SUCCESS;
	}
	// ��Ÿ ERROR ���� ����� ó���� ���ǵǾ� ���� �����Ƿ�, �׳� ������� ���¸� ACTIVE�� �����ϵ��� ��
	else
	{
		m_runningState = OPROS_ES_ACTIVE;
		return OPROS_SUCCESS;
	}
}

ReturnType Executor::reset(ComponentAdapter *pComp)
{
	// DESTROYED ���¸� ������ ��� ���¿��� reset() �Լ��� ȣ�� ����
	if (m_runningState == OPROS_ES_DESTROYED)
		return OPROS_PRECONDITION_NOT_MET;

	// ��ϵ� ������Ʈ�� �ʱ�ȭ�ϰ� �ٽ� �����ϵ��� �� (���⼭�� ������Ʈ�� ���¸� ��ȭ�ǰ� callback �Լ��� ó���� ����)
	ComponentAdapter *pFoundComp = NULL;
	list<ComponentAdapter *>::iterator itr;

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
		ComponentAdapter *p = (*itr);
		if (p->getUniqueID() == pComp->getUniqueID())
		{
			if (p->getComponent()->getStatus() == OPROS_CS_DESTROYED) // CHECK A CONSTANT
				return OPROS_OUT_OF_RESOURCES;

			pFoundComp = p;
			pFoundComp->getComponent()->initialize();
			pFoundComp->getComponent()->start();

			break;
		}
	}

	// �ش� ������Ʈ�� �� ã������, ������ ������
	if (pFoundComp == NULL)
		return OPROS_BAD_INPUT_PARAMETER;

	// ����⸦ �����Ŵ
	// ������� ���� ���¿� ���� ó�� ����� �޶���.
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	//   2. ���� ������� ���°� ó�� ������� �����̸� �����带 �����ϰ� ������.
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��

	// -->
	//   1. ���� ������� ���°� ACTIVE �����̸� �׳� ����
	if (m_runningState == OPROS_ES_ACTIVE)
		return OPROS_SUCCESS;
	//   2. ���� ������� ���°� ó�� ������� �����̸� �����带 �����ϰ� ������.
	else if (m_runningState == OPROS_ES_CREATED)
	{
		// ���¸� ACTIVE ���·� �����ϰ�, �����带 �����Ѵ�.
		m_runningState = OPROS_ES_ACTIVE;
		m_executorStarter.setExecutor(this);

#ifdef NO_BOOST
		ThreadManager threadMgr;
		OSThread executorThread = threadMgr.getThread(m_executorStarter);
		executorThread.threadDetach();			
#else
		ThreadManager threadMgr; // ����, ThreadManager�� �̱������� �����ؾ� ��
		boost::thread executorThread = threadMgr.getThread(m_executorStarter);
		executorThread.detach();
#endif

		return OPROS_SUCCESS;
	}
	//   3. ���� ������� ���°� INACTIVE �����̸�, �����带 ������ϵ��� mutex�� �뺸��
	else if (m_runningState == OPROS_ES_INACTIVE)
	{
		// INACTIVE ���¿����� ������ �ߴ��ϰ� �ִ� �����带 �ٽ� �����Ų��.
#ifdef NO_BOOST
		OSGuard lock(&m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.broadcast();
#else
		boost::unique_lock<boost::mutex> lock(m_mutex);
		m_runningState = OPROS_ES_ACTIVE;
		m_cond.notify_all();
#endif		
		return OPROS_SUCCESS;
	}
	// ��Ÿ ERROR ���� ����� ó���� ���ǵǾ� ���� �����Ƿ�, �׳� ������� ���¸� ACTIVE�� �����ϵ��� ��
	else
	{
		m_runningState = OPROS_ES_ACTIVE;
		return OPROS_SUCCESS;
	}
}

ReturnType Executor::destroy()
{
	if (m_runningState == OPROS_ES_CREATED || 
		m_runningState == OPROS_ES_PREPARE_TO_DESTROYED ||
		m_runningState == OPROS_ES_DESTROYED)
	{
		return OPROS_SUCCESS;
	}

	stop();
	destroyComponents();

#ifdef USE_QUEUE_TIMER	
	m_runningState = OPROS_ES_DESTROYED;
#else
	m_runningState = OPROS_ES_PREPARE_TO_DESTROYED;
#endif

	return OPROS_SUCCESS;
}

ReturnType Executor::setPeriod(int period)
{
	if (period < 0)
		return OPROS_INPUT_OUT_OF_RANGE;

	m_period = period;
	return OPROS_SUCCESS;
}

int Executor::getPeriod()
{
	return m_period;
}

ReturnType Executor::setPriority(int priority)
{
	m_priority = priority;
	return OPROS_SUCCESS;
}

int Executor::getPriority()
{
	return m_priority;
}

//typedef unsigned long long ticks;
//
//static __inline__ ticks getticks(void)
//{
//     unsigned  long a, d;
//     asm("cpuid");
//     asm volatile("rdtsc" : "=a" (a), "=d" (d));
//
//     return (((ticks)a) | (((ticks)d) << 32));
//}



///////////////////////////////////////
// Executor::run()�� ȣ�����
// - Executor::start()
//		--> boost::thread ����
//			--> ExecutorStarter
//				--> Executor::run();
void Executor::run()
{

#ifdef USE_QUEUE_TIMER	
	if (m_runningState ==OPROS_ES_ACTIVE)
	{		// ��ϵ� ������Ʈ���� �����Ų��.
		m_runActState = true;

		//ptime beginTime(microsec_clock::local_time());// ���� �ð�
		executeComponents();					

		//ptime endTime(microsec_clock::local_time());	// �Ϸ� �ð�

		if (m_period == 0)	// non-periodic mode�� ��쿡��,
			m_runningState = OPROS_ES_INACTIVE;
		else
		{
//			long elapsedMicrosec =
//					elapsedTime.length().total_microseconds(); // elapsed micro sec
		}

		m_runActState = false;
	}
#else

#ifndef __arm__
	struct sched_param sp;
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	 sched_setscheduler(0, SCHED_FIFO, &sp);
#endif

#ifndef ANDROID
#ifndef __arm__
	mlockall(MCL_CURRENT | MCL_FUTURE);
#endif
#endif

#ifndef __arm__
	struct sched_param    param;
	memset(&param, (int)0, (size_t)sizeof(param));
	param.sched_priority = this->getPriority();
	if (m_sched_policy==OPROS_SCHED_FIFO)
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	else
	if (m_sched_policy==OPROS_SCHED_RR)
		pthread_setschedparam(pthread_self(), SCHED_RR, &param);
	else
		pthread_setschedparam(pthread_self(), SCHED_OTHER, &param);
#endif

 	struct timespec nextBeginTime;
     	clock_gettime(CLOCK_MONOTONIC, &nextBeginTime);

	m_runActState = false;

	while (m_runningState == OPROS_ES_ACTIVE || m_runningState
			== OPROS_ES_INACTIVE)
	{
		//std::cout << "THREAD:" << std::endl;
		//		cout << endl << "COUNT : " << ++count << endl;
		if (m_runningState == OPROS_ES_INACTIVE)
		{
#ifdef NO_BOOST
			//OSGuard lock(&m_mutex);
#else
			boost::unique_lock<boost::mutex> lock(m_mutex);
#endif
			while (m_runningState == OPROS_ES_INACTIVE)
			{
#ifdef NO_BOOST
				{
					OSGuard lock(&m_mutex);

					m_cond.wait(&m_mutex, 1000);
				}
#else
				boost::posix_time::millisec waitTime(1000); 
				m_cond.timed_wait<boost::posix_time::millisec> (lock, waitTime);
#endif

//				nextBeginTime = microsec_clock::local_time(); // next time to begin
			     	clock_gettime(CLOCK_MONOTONIC, &nextBeginTime);
			}

		}
		else // ACTIVE
		{
			m_runActState = true;

			struct timespec currentBeginTime = nextBeginTime;

			executeComponents(); 

			m_runActState = false;

			struct timespec endTime;
		     	clock_gettime(CLOCK_MONOTONIC, &endTime);


			if (m_period == 0) {
				m_runningState = OPROS_ES_INACTIVE;
			}
			else
			{
				long elapsedNanoTime = (endTime.tv_sec - currentBeginTime.tv_sec) * 1000*1000*1000  +   endTime.tv_nsec - currentBeginTime.tv_nsec;
				long sleepNanoTime = (long) (m_period * 1000000) - elapsedNanoTime; // period is Milli SEC

				if (sleepNanoTime < 0)
				{

//					cerr << "WARNING : Deadline missed !!!!!!" << endl;

				     clock_gettime(CLOCK_MONOTONIC, &nextBeginTime);
				}
				else
				{
					long overNanoSec = (nextBeginTime.tv_nsec + m_period * 1000 * 1000) % 1000000000;
					long overSec = (nextBeginTime.tv_nsec + m_period * 1000 * 1000) / 1000000000;
					nextBeginTime.tv_nsec = overNanoSec;
					nextBeginTime.tv_sec += overSec;;
#ifdef ANDROID
					pthread_cond_timedwait_monotonic_np(&m_rt_cond, &m_rt_mutex, &nextBeginTime);
#else
					clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &nextBeginTime, NULL);
#endif
				}
			}
			//			cout << "End of Sleep " << endl;
		}
	}
#ifndef ANDROID
	munlockall();
#endif


#endif



	//// ������ �ܰ��̸�, ���¸� STOPPED�� ������ ���´�.
	//// STOPPED ���°� �Ǹ� �Ҹ��ڿ��� �Ҹ��� �� �ְ� �ȴ�.  //  stop�̳� �Ҹ��ڿ��� destory������ �����Ƿ� ���ʿ���.
	//if (m_runningState == OPROS_ES_PREPARE_TO_DESTROYED)
	//{
	//	destroyComponents();
	//	m_runningState = OPROS_ES_DESTROYED;
	//}

//	cout << "<Executor::run> exit run()" << endl;
	if (m_runningState == OPROS_ES_PREPARE_TO_DESTROYED)
	{
		m_runningState = OPROS_ES_DESTROYED;
	}

	
}

void Executor::executeComponents()
{
	if (m_period == 0)
	{
#ifdef NO_BOOST
		OSGuard lock(&m_mutexForList);
#else
		boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif
		list<ComponentAdapter *>::iterator itr;

		ComponentAdapter *p;

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			p = (*itr);
			try
			{
				if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE)
				{
					CoordinationUnit *t_cu = p->getCoordinationUnit();

					if (t_cu->isEnabled()) // coordination unit enabled
					{
						if (t_cu->isTriggered()) // ��� Ʈ���Ű� ON�� ���¿� ������
						{
							p->setExecuteState(true);
							p->getComponent()->onExecute(); //
							p->setExecuteState(false);
							t_cu->setAllFollower(); // follower���� �ش� ������Ʈ�� trigger�� on ��Ŵ
							t_cu->clearAllTrigger();
						}

					}
					else {
						p->setExecuteState(true);
						p->getComponent()->onExecute();
						p->setExecuteState(false);
					}
				}

			} catch (ArgumentNumberException &anEx)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error("ArgumentNumberException : id=" << p->getId() << "reason:"<<anEx.getMessage());
			} catch (MethodInvokeException &miEx)
			{
				p->setExecuteState(false);
				//p->getComponent()->stopOnError();
				log_error ("MethodInvokeException id="	<< p->getId() << ",msg=" << miEx.getMessage());

			} catch (...)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error ("UnknowmException : id=" << p->getId());
			}

		}

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			p = (*itr);
			try
			{
				if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE)
				{
					p->setExecuteState(true);
					p->getComponent()->onUpdated();
					p->setExecuteState(false);
				}
			}catch (ArgumentNumberException &anEx)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error("ArgumentNumberException : id=" << p->getId() << "reason:"<<anEx.getMessage());
			} catch (MethodInvokeException &miEx)
			{
				p->setExecuteState(false);
				//p->getComponent()->stopOnError();
				log_error ("MethodInvokeException id="	<< p->getId() << ",msg=" << miEx.getMessage());

			} catch (...)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error ("UnknowmException : id=" << p->getId());
			}
		}

	}
	else
	{

#ifdef NO_BOOST
		OSGuard lock(&m_mutexForList);
#else
		boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif
		list<ComponentAdapter *>::iterator itr;
		ComponentAdapter *p;

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			p = (*itr);
			try
			{
				if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE)
				{
					CoordinationUnit *t_cu = p->getCoordinationUnit();

					if (t_cu->isEnabled()) // coordination unit enabled
					{
						if (t_cu->isTriggered()) // ��� Ʈ���Ű� ON�� ���¿� ������
						{
							p->setExecuteState(true);
							p->getComponent()->onExecute(); //
							p->setExecuteState(false);
							t_cu->setAllFollower(); // follower���� �ش� ������Ʈ�� trigger�� on ��Ŵ
							t_cu->clearAllTrigger();
						}

					}
					else {
						p->setExecuteState(true);
						p->getComponent()->onExecute();
						p->setExecuteState(false);
					}
				}

			} catch (ArgumentNumberException &anEx)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error("ArgumentNumberException : id=" << p->getId() << "reason:"<<anEx.getMessage());
			} catch (MethodInvokeException &miEx)
			{
				p->setExecuteState(false);
				//p->getComponent()->stopOnError();
				log_error ("MethodInvokeException id="	<< p->getId() << ",msg=" << miEx.getMessage());

			} catch (...)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error ("UnknowmException : id=" << p->getId());
			}

		}

		for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
		{
			p = (*itr);
			try
			{
				if (p->getComponent()->getStatus() == OPROS_CS_ACTIVE)
				{
					p->setExecuteState(true);
					p->getComponent()->onUpdated();
					p->setExecuteState(false);
				}
			} catch (ArgumentNumberException &anEx)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error("ArgumentNumberException : id=" << p->getId() << "reason:"<<anEx.getMessage());
			} catch (MethodInvokeException &miEx)
			{
				p->setExecuteState(false);
				//p->getComponent()->stopOnError();
				log_error ("MethodInvokeException id="	<< p->getId() << ",msg=" << miEx.getMessage());

			} catch (...)
			{
				p->setExecuteState(false);
				p->getComponent()->stopOnError();
				log_error ("UnknowmException : id=" << p->getId());
			}
		}
	}

}

ReturnType Executor::destroyComponents()
{
	if (m_runActState)
	{
		log_error ("Executor is not ready to be destroyed...");
		return OPROS_PRECONDITION_NOT_MET;
	}

#ifdef NO_BOOST
		OSGuard lock(&m_mutexForList);
#else
		boost::lock_guard<boost::mutex> lock(m_mutexForList);
#endif
	list<ComponentAdapter *>::iterator itr;

#ifndef NO_EXECUTOR_MESSAGE
	cout << "Executor size = " << m_compList.size() << endl;
#endif

	for (itr = m_compList.begin(); itr != m_compList.end(); itr++)
	{
#ifndef NO_EXECUTOR_MESSAGE
		cout << "1. Executor size = " << m_compList.size() << endl;
#endif

		ComponentAdapter *p = (*itr);
		p->getComponent()->destroy();

#ifndef NO_EXECUTOR_MESSAGE
		cout << "2. Executor size = " << m_compList.size() << endl;
#endif
	}

#ifndef NO_EXECUTOR_MESSAGE
	cout << "End of Executor size = " << m_compList.size() << endl;
#endif

	return OPROS_SUCCESS;
}
