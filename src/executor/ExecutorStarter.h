/*
 * ExecutorStarter.cpp
 *
 *  Created on: 2009. 3. 9
 *      Author: jangcs@etri.re.kr
 */

#ifndef EXECUTORSTARTER_CPP_
#define EXECUTORSTARTER_CPP_

class Executor;

////////////////////////////////////////////////////////
// SchedulerStarter
class ExecutorStarter
{
public:
	Executor *m_pExecutor;

	void setExecutor(Executor *pExecutor);
	void operator() ();
};


#endif /* EXECUTORSTARTER_CPP_ */
