/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataObjectMap.cpp
 *
 */

#include "DataObjectMap.h"

using namespace std;

DataObjectMap::DataObjectMap()
{
}

DataObjectMap::~DataObjectMap()
{
	clear();
}

void DataObjectMap::clear()
{
	DataObjectMapType::const_iterator pos = m_map.begin();
	for (; pos != m_map.end(); ++pos) {
		if (pos->second != NULL) delete pos->second;
	}
	m_map.clear();
}

StringList DataObjectMap::getNames()
{
	StringList list;
	DataObjectMapType::const_iterator pos = m_map.begin();
	for (; pos != m_map.end(); ++pos) {
		list.push_back(pos->first);
	}

	return list;
}

void DataObjectMap::setData(const string &name, DataObject *data, bool owned)
{
	DataHolder *holder = new DataHolder();
	holder->owned = owned;
	holder->data = data;

	m_map.insert(std::make_pair(name, holder));
}

DataObject *DataObjectMap::getData(const string &name)
{
	DataObjectMapType::const_iterator pos = m_map.find(name);
	if (pos == m_map.end()) return NULL;
	else return pos->second->data;
}

DataObject *DataObjectMap::getData(const string &name, DataObject *defaultData)
{
	DataObjectMapType::const_iterator pos = m_map.find(name);
	if (pos == m_map.end()) return defaultData;
	else return pos->second->data;
}

bool DataObjectMap::isExist(const string &name)
{
	DataObjectMapType::const_iterator pos = m_map.find(name);
	if (pos == m_map.end()) return false;
	else return true;
}

bool DataObjectMap::removeData(const string &name)
{
	m_map.erase(m_map.find(name));
	return true;
}

