/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Profile.h
 *
 */

#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <string>

#include "system/config/Config.h"
#include "system/util/Exceptions.h"
#include "system/util/Printable.h"
#include "system/util/StringConverter.h"

using namespace opros::engine;
/**
	모든 프로파일의 최상위 클래스.

	@see ComponentProfile
	@see CompositeComponentProfile
	@see ApplicationProfile
*/
class Profile : public StringConverter, public Printable
{
protected:
	/** xml 프로파일에서 읽은 정보를 저장하는 Config 객체 */
	Config *m_cfg;

	/** xml 프로파일의 경로 정보 */
	std::string  m_path;

public:
	Profile();
	virtual ~Profile();

	virtual std::string getName() = 0;

	virtual std::string  getProfileTag() = 0;
	Config *getConfig() { return m_cfg; };

	std::string  getPath() { return m_path; };
	void setPath(const std::string  &path) { m_path = path; };

	virtual bool parse(Config *cfg);

protected :
	virtual bool doParse(Config *cfg) = 0;
};

/**
	프로파일 내의 일부 정보를 나타내는 info 객체의 최상위 클래스.
*/
class ProfileInfo : public StringConverter, public Printable
{
public:
	virtual void parse(Config *cfg) = 0;
};

#endif 
