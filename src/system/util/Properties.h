/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Properties.h
 *
 */
#if !defined(_PROPERTY_H)
#define _PROPERTY_H

#include <iostream>
#include <string>
#include <map>

#include "StringConverter.h"
#include "Printable.h"
#include "system/os/OSGuard.h"
#include "system/os/OSMutex.h"
/**
	(Name, Value) 리스트를 관리하는 클래스.
 */
class Properties : public StringConverter, public Printable
{
	typedef std::map<std::string, std::string> StringMap;

	OSMutex m_mutex;

	StringMap m_map;

	bool parsingLine(const std::string &line);

public:
	Properties();
	Properties(Properties &props);
	virtual ~Properties();

	friend std::ostream & operator<<(std::ostream &os, Properties &props) {
		props.print(os);
		return os;
	};
	friend std::ostream & operator<<(std::ostream &os, Properties *props) {
		if (props != NULL) props->print(os);
		else os << "Properties=NULL";

		return os;
	};


	void copy(Properties &props);

	void setProperty(const std::string &name, const std::string &value);
	const std::string &getProperty(const std::string &name) throw (InvalidArgumentException);
	const std::string &getProperty(const std::string &name, const std::string &defaultValue) const;

	bool isExist(const std::string &name);
	bool removeProperty(const std::string &name);
	void clearProperties();

	bool getBool(const std::string &name) throw (InvalidArgumentException) ;
	bool getBool(const std::string &name, bool defaultValue);

	char getChar(const std::string &name) throw (InvalidArgumentException) ;
	char getChar(const std::string &name, char defaultValue);

	int getInt(const std::string &name) throw (InvalidArgumentException) ;
	int getInt(const std::string &name, int defaultValue);

	long getLong(const std::string &name) throw (InvalidArgumentException) ;
	long getLong(const std::string &name, long defaultValue);

	float getFloat(const std::string &name) throw (InvalidArgumentException) ;
	float getFloat(const std::string &name, float defaultValue);

	double getDouble(const std::string &name) throw (InvalidArgumentException) ;
	double getDouble(const std::string &name, double defaultValue);

	void toProperties(const std::string& source, char separatorChar);

	void getNames(std::vector<std::string> &list);

	bool load(const std::string &filepath);
	bool load(std::istream &is);

	bool save(const std::string &filepath);
	bool save(std::ostream &os);

	// from Printable
	virtual void print(std::ostream &os, int level = 0);

	// for test
	void test();

	Properties &operator=(Properties &p);
};



#endif /* _PROPERTY_H */
