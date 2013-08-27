/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 28
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TinyXmlConfig.cpp
 *
 */

#include "TinyXmlConfig.h"

#include "system/util/BasicTypes.h"

using namespace std;
using namespace opros::engine;

Config *TinyXmlConfig::createConfig()
{
	TiXmlElement *elem = new TiXmlElement("");
	TinyXmlConfig *cfg = new TinyXmlConfig(portable_dynamic_cast(TiXmlNode *, elem));
	return cfg;
}

string TinyXmlConfig::getName()
{
	if (m_node == NULL) return "";
	return m_node->ValueStr();
}

void TinyXmlConfig::setName(const string &name)
{
	if (m_node == NULL) return;
	m_node->SetValue(name.c_str());
}

void TinyXmlConfig::setValue(const string &value)
{
	if (m_node == NULL) return;

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		TiXmlNode *node = elem->FirstChild();
		TiXmlText* childText = NULL;
		if (node != NULL) {
			childText = node->ToText();
			if ( childText != NULL ) childText->SetValue(value);
		}
		else {
			childText = new TiXmlText(value);
			elem->LinkEndChild(childText);
		}
	}
}

string TinyXmlConfig::getValue() throw (InvalidArgumentException)
{
	if (m_node == NULL) throw InvalidArgumentException("There is no value.");

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		const char *text = elem->GetText();
		if (text == NULL) return "";
		else return string(text);
	}
	else throw InvalidArgumentException("There is no value.");
}

StringList TinyXmlConfig::getAttributeNames()
{
	StringList tmp;

	if (m_node == NULL) return tmp;

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		TiXmlAttribute *attr = elem->FirstAttribute();
		while (attr != NULL) {
			tmp.push_back(attr->Name());
			attr = attr->Next();
		}
	}

	return tmp;
}

string TinyXmlConfig::getAttribute(string paramName) throw (InvalidArgumentException)
{
	if (m_node == NULL) throw InvalidArgumentException("No such attribute : " +  paramName);

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		const string *value = elem->Attribute(paramName);
		if (value != NULL) return *value;
	}

	throw InvalidArgumentException("No such attribute : " +  paramName) ;
}

void TinyXmlConfig::removeAttribute(const string &paramName)
{
	if (m_node == NULL) return;

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		elem->RemoveAttribute(paramName);
	}
}

void TinyXmlConfig::removeAttributeAll()
{
	if (m_node == NULL) return;

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		TiXmlAttribute *attr = elem->FirstAttribute();
		while (attr != NULL) {
			elem->RemoveAttribute(attr->Name());
			attr = elem->FirstAttribute();
		}
	}
}

void TinyXmlConfig::addAttribute(const string &attrName, const string &attrValue)
{
	if (m_node == NULL) return;

	TiXmlElement *elem = m_node->ToElement();
	if (elem != NULL) {
		elem->SetAttribute(attrName, attrValue);
	}
}
