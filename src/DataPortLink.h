/*
 * DataPortLink.h
 *
 *  Created on: 2008. 9. 30
 *      Author: sby
 */

#ifndef DATAPORTLINK_H_

#define DATAPORTLINK_H_

#include <list>
#include <OPRoSTypes.h>
#include <DataPort.h>
#include <IDataPortLink.h>
#include <ReturnType.h>

class DataPortLinkImpl;
class DataPortLink: public IDataPortLink {

	DataPortLinkImpl *m_impl;

public:
	virtual ReturnType push(opros_any & data);
	virtual ReturnType setSource(DataPort *src);
	virtual ReturnType addDestination(DataPort *dest);
	virtual ReturnType addDestination(RemoteDataPort *dest);
	virtual ReturnType removeDestination(DataPort *dest);
	virtual ReturnType removeDestination(RemoteDataPort *dest);
	virtual ReturnType clearDestination();
	virtual ReturnType clearSource();
	DataPortLink();
	virtual ~DataPortLink();
};

#endif /* DATAPORTLINK_H_ */
