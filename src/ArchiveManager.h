
/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 4. 7
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ArchiveManager.h
 *
 *
 */

#ifndef ARCHIVEMANAGER_H_
#define ARCHIVEMANAGER_H_
#include "system/Manager.h"
#include "ArchiveFactory.h"
#include <map>
#define ARCHIVE_MANAGER		"archive_manager"
#define DEFAULT_ARCHIVE_TYPE 		"binary"

class ArchiveFactory;
typedef std::map<std::string , ArchiveFactory *> ArchiveFactoryList;

class ArchiveManager : public Manager{
	ArchiveFactoryList m_list;
	std::string m_default_type;
public:
	ArchiveManager(const std::string &def_type);
	virtual ~ArchiveManager();

	void addArchiveFactory(const std::string &msg_type, ArchiveFactory *ptr);
	void removeArchiveFactory(const std::string &msg_type);

	ArchiveFactory *getArchiveFactory(const std::string &msg_type);
	ArchiveFactory *getDefaultArchiveFactory();
};

#endif /* ARCHIVEMANAGER_H_ */
