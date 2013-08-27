/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 28
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PDFConfig.h
 *
 */

#ifndef PDFCONFIG_H_
#define PDFCONFIG_H_

// PDFConfig
#include "system/config/Config.h"
using namespace opros::engine;
/**
	properties 파일을 이용하여 설정 정보를 제공하는 클래스
*/
class PDFConfig: public Config
{
protected :
	std::string m_name;
	std::string m_value;

public:
	PDFConfig();
	PDFConfig(const std::string  &name, const std::string  &value) : m_name(name), m_value(value) {};
	virtual ~PDFConfig();

	virtual Config *createConfig() { return new PDFConfig(); };
	virtual Config *createConfig(const std::string  &name, const std::string  &value) {
		return new PDFConfig(name, value);
	};

	virtual std::string  getName() { return m_name; };
	virtual void setName(const std::string  &name) { m_name = name; };

	virtual void setValue(const std::string  &value) { m_value = value; };
	virtual std::string  getValue() throw (InvalidArgumentException) { return m_value; };

	virtual StringList getAttributeNames() { StringList tmp; return tmp; };
	virtual std::string  getAttribute(std::string  paramName) throw (InvalidArgumentException) {
		throw InvalidArgumentException("Not Supported in PDFConfig");
	}
	virtual void removeAttribute(const std::string  &paramName) {};
	virtual void removeAttributeAll() {};

	virtual void addAttribute(const std::string  &attrName, const std::string  &attrValue) {};

	virtual void print(std::ostream &os);
};

#endif 
