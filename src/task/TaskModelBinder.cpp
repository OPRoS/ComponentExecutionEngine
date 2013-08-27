#include <modconfig/ModConfig.h>

#ifdef MOD_OPROS_TASK

#ifdef WIN32
#pragma comment(lib, "TPLexecutor.lib");
#pragma comment(lib, "TPLparser.lib");
#pragma comment(lib, "TPLutility.lib");
#ifdef _DEBUG
#pragma comment(lib, "antlr3cd.lib");
#else
#pragma comment(lib, "antlr3c.lib");
#endif
//#include <windows.h>
#endif

#include "binding/ModelBinder.h"
//#include "binding/DirectBinder.h"
// from task

#include "TaskModelBinder.h"
#include "comm/RemoteCLISvr.h"//winsock2.h window.h 충돌 회피


// from opros
#include "system/Registry.h"
#include "system/Log.h"
#include "system/monitor/Symbolmonitor.h"
#include "system/monitor/MonitorManager.h"
#include "archive/opros_archive.h"



class TaskModelBinderProvider : public ModelBinder
{
	TaskModelBinder *m_host;
	Registry *m_registry;
	SymbolMonitor *m_cm;
	OSFileSys fsys;

	RemoteCLISvr *m_rc;
	LocalCLI *m_lc;

public:
	TaskModelBinderProvider(TaskModelBinder *host);
	virtual ~TaskModelBinderProvider();

	virtual void setModelValue(LogFile* lf, std::string, RFSMvalueEval);
	virtual RFSMvalueEval getModelValue(LogFile* lf, RFSMvariable*, int);	
	virtual RFSMvalueEval callFunction(LogFile* lf, int callType, std::string, 
			int, unsigned int rtnType, std::string funcName, ValueMap*);
	void setup();
	int init();
	int start();
	int stop();
	int destroy();

};

TaskModelBinderProvider::TaskModelBinderProvider(TaskModelBinder *host):m_host(host)
{
	m_registry = NULL;
	m_cm = NULL;
	m_rc= NULL;
	m_lc=NULL;

}
void TaskModelBinderProvider::setup()
{
	if (m_cm == NULL)
	{
		m_registry = Registry::getRegistry();
		Manager *mgr = m_registry->getManager(MONITOR_MANAGER);
		assert(mgr!=NULL);
		MonitorManager *mm = portable_dynamic_cast(MonitorManager *, mgr);
		assert(mm!=NULL);
		Monitor *mp = mm->getMonitor("symbol");
		if (mp != NULL)
		{
			SymbolMonitor *smp = portable_dynamic_cast(SymbolMonitor *, mp);
			assert(smp != NULL);
			m_cm = smp;
		}
		else
		{
			log_error("symbol monitor is required");
		}
	}
}

TaskModelBinderProvider::~TaskModelBinderProvider()
{
}

RFSMvalueEval TaskModelBinderProvider::getModelValue(LogFile* lf, RFSMvariable* rvar, int line)
{
	if (m_cm==NULL) setup();

	RFSMvalueEval eval(line, rvar->getBlockName());	

	if (m_cm==NULL) 
	{
		log_error("Task Binder not configured");
		return eval;
	}

	unsigned int type = rvar->getType();
	std::string name = rvar->getName();

		if(type == TYP_BOOL){

		try {
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			bool intval;

			onstr>>intval;

			eval.setBoolValue(intval);
			
		} catch (...)
		{
			log_error("cannot find integer value"<<name);
		}
	}
	if(type == TYP_INT){

		try {
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			int intval;

			onstr>>intval;

			eval.setIntValue(intval);
			
		} catch (...)
		{
			log_error("cannot find integer value"<<name);
		}
	}
		if(type == TYP_SHORT){

		try {
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			short intval;

			onstr>>intval;

			eval.setIntValue(intval);
			
		} catch (...)
		{
			log_error("cannot find integer value"<<name);
		}
	}
	if(type == TYP_LONG){

		try {
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			long intval;

			onstr>>intval;

			eval.setIntValue(intval);
			
		} catch (...)
		{
			log_error("cannot find integer value"<<name);
		}
	}
	if(type == TYP_DOUBLE){
		try{
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			double f_val;

			onstr>>f_val;

			eval.setFloatValue(f_val);

		} catch (...)
		{
			log_error("cannot find float value"<<name);
		}

	}
	if(type == TYP_FLOAT){
		try{
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			float f_val;

			onstr>>f_val;

			eval.setFloatValue(f_val);

		} catch (...)
		{
			log_error("cannot find float value"<<name);
		}

	}
	if(type == TYP_STRING){
		try{
			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");

			m_cm->symbolGet(value);

			std::stringstream instring(value.getValue());
			opros::archive::string_iarchive onstr(&instring);

			std::string strval;

			onstr>>strval;
			eval.setStrValue(strval);
		} catch (...)
		{
			log_error("cannot find string value"<<name);
		}
	}

	return eval;

}

void TaskModelBinderProvider::setModelValue(LogFile* lf, std::string name, RFSMvalueEval eval)
{
	if (m_cm==NULL) setup();

	unsigned int type = eval.getType();

	FLOG_TRACE(lf, "(PLUG-IN:MODEL, OUT) Setting model value");

	if(type == TYP_BOOL) {
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			bool v = eval.getBoolValue();
			instr<<v;

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);

		} catch (...)
		{
			log_error("cannot find bool value"<<name);
		}
	}
	else
	if(type == TYP_INT){
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			int v = eval.getIntValue();
			instr<< v;

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);

		} catch (...)
		{
			log_error("cannot find int value"<<name);
		}
		// LOG LOG LOG
	}
	else
	if(type == TYP_SHORT){
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			short v = eval.getIntValue();
			instr<< v;

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);

		} catch (...)
		{
			log_error("cannot find short value"<<name);
		}
		// LOG LOG LOG
	}
	else
	if(type == TYP_LONG){
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			long v = eval.getIntValue();
			instr<< v;

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);

		} catch (...)
		{
			log_error("cannot find long value"<<name);
		}
		// LOG LOG LOG
	}
	else
	if(type == TYP_FLOAT){
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			float v = eval.getFloatValue();
			instr<<v;

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);
		} catch (...)
		{
			log_error("cannot find float value"<<name);
		}
		// LOG LOG
	}
	else
	if(type == TYP_DOUBLE){
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			double v = eval.getFloatValue();
			instr<<v;

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);
		} catch (...)
		{
			log_error("cannot find float value"<<name);
		}
		// LOG LOG
	}
	else
	if(type == TYP_STRING){
		try{
			std::stringstream outstring;
			opros::archive::string_oarchive instr(&outstring);
			instr<<eval.getStrValue();

			SymbolVariable value;
			value.setVarName(name);
			value.setFormat("str");
			value.setValue(outstring.str());

			m_cm->symbolSet(value);
		} catch (...)
		{
			log_error("cannot find string value"<<name);
		}
	}
}


//   AnyList params;
//	params.push_back(AnyValue(1));
//	params.push_back(AnyValue(1.2));
//	POLog(logger::debug, "[Sample] Calling PsychicPiranea.getVal");
//	int i = OPCallFunction(&val"PsychicPiranea.getVal", params);


RFSMvalueEval TaskModelBinderProvider::callFunction(LogFile* lf, int callType, 
		std::string blockname, int line, unsigned int rtnType, 
		std::string funcName, ValueMap* paramList)
{
	if (m_cm==NULL) setup();
//-pthread_mutex_lock(m_mutexp);
	RFSMvalueEval rtnEval(line, blockname);

	std::string paramValue;

//	return rtnEval;
		std::stringstream outstring;
		opros::archive::string_oarchive param(&outstring);
		
	std::string void_str("v");


	if(paramList != NULL){

		int i;
		int sz = paramList->size();
		std::map<std::string, RFSMvalueEval>::iterator it;

		for (i=0;i<sz;i++)
		{						
		for(it=paramList->begin() ; it!=paramList->end() ; it++){
			std::string pname = it->first;
			RFSMvalueEval eval = it->second;

				if (eval.getIndex()==i)
				{
			//입력 파라메터 변환
					if (eval.getType() == TYP_BOOL) {
						bool val= eval.getBoolValue();
						param << val;
					}
					else
						if(eval.getType() == TYP_INT || eval.getType()==TYP_SHORT || eval.getType() == TYP_LONG){
				int val= eval.getIntValue();
				param << val;
			}
					else
						if(eval.getType() == TYP_FLOAT || eval.getType() == TYP_DOUBLE){
						double val = eval.getFloatValue();
				param << val;
			}
					else
			if(eval.getType() == TYP_STRING){
				std::string val = eval.getStrValue();
				param << val;
			}
					break;
				}
			}
		}
//		param <<'\"';

		
	}
	else
	{
		param << void_str;

	}
		paramValue = outstring.str();


	SymbolFunction value;
	int ret;

	value.setVarName(funcName);
	value.setFormat("str");
	value.setValue(paramValue);

	ret = m_cm->symbolCall(value);

	if (ret !=0) 
	{
		RFSMvalueEval nullRet;

		return nullRet;

	}

	if (rtnType != TYP_VOID)
	{
		std::stringstream instring(value.getReturnValue());
		opros::archive::string_iarchive onstr(&instring);

		if (rtnType == TYP_INT)
		{
			int val;
			onstr >> val;

			rtnEval.setIntValue(val);
		}
		else
		if (rtnType == TYP_SHORT)
		{
			short val;
			onstr >> val;

			rtnEval.setIntValue(val);
		}
		else
		if (rtnType == TYP_LONG)
		{
			long val;
			onstr >> val;

			rtnEval.setIntValue(val);
		}

		else 
		if (rtnType == TYP_BOOL)
		{
			bool val;
			onstr >> val;

			rtnEval.setBoolValue(val);
		}
		else
		if (rtnType==TYP_DOUBLE)
		{
			double val;
			onstr >> val;

			rtnEval.setFloatValue(val);
		}
		else
		if (rtnType == TYP_FLOAT)
		{
			float val;
			onstr >> val;

			rtnEval.setFloatValue(val);
		}
		else
		if (rtnType == TYP_STRING)
		{
			std::string val;
			onstr >> val;

			rtnEval.setStrValue(val);
		}
	}

//-pthread_mutex_unlock(m_mutexp);		
	return rtnEval;
}



int TaskModelBinderProvider::init()
{
	TaskConfig cfg;
	cfg.init();
	
	if(TaskConfig::beSetted())
		;//		this->start_thr(RThread::ATTACH,"PLUGIN_BINDER");
	else
		printf("Because of configuration problems, TaskModelBinder cannot start Task Executor.\n");

	return 0;
}


int TaskModelBinderProvider::start()
{
	if (m_cm == NULL) setup();
	

	fsys.open();
	
	std::string homeDir = TaskConfig::getDeployHome();
	if (homeDir == "") homeDir = getCurPath();
	fsys.setDir(homeDir);

	
	if(TaskConfig::isTelServerOn()){
		printf("[TaskModelBinder] OPRoS Task Executor is started.\n");
		//plug-in에서는 tpl.cfg에 상관없이 telnet과 binder가 자동으로 지정된다.

		m_rc=new RemoteCLISvr(this, &fsys);
		
		m_rc->create_thr(RThread::ATTACH, "REMOTE_CLI_SVR");
		
	}
	else{
		m_lc = new LocalCLI(NULL, NULL, &fsys, this);
		m_lc->run();
	}

	return 0;
}


int TaskModelBinderProvider::stop()
{
	return 0;
}
	
int TaskModelBinderProvider::destroy()
{
	return 0;
}

TaskModelBinder::TaskModelBinder(void)
{
	mp = new TaskModelBinderProvider(this);
}

TaskModelBinder:: ~TaskModelBinder(void)
{
	if (mp != NULL) delete mp;
}

int TaskModelBinder::init()
{
	return mp->init();
}

int TaskModelBinder::start()
{
	return mp->start();
}

int TaskModelBinder::stop()
{
	return mp->stop();
}

int TaskModelBinder::destory()
{
	return mp->destroy();
}



#endif //MOD_OPROS_TASK

