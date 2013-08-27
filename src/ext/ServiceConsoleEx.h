/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 10. 26
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ServiceConsoleEx.h
 *
 */

#ifndef SERVICE_CONSOLE_EX_HEADER
#define SERVICE_CONSOLE_EX_HEADER

using namespace std;

#ifdef 	MOD_PYTHON

#include <Python.h>
#include <system/monitor/SymbolMonitor.h>
#include <system/monitor/MonitorManager.h>
#include <archive/opros_string_oarchive.h>


extern SymbolMonitor *m_monitor;

static PyObject* opros_func_call(PyObject *self, PyObject *args)
{
	int tuple_size = PyTuple_Size(args);

	
	PyObject *t = PyTuple_GetItem(args, 0);

	if (!PyString_Check(t))
	{
		std::cout<<"the first argument of \"opros.call\" should be a string."<<std::endl;

		Py_INCREF(Py_None);
		return Py_None;
	}


	// str means component.port.funcName

	if (m_monitor==NULL)
	{
		Registry *m_registry = Registry::getRegistry();
		if (m_registry == NULL)
		{
			std::cout<<"the component engine does not work."<<std::endl;

			Py_INCREF(Py_None);
			return Py_None;
		}

		Manager *mgr = m_registry->getManager(MONITOR_MANAGER);
		if (mgr == NULL)
		{
			std::cout<<"the symbol manager was not found[1]."<<std::endl;

			Py_INCREF(Py_None);
			return Py_None;
		}

		MonitorManager *mm = portable_dynamic_cast(MonitorManager *, mgr);
		if (mm == NULL)
		{
			std::cout<<"the symbol manager was not found[2]."<<std::endl;

			Py_INCREF(Py_None);
			return Py_None;
		}

		Monitor *mp = mm->getMonitor("symbol");
		if (mp == NULL)
		{
			std::cout<<"cannot find symbol processor[1]."<<std::endl;

			Py_INCREF(Py_None);
			return Py_None;
		}
	

		m_monitor = portable_dynamic_cast(SymbolMonitor *, mp);
		if (m_monitor == NULL)
		{
			std::cout<<"cannot find symbol processor[2]."<<std::endl;

			Py_INCREF(Py_None);
			return Py_None;
		}
	} // if (m_monitor==NULL)



	SymbolFunction var;

	
	char *str = PyString_AS_STRING(t);

	std::string funName(str);

	// parameter serialization

	var.setVarName(funName);
	var.setFormat("str");

	if (tuple_size > 1)
	{
		int i;
		std::stringstream mout;
		opros::archive::string_oarchive param(&mout);;


		for (i=1;i<tuple_size;i++)
		{
			PyObject *temp = PyTuple_GetItem(args, i); 

			if (PyString_Check(temp))
			{
				char *str = PyString_AS_STRING(temp);

				std::cout<<" arg:"<<i<<" is "<<str<<std::endl;

				std::string nstr(str);

				param << nstr;
			}
			else
			if (PyInt_Check(temp))
			{
				long v = PyInt_AS_LONG(temp);

				std::cout<<" value is(long) :"<<i<<" is "<<v<<std::endl;

				param << v;

			}
			else
			if (PyFloat_Check(temp))
			{
				double v = PyFloat_AS_DOUBLE(temp);

				std::cout<<" value is(double) :" << i<< " is " << v << std::endl;

				param << v;
			}
		}

		var.setValue(mout.str());

	}	

	m_monitor->symbolCall(var);

	
	Py_INCREF(Py_None);
	return Py_None;

}



static PyMethodDef OPRoSCallMethods[] = {
	{"call", opros_func_call, METH_VARARGS,"opros function call service."},
	{NULL,NULL,0,NULL}
};





class PyServiceConsoleProvider : public ServiceConsoleProvider
{
	PyThreadState *mainThreadState;
public:
	PyThreadState *getMainThreadState()
	{
		return mainThreadState;
	}

	~PyServiceConsoleProvider()
	{
		PyEval_AcquireLock();
		PyThreadState_Swap(mainThreadState);
		PyEval_ReleaseLock();
	}

	PyServiceConsoleProvider()
	{

		// save a pointer to the main PyThreadState object
		mainThreadState = PyThreadState_Get();
		// release the lock
		PyEval_ReleaseLock();
	}
};


class ServiceConsoleWorkerProvider
{
	PyInterpreterState *mainInterpreterState;
	PyThreadState *myThreadState;
public:
	ServiceConsoleWorkerProvider(PyServiceConsoleProvider *mp)
	{
		PyEval_AcquireLock();
		mainInterpreterState=mp->getMainThreadState()->interp;
		myThreadState = //PyThreadState_New(mainInterpreterState);
		myThreadState = Py_NewInterpreter();

		
		
 		Py_InitModule("opros", OPRoSCallMethods);




		PyEval_ReleaseLock();
	}

	virtual	void runCode(std::string &code)
	{
		PyEval_AcquireLock();
		PyThreadState_Swap(myThreadState);
		PyRun_SimpleString(code.c_str());
		PyThreadState_Swap(NULL);
		PyEval_ReleaseLock();
	}

	virtual ~ServiceConsoleWorkerProvider()
	{
		// grab the lock
		PyEval_AcquireLock();
		// swap my thread state out of the interpreter
		PyThreadState_Swap(NULL);
		// clear out any cruft from thread state object
		PyThreadState_Clear(myThreadState);
		// delete my thread state object
		//PyThreadState_Delete(myThreadState);
		Py_EndInterpreter(myThreadState);
		// release the lock
		PyEval_ReleaseLock();
	}
};

#endif //  	MOD_PYTHON

#endif // SERVICE_CONSOLE_EX_HEADER