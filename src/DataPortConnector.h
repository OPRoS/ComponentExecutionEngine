/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : DataPortConnector.h
 *
 *
 */

#ifndef DATAPORTCONNECTOR_H_
#define DATAPORTCONNECTOR_H_
#include "ReturnType.h"

/**
 *	DataPortConnector is the abstract base class for connector classes
 *	         	        to connect a data port with a consistent data port on remote hosts.
 */
class DataPortConnector {
public:
	DataPortConnector(){};
	virtual ~DataPortConnector(){};

	/**
	 * Send data from a source data port to a destination data port on a remote host.
	 */
	virtual ReturnType sendData(unsigned char *data, int len) = 0;
	virtual ReturnType recvData(unsigned char *data, int len) = 0;
};

#endif /* DATAPORTCONNECTOR_H_ */
