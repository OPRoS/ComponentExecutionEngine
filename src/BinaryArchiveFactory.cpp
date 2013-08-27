/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 7
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : BinaryArchiveFactory.cpp
 *
 *
 */

#include "BinaryArchiveFactory.h"
#include "archive/opros_archive.h"


BinaryArchiveFactory::BinaryArchiveFactory() {
	// TODO Auto-generated constructor stub

}

BinaryArchiveFactory::~BinaryArchiveFactory() {
	// TODO Auto-generated destructor stub
}

opros::archive::oarchive *BinaryArchiveFactory::getOutputArchive(std::ostream *str)
{
	return new opros::archive::binary_oarchive(str);

}

opros::archive::iarchive *BinaryArchiveFactory::getInputArchive(std::istream *str)
{
	return new opros::archive::binary_iarchive(str);
}

void BinaryArchiveFactory::release(opros::archive::oarchive *arc) {
	delete arc;
}
void BinaryArchiveFactory::release(opros::archive::iarchive *arc) {
	delete arc;
}

