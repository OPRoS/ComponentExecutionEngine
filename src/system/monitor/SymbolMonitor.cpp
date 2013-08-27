/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 9. 30
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : SymbolMonitor.cpp
 *
 */


#include "SymbolMonitor.h"
#include "SymbolVariable.h"


#include "system/Registry.h"
#include "system/control/ControllerManager.h"

#include "system/Log.h"
#include "system/Trace.h"

#include "system/cm/ComponentAdapterComposite.h"
#include "SymbolServicePortProvider.h"

#include "StringArchiveFactory.h"
#include "BinaryArchiveFactory.h"

using namespace std;

long find_ch_esc(char *p, long psize, char ch, char esc);

int parseSymbolGetRequest(char *p, long psize, std::vector<SymbolVariable> &out)
{
	long spos, epos;  // block start, block end
	long bpos; // block start position

	bpos = 0;
	spos = 0;
	epos = 0;

	do
	{		
		spos = find_ch_esc( &p[bpos], psize - bpos, '{', '\\');
		if (spos <0) break;
		epos = find_ch_esc( &p[bpos+spos], psize - bpos - spos, '}','\\');
		if (epos <0) break;

		if (epos >=0)
		{
			SymbolVariable s;

			s.parse(&p[bpos+spos], epos+1);

			out.push_back(s);

			bpos = bpos + spos + epos + 1;
			if (bpos >= psize) break;
		}
	}while(1);

	return out.size();
}

int parseSymbolSetRequest(char *p, long psize, std::vector<SymbolVariable> &out)
{
	return parseSymbolGetRequest(p, psize, out);
}

int parseSymbolCallRequest(char *p, long psize, std::vector<SymbolFunction> &out)
{
	long spos, epos;  // block start, block end
	long bpos; // block start position

	bpos = 0;
	spos = 0;
	epos = 0;

	do
	{		
		spos = find_ch_esc( &p[bpos], psize - bpos, '{', '\\');
		if (spos <0) break;
		epos = find_ch_esc( &p[bpos+spos], psize - bpos - spos, '}','\\');
		if (epos <0) break;

		if (epos >=0)
		{
			SymbolFunction s;

			s.parse(&p[bpos+spos], epos+1);

			out.push_back(s);

			bpos = bpos + spos + epos + 1;
			if (bpos >= psize) break;
		}
	}while(1);

	return out.size();
}

int makeCallReturnPayload(std::string &out, std::vector<SymbolFunction> &in)
{
	typedef std::vector<SymbolFunction> ptype;

	ptype::iterator it = in.begin();

	for(;it != in.end(); ++it)
	{
		out.append("{");
		SymbolFunction  p = *it;

		//if (p.getAppName().size()!=0)
		//{
		//	out.append("app=");
		//	out.append(p.getAppName());
		//	out.append(";");
		//}

		if (p.getVarName().size()!=0)
		{
			out.append("var=");
			out.append(p.getVarName());
			out.append(";");			
		}

		if (p.getFormat().size()!=0)
		{
			out.append("valueformat=");
			out.append(p.getFormat());
			out.append(";");			
		}

		if (p.getReturnValue().size()!=0)
		{
			out.append("value=\"");
			out.append(p.getReturnValue());
			out.append("\";");			
		}
		out.append("}");

	}

	return 0;
}

int makePayload(std::string &out, std::vector<SymbolVariable> &in)
{
	typedef std::vector<SymbolVariable> ptype;

	ptype::iterator it = in.begin();

	for(;it != in.end(); ++it)
	{
		out.append("{");
		SymbolVariable  p = *it;

		/*if (p.getAppName().size()!=0)
		{
			out.append("app=");
			out.append(p.getAppName());
			out.append(";");
		}*/

		if (p.getVarName().size()!=0)
		{
			out.append("var=");
			out.append(p.getVarName());
			out.append(";");			
		}

		if (p.getFormat().size()!=0)
		{
			out.append("valueformat=");
			out.append(p.getFormat());
			out.append(";");			
		}

		if (p.getValue().size()!=0)
		{
			out.append("value=\"");
			out.append(p.getValue());
			out.append("\";");			
		}

		out.append("}");

	}

	return 0;
}



SymbolMonitor::SymbolMonitor()
{
	trace_enter();

	m_registry = Registry::getRegistry();
	Manager *mgr = m_registry->getManager(COMPONENT_MANAGER);
	m_cm = portable_dynamic_cast(ComponentManager *, mgr);		
}

SymbolMonitor::~SymbolMonitor()
{
}

bool SymbolMonitor::doCommand(MonitorWorker *mw, Properties &props)
{
	try {
		string cmd = props.getProperty("cmd");
		if (cmd == "get") {
			symbolGet(mw, props);
		}
		else if (cmd == "set") {
			symbolSet(mw, props);
		}
		else if (cmd == "call") {
			symbolCall(mw, props);
		}
		
		else {
			mw->returnFail("symbol", props.getProperty("cmd", "unknown"),"Invalid command");
		}
	}
	catch (Exception &ex) {
		log_error(ex);
		const char *res = ex.what();
		string errs;

		if (res == NULL) errs = "Unknown Error";
		else errs = res;
		mw->returnFail("symbol", props.getProperty("cmd", "unknown"),errs);		
	}

	return true;
}


ComponentAdapterPtr SymbolMonitor::findComponent(std::string &comname)
{
	std::string appname;
	std::string cmp_name;
	std::string fname;
	std::string sname;

	SymbolVariable::parseVarFirst(comname,appname, cmp_name);// fname will be app name
	if (cmp_name.size()==0)
	{
		fname = appname;
	}
	else{

		SymbolVariable::parseVarFirst(cmp_name,fname, sname);// fname will be app name
	}


	if (fname.size() != 0)
	{
		ComponentAdapterPtr pca = m_cm->getComponentAdapter(fname);
		if (pca.isNULL()) 
		{
				log_error("cannot find target component "<<fname<<".[1]");
				return NULL;		
		}

		if (pca->getType()== CA_COMPOSITE)
		{
			if (sname.size()==0)
			{
				log_error("target component is composite component. cannot retrieve symbol value");
				return NULL;
			}

			
			ComponentAdapterComposite *cac = portable_dynamic_cast(ComponentAdapterComposite *, pca.get());
			opros_assert(cac!=NULL);
			
			std::string fname_one, sname_one, or_sname;
			or_sname = sname;

			do {
				fname_one.clear();
				sname_one.clear();

				SymbolVariable::parseVarFirst(or_sname, fname_one, sname_one);

				ComponentAdapterPtr ccac = cac->getChildComponent(fname_one);
				if (ccac.isNULL()) 
				{
					log_error("cannot find target component "<<fname_one<<".[2]");
					return NULL;		
				}

				if (ccac->getType()==CA_COMPOSITE)
				{
					if (sname_one.size()==0)
					{
						log_error("target component "<<fname_one<<" is composite component. cannot retrieve symbol value");
						return NULL;
					}

					cac = portable_dynamic_cast(ComponentAdapterComposite *, ccac.get());
					or_sname = sname_one;
				}
				else
				if (ccac->getType()== CA_ATOMIC)
				{
					if (sname_one.size()!=0)
					{
						log_error("component has to be a composite component.[1]");
						return NULL;
					}

					return ccac;
				}
			} while(1);
		}
		else
		if (pca->getType()==CA_ATOMIC)
		{
			if (sname.size()!=0)
			{
				log_error("component has to be a composite component.[2]");
				return NULL;
			}

			return pca;
		}
	}

	return NULL;
}


int SymbolMonitor::symbolGet(std::vector<SymbolVariable> &rqlist)
{
	if (rqlist.size() >0)
	{
		// symbol get is requested
		// find symbol value.. end response making

		typedef std::vector<SymbolVariable> rqlisttype;

		rqlisttype::iterator it = rqlist.begin();

		long sc_spos = 0;
		long sc_epos = 0;
		int tret;

		for(;it != rqlist.end(); ++it)
		{
			SymbolVariable &s = *it;

			tret = symbolGet(s);

			if (tret) return tret;		
		}
	}

	return 0;
}


int SymbolMonitor::symbolGet(SymbolVariable &s)
{
	ComponentAdapterPtr pcm = findComponent(s.getComponentName());
	if (pcm.isNULL())
	{				
		return -1;;
	}
	else
	{
		SymbolServicePortProvider *p = (SymbolServicePortProvider *)pcm->getProvidedServicePort("opros.symbol");
		if (p==NULL)
		{
			return -2;
		}

		s.setValue(p->getSymbolValue(s.getSymbolName()));
		s.setFormat(std::string("str"));
	}

	return 0;
}




int SymbolMonitor::symbolGet(MonitorWorker *mw, Properties &props)
{
	trace_enter();

	int paySize = props.getInt("payloadSize");

	char *buf = new char[paySize];
	if (buf == NULL) 
	{
		trace("symbol get error: cannot allocate payload buffer");
		mw->returnFail("symbol","get","protocol error");
		return -1;
	}


	if (mw->read((byte *)buf, paySize) != paySize) 
	{
		delete buf;
		trace("symbol get error: cannot read payload");
		mw->returnFail("symbol","get","protocol error");
		return -2;
	}


	std::vector<SymbolVariable> rqlist;
	parseSymbolGetRequest(buf,paySize,rqlist); // parse ok
	delete buf;

	if (rqlist.size() >0)
	{
		// symbol get is requested
		// find symbol value.. end response making

		typedef std::vector<SymbolVariable> rqlisttype;

		rqlisttype::iterator it = rqlist.begin();

		long sc_spos = 0;
		long sc_epos = 0;

		for(;it != rqlist.end(); ++it)
		{
			SymbolVariable &s = *it;


			ComponentAdapterPtr pcm = findComponent(s.getComponentName());
			if (pcm.isNULL())
			{
				mw->returnFail("symbol","get","cannot find component");
				return -3;
			}
			else
			{
				SymbolServicePortProvider *p = (SymbolServicePortProvider *)pcm->getProvidedServicePort("opros.symbol");
				if (p==NULL)
				{
					mw->returnFail("symbol","get","cannot find symbol port");
					return -4;
				}

				s.setValue(p->getSymbolValue(s.getSymbolName()));
				if (s.getValue().size()==0)
				{
					mw->returnFail("symbol","get","cannot find symbol or symbol is empty");
					return -5;
				}

				s.setFormat(std::string("str"));
			}
		}
	}
	else {
		mw->returnFail("symbol","get","cannot find symbol or symbol is empty or protocol error");
		return -6;
	}


	std::string returnPayload;


	makePayload(returnPayload,rqlist);

	////////////////////////////////////////////////////////

	string ret="0";
	
	mw->returnOK(props, ret, returnPayload);

	return 0;
}


int SymbolMonitor::symbolSet(std::vector<SymbolVariable> &rqlist)
{
	if (rqlist.size() >0)
	{
		// symbol get is requested
		// find symbol value.. end response making

		typedef std::vector<SymbolVariable> rqlisttype;

		rqlisttype::iterator it = rqlist.begin();

		long sc_spos = 0;
		long sc_epos = 0;
		int tret;

		for(;it != rqlist.end(); ++it)
		{
			SymbolVariable &s = *it;

			tret = symbolSet(s);

			if (tret) return tret;
		}
	}

	return 0;
}




int SymbolMonitor::symbolSet(SymbolVariable &rq)
{
	
	ComponentAdapterPtr pcm = findComponent(rq.getComponentName());
	if (pcm.isNULL())
	{				
		return -1;
	}
	else
	{
		SymbolServicePortProvider *p = (SymbolServicePortProvider *)pcm->getProvidedServicePort("opros.symbol");
		int tret;

		if (p==NULL)
		{					
			return -2;
		}

		if (!rq.getFormat().compare("str"))
		{
			p->setEncodingRule(OPROS_SYMBOL_ENCODING_STR);
			tret = p->setSymbolValue(rq.getSymbolName(),rq.getValue());
			if (tret <0)
			{						
				return -3;
			}
		}
		else if (!rq.getFormat().compare("bin"))
		{
			p->setEncodingRule(OPROS_SYMBOL_ENCODING_BIN);					
			tret = p->setSymbolValue(rq.getSymbolName(),rq.getValue());
			if (tret <0)
			{						
				return -4;
			}
		}				
	}

	return 0;
}



int SymbolMonitor::symbolSet(MonitorWorker *mw, Properties &props)
{
	trace_enter();

	int paySize = props.getInt("payloadSize");

	char *buf = new char[paySize];
	if (buf == NULL) 
	{
		trace("symbol set error: cannot allocate payload buffer");
		mw->returnFail("symbol","set","protocol error");
		return -1;
	}


	if (mw->read((byte *)buf, paySize) != paySize) 
	{
		delete buf;
		trace("symbol set error: cannot read payload");
		mw->returnFail("symbol","set","protocol error");
		return -2;
	}


	std::vector<SymbolVariable> rqlist;
	parseSymbolSetRequest(buf,paySize,rqlist); // parse ok
	delete buf;

	int tret = 0;
	if (rqlist.size() >0)
	{
		// symbol get is requested
		// find symbol value.. end response making

		typedef std::vector<SymbolVariable> rqlisttype;

		rqlisttype::iterator it = rqlist.begin();

		long sc_spos = 0;
		long sc_epos = 0;

		for(;it != rqlist.end(); ++it)
		{
			SymbolVariable &s = *it;


			ComponentAdapterPtr pcm = findComponent(s.getComponentName());
			if (pcm.isNULL())
			{
				mw->returnFail("symbol","set","cannot find component");
				return -3;
			}
			else
			{
				SymbolServicePortProvider *p = (SymbolServicePortProvider *)pcm->getProvidedServicePort("opros.symbol");
				if (p==NULL)
				{
					mw->returnFail("symbol","set","cannot find symbol port");
					return -4;
				}

				if (!s.getFormat().compare("str"))
				{
					p->setEncodingRule(OPROS_SYMBOL_ENCODING_STR);
					try {
						tret = p->setSymbolValue(s.getSymbolName(), s.getValue());
						if (tret <0)
						{
							mw->returnFail("symbol","set","cannot find symbol");
							return -5;
						}
					}
					catch (opros::archive::archive_type_exception x)
					{
						mw->returnFail("symbol","set","type mismatch");
						return -6;
					}
					catch (opros::archive::archive_format_exception fx)
					{
						mw->returnFail("symbol","set","type mismatch");
						return -7;
					}
				}
				else if (!s.getFormat().compare("bin"))
				{
					p->setEncodingRule(OPROS_SYMBOL_ENCODING_BIN);					
					try {
						tret = p->setSymbolValue(s.getSymbolName(), s.getValue());
						if (tret <0)
						{
							mw->returnFail("symbol","set","cannot find symbol");
							return -8;
						}
					}
					catch (opros::archive::archive_type_exception x)
					{
						mw->returnFail("symbol","set","type mismatch");
						return -9;
					}
					catch (opros::archive::archive_format_exception fx)
					{
						mw->returnFail("symbol","set","type mismatch");
						return -10;
					}
				}				
			}
		}
	}


	////////////////////////////////////////////////////////

	string ret="0";
	
	mw->returnOK(props, ret);


	return 0;
}
/*
class TStringArchiveFactory : public TArchiveFactory
{
public:
	virtual opros::archive::oarchive *getOutputArchive(std::ostream *str)
	{
		return new opros::archive::string_oarchive(str);

	}

	virtual opros::archive::iarchive *getInputArchive(std::istream *str)
	{
		return new opros::archive::string_iarchive(str);
	}


	virtual void release(opros::archive::oarchive *arc)
	{
		delete arc;
	}

	virtual void release(opros::archive::iarchive *arc)
	{
		delete arc;
	}

	virtual ~TStringArchiveFactory(){};
};


class TBinaryArchiveFactory : public TArchiveFactory
{
public:
	virtual opros::archive::oarchive *getOutputArchive(std::ostream *str)
	{
		return new opros::archive::string_oarchive(str);

	}

	virtual opros::archive::iarchive *getInputArchive(std::istream *str)
	{
		return new opros::archive::string_iarchive(str);
	}


	virtual void release(opros::archive::oarchive *arc)
	{
		delete arc;
	}

	virtual void release(opros::archive::iarchive *arc)
	{
		delete arc;
	}

	virtual ~TBinaryArchiveFactory(){};
};

*/

int SymbolMonitor::symbolCall(MonitorWorker *mw, Properties &props)
{
	trace_enter();

	int paySize = props.getInt("payloadSize");

	char *buf = new char[paySize];
	if (buf == NULL) 
	{
		trace("symbol call error: cannot allocate payload buffer");
		mw->returnFail("symbol","call","protocol error");
		return -1;
	}


	if (mw->read((byte *)buf, paySize) != paySize) 
	{
		delete buf;
		trace("symbol call error: cannot read payload");
		mw->returnFail("symbol","call","protocol error");
		return -2;
	}


	std::vector<SymbolFunction> rqlist;
	parseSymbolCallRequest(buf,paySize,rqlist); // parse ok
	delete buf;

	int tret = 0;
	if (rqlist.size() >0)
	{
		// symbol get is requested
		// find symbol value.. end response making

		typedef std::vector<SymbolFunction> rqlisttype;

		rqlisttype::iterator it = rqlist.begin();

		long sc_spos = 0;
		long sc_epos = 0;

		std::string returnValue;

		for(;it != rqlist.end(); ++it)
		{
			SymbolFunction &s = *it;


			ComponentAdapterPtr pcm = findComponent(s.getComponentName());
			if (pcm.isNULL())
			{
				mw->returnFail("symbol","call","cannot find component");
				return -3;
			}
			else
			{
				ProvidedServicePort *p = pcm->getProvidedServicePort(s.getPortName());
				if (p==NULL)
				{
					mw->returnFail("symbol","call",std::string("cannot find service port ").append(s.getPortName()));
					return -4;
				}

				Method *d_m = p->getMethod(s.getSymbolName());

				if (d_m==NULL)
				{
					mw->returnFail("symbol","call",std::string("cannot find method ").append(s.getVarName()));
					return -5;
				}

				if (!s.getFormat().compare("str"))
				{
					std::stringstream inv(s.getValue());
					std::stringstream outv;
					
					if (s.getValue().size()==0)
					{
						mw->returnFail("symbol","call","invalid paramter");
						return -6;
					}
					
					opros::archive::string_iarchive d_in_arc(&inv);
					opros::archive::string_oarchive d_out_arc(&outv);

					ReturnType cc_ret;
					try {
					cc_ret = d_m->call(d_in_arc, d_out_arc);	
					} 
					catch (opros::archive::archive_type_exception etx)
					{
						mw->returnFail("symbol","call","invalid paramter");
						return -7;
					}
					catch (opros::archive::archive_format_exception efx)
					{
						mw->returnFail("symbol","call","invalid paramter");
						return -8;
					}

					if (cc_ret != OPROS_SUCCESS)
					{
						mw->returnFail("symbol","call","method call error");
						return -9;
					}

					s.setReturnValue( outv.str());
				}
				else if (!s.getFormat().compare("bin"))
				{
					std::stringstream inv(s.getValue());
					std::stringstream outv;
					
					
					if (s.getValue().size()==0)
					{
						mw->returnFail("symbol","call","invalid paramter");
						return -10;
					}
				
					opros::archive::binary_iarchive d_in_arc(&inv);
					opros::archive::binary_oarchive d_out_arc(&outv);

					ReturnType cc_ret;

					try {
					cc_ret = d_m->call(d_in_arc, d_out_arc);	
					}
					catch (opros::archive::archive_type_exception etx)
					{
						mw->returnFail("symbol","call","invalid paramter");
						return -11;
					}
					catch (opros::archive::archive_format_exception efx)
					{
						mw->returnFail("symbol","call","invalid paramter");
						return -12;
					}

					if (cc_ret != OPROS_SUCCESS)
					{
						mw->returnFail("symbol","call","method call error");
						return -13;
					}

					s.setReturnValue( outv.str());
				}				
			}
		}
	}


	////////////////////////////////////////////////////////

	string ret="0";

	std::string returnPayload;


	makeCallReturnPayload(returnPayload,rqlist);		

	
	mw->returnOK(props, ret, returnPayload);
	
	return 0;
	
}



int SymbolMonitor::symbolCall(std::vector<SymbolFunction> &rqlist)
{
	int tret = 0;
	if (rqlist.size() >0)
	{
		// symbol get is requested
		// find symbol value.. end response making

		typedef std::vector<SymbolFunction> rqlisttype;

		rqlisttype::iterator it = rqlist.begin();

		long sc_spos = 0;
		long sc_epos = 0;

		std::string returnValue;
		int tret;

		for(;it != rqlist.end(); ++it)
		{
			SymbolFunction &s = *it;

			tret =symbolCall(s);
			if (tret) return tret;		
		}
	}


	return 0;
}



int SymbolMonitor::symbolCall(SymbolFunction &rq)
{
	ComponentAdapterPtr pcm = findComponent(rq.getComponentName());
	if (pcm.isNULL())
	{				
		return -1;
	}
	else
	{
		ProvidedServicePort *p = pcm->getProvidedServicePort(rq.getPortName());
		if (p==NULL)
		{
			return -2;
		}

		Method *d_m = p->getMethod(rq.getSymbolName());

		if (d_m==NULL)
		{
			return -3;
		}
		
		if (!rq.getFormat().compare("str"))
		{
			std::stringstream inv(rq.getValue());
			std::stringstream outv;
					
			
			if (rq.getValue().size()==0)
			{					
					return -4;
			}
					
			opros::archive::string_iarchive d_in_arc(&inv);
			opros::archive::string_oarchive d_out_arc(&outv);

			ReturnType cc_ret;
			try {
			 cc_ret = d_m->call(d_in_arc, d_out_arc);	
			}
			catch (MethodInvokeException mx)
			{
					return -5;
			}
			catch (opros::archive::archive_type_exception etx)
			{
					return  -6;
			}
			catch (opros::archive::archive_format_exception efx)
			{			
					return  -7;
			}
					
			if (cc_ret != OPROS_SUCCESS)
			{
						return -8;
			}

			rq.setReturnValue( outv.str());
		}
		else if (!rq.getFormat().compare("bin"))
		{
			std::stringstream inv(rq.getValue());
			std::stringstream outv;
					
			if (rq.getValue().size()==0)
			{					
					return -9;
			}
					
			opros::archive::binary_iarchive d_in_arc(&inv);
			opros::archive::binary_oarchive d_out_arc(&outv);

			ReturnType cc_ret;
			try {
			 cc_ret = d_m->call(d_in_arc, d_out_arc);	
			}
			catch (opros::archive::archive_type_exception etx)
			{
					return -10;
			}
			catch (opros::archive::archive_format_exception efx)
			{			
					return -11;
			}

			if (cc_ret != OPROS_SUCCESS)
			{
				return -12;
			}

			rq.setReturnValue( outv.str());
		}				
	}

	return 0;
}
