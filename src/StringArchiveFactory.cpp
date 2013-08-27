/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 7
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : StringArchiveFactory.cpp
 *
 *
 */

#include "StringArchiveFactory.h"
#include "archive/opros_archive.h"


StringArchiveFactory::StringArchiveFactory() {
	// TODO Auto-generated constructor stub

}

StringArchiveFactory::~StringArchiveFactory() {
	// TODO Auto-generated destructor stub
}

opros::archive::oarchive *StringArchiveFactory::getOutputArchive(std::ostream *str)
{
	return new opros::archive::string_oarchive(str);

}

opros::archive::iarchive *StringArchiveFactory::getInputArchive(std::istream *str)
{
	return new opros::archive::string_iarchive(str);
}

void StringArchiveFactory::release(opros::archive::oarchive *arc) {
	delete arc;
}
void StringArchiveFactory::release(opros::archive::iarchive *arc) {
	delete arc;
}

