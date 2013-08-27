
#ifndef _HelloMaker_COMPONENT_H
#define _HelloMaker_COMPONENT_H
/*
 *  Generated sources by OPRoS Component Generator (OCG V2.0[Symbol])
 *   
 */
#include <OPRoSTypes.h>
#include <Component.h>
#include <InputDataPort.h>
#include <OutputDataPort.h>
#include <InputEventPort.h>
#include <OutputEventPort.h>
#include <Event.h>

#include <string>
	
#include "MessagePrintRequired.h"

#include <vector>
#include <list>
#include <valarray>
#include <deque>
#include <stack>
#include <set>
#include <queue>
#include <map>
#include <complex>
            


typedef struct 
{
	int intA;
	float bbb;
	char name[10];
	std::valarray<bool> m_a;
	std::vector<bool> m_b;
	std::list<bool> m_c;
	std::deque<bool> m_d;
	std::stack<bool> m_e;
	std::set<bool> m_f;
	std::multiset<bool> m_g;
	std::queue<bool> m_h;
	std::priority_queue<bool> m_i;
	std::map<bool,bool> m_j;
	std::multimap<bool,bool> m_k;
	std::complex<bool> m_l;
	std::wstring wt_name;
}MyData;



inline void save_this(opros::archive::oarchive &ar, MyData &t)
{
	ar << t.intA;
	ar << t.bbb;
	ar << t.name;
	ar << t.m_a;
	ar << t.m_b;
	ar << t.m_c;
	ar << t.m_d;
	ar << t.m_e;
	ar << t.m_f;
	ar << t.m_g;
	ar << t.m_h;
	ar << t.m_i;
	ar << t.m_j;
	ar << t.m_k;
	ar << t.m_l;
	ar << t.wt_name;
}
inline void load_this(opros::archive::iarchive  &ar, MyData &t)
{
	ar >> t.intA;
	ar >> t.bbb;
	ar >> t.name;
	ar >> t.m_a;
	ar >> t.m_b;
	ar >> t.m_c;
	ar >> t.m_d;
	ar >> t.m_e;
	ar >> t.m_f;
	ar >> t.m_g;
	ar >> t.m_h;
	ar >> t.m_i;
	ar >> t.m_j;
	ar >> t.m_k;
	ar >> t.m_l;
	ar >> t.wt_name;
}
inline std::string typeName(MyData *p)
{
	return "MyData";
}


class HelloMaker: public Component	
{
protected:
// data

	OutputDataPort< std::string > DataOut;	

	InputDataPort<  std::string > DataIn;
	

//event

	OutputEventPort< std::string > EventOut;
	InputEventPort< std::string > EventIn;

// method for provider
	

// method for required
	MessagePrintRequired *ptrMessagePrint;



// symbol value generation
	

public:
	HelloMaker();
	HelloMaker(const std::string &compId);
	virtual ~HelloMaker();
	virtual void portSetup();

protected:
	virtual ReturnType onInitialize();
	virtual ReturnType onStart();
	virtual ReturnType onStop();
	virtual ReturnType onReset();
	virtual ReturnType onError();
	virtual ReturnType onRecover();
	virtual ReturnType onDestroy();

public:
	virtual ReturnType onEvent(Event *evt);
	virtual ReturnType onExecute();
	virtual ReturnType onUpdated();
	virtual ReturnType onPeriodChanged();
};

#endif

