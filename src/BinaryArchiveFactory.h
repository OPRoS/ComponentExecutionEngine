/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2011. 3. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : BinaryArchiveFactory.h
 *
 *
 */

#ifndef BINARYARCHIVEFACTORY_H_
#define BINARYARCHIVEFACTORY_H_
#include "ArchiveFactory.h"

class BinaryArchiveFactory : public ArchiveFactory{
public:
	BinaryArchiveFactory();
	virtual ~BinaryArchiveFactory();

	virtual opros::archive::oarchive *getOutputArchive(std::ostream *str);	
	virtual opros::archive::iarchive *getInputArchive(std::istream *str);

	virtual void release(opros::archive::oarchive *arc);
	virtual void release(opros::archive::iarchive *arc);
};

#endif /* BINARYARCHIVEFACTORY_H_ */
