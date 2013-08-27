/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2011. 3. 2
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : StringArchiveFactory.h
 *
 *
 */

#ifndef STRINGARCHIVEFACTORY_H_
#define STRINGARCHIVEFACTORY_H_
#include "ArchiveFactory.h"

class StringArchiveFactory : public ArchiveFactory{
public:
	StringArchiveFactory();
	virtual ~StringArchiveFactory();

	virtual opros::archive::oarchive *getOutputArchive(std::ostream *str);	
	virtual opros::archive::iarchive *getInputArchive(std::istream *str);

	virtual void release(opros::archive::oarchive *arc);
	virtual void release(opros::archive::iarchive *arc);
};

#endif /* STRINGARCHIVEFACTORY_H_ */
