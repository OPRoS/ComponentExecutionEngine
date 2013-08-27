/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 10. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CompositeCompositeInputDataPort.h
 *
 *
 */

#ifndef COMPOSITE_INPUTDATAPORT_H_
#define COMPOSITE_INPUTDATAPORT_H_

#include <OPRoSTypes.h>
#include <InputDataPort.h>
#include <ReturnType.h>
class CompositeInputDataPortImpl;

class CompositeInputDataPort : public DataPort{
	CompositeInputDataPortImpl *m_provider;
public:
	CompositeInputDataPort();
	CompositeInputDataPort(const std::string &name);


	virtual DataPortRole getRole();

	virtual opros_any *peek();
	virtual opros_any *pop();

	virtual void reset();

	virtual void setArchiveFactory(ArchiveFactory *darc);
	virtual ReturnType push(opros_any & data);
	virtual ReturnType pushData(unsigned char *data, int size);

	virtual bool checkType(opros_any & data);
	virtual bool checkType(DataPort *dst);

	virtual void addPort(DataPort *p);
	virtual void removePort(DataPort *p);
	virtual ~CompositeInputDataPort();
};

#endif /* COMPOSITE_INPUTDATAPORT_H_ */
