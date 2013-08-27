/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Config.h
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <cstring>
#include <string>
#include <list>
#include <iostream>
#include <fstream>

#include "system/util/BasicTypes.h"
#include "system/util/Exceptions.h"
#include "system/util/Properties.h"
#include "system/util/StringConverter.h"

namespace opros {
	namespace engine {

class Config;
typedef std::list<Config *> ConfigList;

/**
	XML 등의 설정 파일을 파싱한 후 해당 정보를 저장하는 클래스.
	태그 정보, attribute 정보, value 정보, 하위 태그 정보등을 
	저장하며, 계층적 구조를 이룸.
*/
class Config : public StringConverter {
protected :

	int m_level;
	bool m_readOnly;

	std::string m_location;
	std::string m_namespace;
	std::string m_prefix;

	ConfigList m_children;

	void releaseChildren();

public:
	Config() : m_level(0), m_readOnly(false), trace_on(false) {};
	virtual ~Config();

	bool trace_on;

	friend std::ostream & operator<<(std::ostream & os, Config &cfg) {
		cfg.print(os);
		return os;
	};
	friend std::ostream & operator<<(std::ostream & os, Config *cfg) {
		if (cfg != NULL) cfg->print(os);
		else os << "Config=NULL";

		return os;
	};

	virtual Config *createConfig() = 0;
	virtual Config *createConfig(const std::string &name, const std::string &value);

	int getLevel() {return m_level; };
	void setLevel(int level) { m_level = level; };

	bool isReadOnly() { return m_readOnly; };
	void setReadOnly(bool readOnly) { m_readOnly = readOnly; };
	void makeReadOnly() { m_readOnly = true; };

	bool operator==(Config &cfg) { return getName() == cfg.getName(); };
	bool operator!=(Config &cfg) { return getName() != cfg.getName();	};
	bool operator<(Config &cfg) { return getName() < cfg.getName(); };

	/**
	 * Return the name of the node.
	 *
	 * @return name of the <code>Config</code> node.
	 */
	std::string getLocation() { return m_location; };
	void setLocation(const std::string &location) { m_location = location; };

    /**
     * Returns a std::string indicating which namespace this Config node
     * belongs to.
     *
     * @return a String identifying the namespace of this Config.
     * @throws InvalidArgumentException if an error occurs
     */
    std::string getNamespace() { return m_namespace; };
	void setNamespace(const std::string &ns) { m_namespace = ns; };

	/**
	 * Returns the prefix of the namespace
	 * @return a <code>String</code> value
	 * @throws ConfigurationException if prefix is not present (<code>null</code>).
	 * @since 4.1
	 */
	std::string getPrefix() { return m_prefix; };
	void setPrefix(const std::string &pf) { m_prefix = pf; };

	/**
	 * Return a <code>Config</code> instance encapsulating the specified
	 * child node.
	 *
	 * @param child The name of the child node.
	 * @param createNew If <code>true</code>, a new <code>Config</code>
	 * will be created and returned if the specified child does not exist. If
	 * <code>false</code>, <code>null</code> will be returned when the specified
	 * child doesn't exist.
	 * @return Config
	 */
	Config *getChild(const std::string &childName);

	/**
	 * Return a <code>list </code> of <code>Config</code>
	 * elements containing all node children. The array order will reflect the
	 * order in the source config file.
	 *
	 * @return All child nodes
	 */
	ConfigList getChildren();

	/**
	 * Return a <code>list</code> of <code>Config</code>
	 * elements containing all node children with the specified name. The array
	 * order will reflect the order in the source config file.
	 *
	 * @param name The name of the children to get.
	 * @return The child nodes with name <code>name</code>
	 */
	ConfigList getChildren(std::string name);

	std::string getChildAttribute(std::string childname, std::string paramName) throw (InvalidArgumentException);

	/**
	 * Return the <code>bool</code> value of the specified parameter contained
	 * in this node.
	 *
	 * @param paramName The name of the parameter you ask the value of.
	 * @return bool value of attribute
	 * @throw (InvalidArgumentException) If no parameter with that name exists.
	 *                                   or if conversion to <code>bool</code> fails.
	 */
	bool getAttributeAsBool(std::string paramName) throw (InvalidArgumentException);
	bool getChildAttributeAsBool(std::string childname, std::string paramName) throw (InvalidArgumentException);

	/**
	 * Return the <code>char</code> value of the specified parameter contained
	 * in this node.
	 *
	 * @param paramName The name of the parameter you ask the value of.
	 * @return char value of attribute
	 * @throw (InvalidArgumentException) If no parameter with that name exists.
	 *                                   or if conversion to <code>char</code> fails.
	 */
	char getAttributeAsChar(std::string paramName) throw (InvalidArgumentException);
	char getChildAttributeAsChar(std::string childname, std::string paramName) throw (InvalidArgumentException);

	/**
	 * Return the <code>int</code> value of the specified attribute contained
	 * in this node.
	 *
	 * @param paramName The name of the parameter you ask the value of.
	 * @return int value of attribute
	 * @throw (InvalidArgumentException) If no parameter with that name exists.
	 *                                   or if conversion to <code>int</code> fails.
	 *
	 */
	int getAttributeAsInt(std::string paramName) throw (InvalidArgumentException);
	int getChildAttributeAsInt(std::string childname, std::string paramName) throw (InvalidArgumentException);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>long</code>.
	 *
	 * @param name The name of the parameter you ask the value of.
	 * @return long value of attribute
	 * @throw (InvalidArgumentException) If no parameter with that name exists.
	 *                                   or if conversion to <code>long</code> fails.
	 */
	long getAttributeAsLong(std::string name) throw (InvalidArgumentException);
	long getChildAttributeAsLong(std::string childname, std::string paramName) throw (InvalidArgumentException);

	/**
	 * Return the <code>float</code> value of the specified parameter contained
	 * in this node.
	 *
	 * @param paramName The name of the parameter you ask the value of.
	 * @return float value of attribute
	 * @throw (InvalidArgumentException) If no parameter with that name exists.
	 *                                   or if conversion to <code>float</code> fails.
	 */
	float getAttributeAsFloat(std::string paramName) throw (InvalidArgumentException);
	float getChildAttributeAsFloat(std::string childname, std::string paramName) throw (InvalidArgumentException);

	/**
	 * Return the <code>double</code> value of the specified parameter contained
	 * in this node.
	 *
	 * @param paramName The name of the parameter you ask the value of.
	 * @return double value of attribute
	 * @throw (InvalidArgumentException) If no parameter with that name exists.
	 *                                   or if conversion to <code>double</code> fails.
	 */
	double getAttributeAsDouble(std::string paramName) throw (InvalidArgumentException);
	double getChildAttributeAsDouble(std::string childname, std::string paramName) throw (InvalidArgumentException);

	std::string getChildValue(std::string childname) throw (InvalidArgumentException);

	/**
	 * Return the <code>bool</code> value of the node.
	 *
	 * @return the value of the node.
	 * @throw (InvalidArgumentException) If conversion to <code>bool</code> fails.
	 */
	bool getValueAsBool() throw (InvalidArgumentException);
	bool getChildValueAsBool(std::string childname) throw (InvalidArgumentException);

	/**
	 * Return the <code>char</code> value of the node.
	 *
	 * @return the value of the node.
	 * @throw (InvalidArgumentException) If conversion to <code>char</code> fails.
	 */
	char getValueAsChar() throw (InvalidArgumentException);
	char getChildValueAsChar(std::string childname) throw (InvalidArgumentException);

	/**
	 * Return the <code>int</code> value of the node.
	 *
	 * @return the value of the node.
	 *
	 * @throw (InvalidArgumentException) If conversion to <code>int</code> fails.
	 */
	int getValueAsInt() throw (InvalidArgumentException);
	int getChildValueAsInt(std::string childname) throw (InvalidArgumentException);

	/**
	 * Return the <code>long</code> value of the node.
	 *
	 * @return the value of the node.
	 * @throw (InvalidArgumentException) If conversion to <code>long</code> fails.
	 */
	long getValueAsLong() throw (InvalidArgumentException);
	long getChildValueAsLong(std::string childname) throw (InvalidArgumentException);

	/**
	 * Return the <code>float</code> value of the node.
	 *
	 * @return the value of the node.
	 * @throw (InvalidArgumentException) If conversion to <code>float</code> fails.
	 */
	float getValueAsFloat() throw (InvalidArgumentException);
	float getChildValueAsFloat(std::string childname) throw (InvalidArgumentException);

	/**
	 * Return the <code>double</code> value of the node.
	 *
	 * @return the value of the node.
	 * @throw (InvalidArgumentException) If conversion to <code>double</code> fails.
	 */
	double getValueAsDouble() throw (InvalidArgumentException);
	double getChildValueAsDouble(std::string childname) throw (InvalidArgumentException);

	/**
	 * Returns the value of the configuration element as a <code>std::string</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return std::string value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	std::string getValue(std::string defaultValue);
	std::string getChildValue(std::string childname, std::string defaultValue);

	/**
	 * Returns the value of the configuration element as a <code>bool</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return bool value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	bool getValueAsBool(bool defaultValue);
	bool getChildValueAsBool(std::string childname, bool defaultValue);

	/**
	 * Returns the value of the configuration element as a <code>char</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return char value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	char getValueAsChar(char defaultValue);
	char getChildValueAsChar(std::string childname, char defaultValue);

	/**
	 * Returns the value of the configuration element as an <code>int</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return int value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	int getValueAsInt(int defaultValue);
	int getChildValueAsInt(std::string childname, int defaultValue);

	/**
	 * Returns the value of the configuration element as a <code>long</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return long value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	long getValueAsLong(long defaultValue);
	long getChildValueAsLong(std::string childname, long defaultValue);

	/**
	 * Returns the value of the configuration element as a <code>float</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return float value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	float getValueAsFloat(float defaultValue);
	float getChildValueAsFloat(std::string childname, float defaultValue);

	/**
	 * Returns the value of the configuration element as a <code>double</code>.
	 * If the configuration value is not set, the default value will be
	 * used.
	 *
	 * @param defaultValue The default value desired.
	 * @return double value of the <code>Config</code>, or default
	 *          if none specified.
	 */
	double getValueAsDouble(double defaultValue);
	double getChildValueAsDouble(std::string childname, double defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>std::string</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return std::string value of attribute. It will return the default
	 *         value if the named attribute does not exist, or if
	 *         the value is not set.
	 */
	std::string getAttribute(std::string name, std::string defaultValue);
	std::string getChildAttribute(std::string childname, std::string name, std::string defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>bool</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return bool value of attribute. It will return the default
	 *         value if the named attribute does not exist, or if
	 *         the value is not set.
	 */
	bool getAttributeAsBool(std::string name, bool defaultValue);
	bool getChildAttributeAsBool(std::string childname, std::string name, bool defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>char</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return char value of attribute. It will return the default
	 *         value if the named attribute does not exist, or if
	 *         the value is not set.
	 */
	char getAttributeAsChar(std::string name, char defaultValue);
	char getChildAttributeAsChar(std::string childname, std::string name, char defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>int</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return int value of attribute. It will return the default
	 *         value if the named attribute does not exist, or if
	 *         the value is not set.
	 */
	int getAttributeAsInt(std::string name, int defaultValue);
	int getChildAttributeAsInt(std::string childname, std::string name, int defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>long</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return long value of attribute. It will return the default
	 *          value if the named attribute does not exist, or if
	 *          the value is not set.
	 */
	long getAttributeAsLong(std::string name, long defaultValue);
	long getChildAttributeAsLong(std::string childname, std::string name, long defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>float</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return float value of attribute. It will return the default
	 *          value if the named attribute does not exist, or if
	 *          the value is not set.
	 */
	float getAttributeAsFloat(std::string name, float defaultValue);
	float getChildAttributeAsFloat(std::string childname, std::string name, float defaultValue);

	/**
	 * Returns the value of the attribute specified by its name as a
	 * <code>double</code>, or the default value if no attribute by
	 * that name exists or is empty.
	 *
	 * @param name The name of the attribute you ask the value of.
	 * @param defaultValue The default value desired.
	 * @return double value of attribute. It will return the default
	 *          value if the named attribute does not exist, or if
	 *          the value is not set.
	 */
	double getAttributeAsDouble(std::string name, double defaultValue);
	double getChildAttributeAsDouble(std::string childname, std::string name, double defaultValue);

	/**
     * Return the name of the node.
     *
     * @return name of the <code>Config</code> node.
     */
	virtual std::string getName() = 0;
	virtual void setName(const std::string &name) = 0;

	/**
     * Set value of the node
     *
     * @return value of the <code>Config</code> node.
     */
	virtual void setValue(const std::string &value) = 0;

	/**
	 * Return the <code>std::string</code> value of the node.
	 *
	 * @return the value of the node.
	 * @throw (InvalidArgumentException) if an error occurs
	 */
	virtual std::string getValue() throw (InvalidArgumentException) = 0;

	void setValue(bool value);
	void setValue(char value);
	void setValue(int value);
	void setValue(long value);
	void setValue(float value);
	void setValue(double value);

	virtual Config *addChild(const std::string &name, const std::string &value = "");
	virtual void addChild(Config *cfg);
	void removeChild(const std::string &name);
	void removeChildren(const std::string &name);
	void removeChildren();

   /**
	 * Return a list of all attribute names.
	 * <p>
	 * <em>The order of attributes in this array can not be relied on.</em>
	 * As with XML, a <code>Config</code>'s attributes are an
	 * <em>unordered</em> set. If your code relies on order, eg
	 * <tt>conf.getAttributeNames()[0]</tt>, then it is liable to break if a
	 * different XML parser is used.
	 * </p>
	 * @return a <code>String[]</code> value
	 */
	virtual StringList getAttributeNames() = 0;

	/**
	 * Return the value of specified attribute.
	 *
	 * @param paramName The name of the parameter you ask the value of.
	 * @return String value of attribute.
	 * @throw (InvalidArgumentException) If no attribute with that name exists.
	 */
	virtual std::string getAttribute(std::string paramName) throw (InvalidArgumentException) = 0;
	virtual void removeAttribute(const std::string &paramName) = 0;
	virtual void removeAttributeAll() = 0;

	virtual void addAttribute(const std::string &attrName, const std::string &attrValue) = 0;
	void addAttribute(const std::string &attrName, bool attrValue);
	void addAttribute(const std::string &attrName, char attrValue);
	void addAttribute(const std::string &attrName, int attrValue);
	void addAttribute(const std::string &attrName, long attrValue);
	void addAttribute(const std::string &attrName, float attrValue);
	void addAttribute(const std::string &attrName, double attrValue);

	virtual void print();
	virtual void print(const std::string &filename);
	virtual void print(std::ostream &os);
	void printAttributes(std::ostream &os);

	void toProperties(Properties &props);
	void toProperties(Properties &props, const std::string &tagName);
};

} // namespace opros
} //	namespace engine

#endif
