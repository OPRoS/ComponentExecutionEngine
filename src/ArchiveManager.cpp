/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 7
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ArchiveManager.cpp
 *
 *
 */

#include "ArchiveManager.h"
#include "BinaryArchiveFactory.h"

ArchiveManager::ArchiveManager(const std::string &def_type) {

	m_default_type = def_type;
	addArchiveFactory(m_default_type, new BinaryArchiveFactory());
}

ArchiveManager::~ArchiveManager() {
	ArchiveFactoryList::iterator it;
	for (it = m_list.begin(); it != m_list.end(); ++it) {
		ArchiveFactory *cur = it->second;
		delete cur;
	}

	m_list.clear();
}

ArchiveFactory *ArchiveManager::getArchiveFactory(const std::string &msg_type) {
	ArchiveFactoryList::iterator rt = m_list.find(msg_type);

	if (rt != m_list.end()) {
		return rt->second;
	}

	return NULL;
}

ArchiveFactory *ArchiveManager::getDefaultArchiveFactory() {
	ArchiveFactoryList::iterator rt = m_list.find(m_default_type);

	if (rt != m_list.end()) {
		return rt->second;
	}

	return NULL;
}

void ArchiveManager::addArchiveFactory(const std::string &msg_type,
		ArchiveFactory *ptr) {

	ArchiveFactoryList::iterator rt= m_list.find(msg_type);

	if (rt != m_list.end())
	{
		ArchiveFactory *temp = rt->second;
		if (temp == ptr)
			return;
		else
		{
			m_list.erase(rt);
			delete temp;
			m_list[msg_type] = ptr;
		}
	}
	else
		m_list[msg_type] = ptr;
}

void ArchiveManager::removeArchiveFactory(const std::string &msg_type)
{
	m_list.erase(msg_type);
}
