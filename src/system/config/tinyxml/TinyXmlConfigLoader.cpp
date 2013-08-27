/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 17
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : TinyXmlConfigLoader.cpp
 *
 */

#include "TinyXmlConfigLoader.h"
#include "TinyXmlConfig.h"

#include "tinyxml.h"

using namespace std;
using namespace opros::engine;

TinyXmlConfigLoader::TinyXmlConfigLoader()
{
}

TinyXmlConfigLoader::~TinyXmlConfigLoader()
{
}

Config *TinyXmlConfigLoader::loadFromFile(const string &cfgFilePath) throw (IOException)
{
	TiXmlDocument *doc = new TiXmlDocument(cfgFilePath.c_str());
	if (!doc->LoadFile()) {
		delete doc;
		return NULL;
	}

	TinyXmlConfig *root = new TinyXmlConfig();
	root->setTiXmlNode(doc);
	addConfig(root, doc);
	return root;
}

void TinyXmlConfigLoader::addConfig(Config *pcfg, TiXmlNode *parent)
{
	TiXmlElement *elem;

	TiXmlNode *node = parent->FirstChild();
	while (node != NULL)
	{
		elem = node->ToElement();
		if (elem != NULL) {
			TinyXmlConfig *cfg = new TinyXmlConfig();
			cfg->setTiXmlNode(elem);			
			cfg->setDeleteNode(false);

			pcfg->addChild(cfg);
			addConfig(cfg, elem);
		}
		node = node->NextSibling();
	}
}

void TinyXmlConfigLoader::saveToFile(Config *cfg, const string &cfgFilePath) throw (IOException)
{
	TinyXmlConfig *tcfg = portable_dynamic_cast(TinyXmlConfig *, cfg);
	if (tcfg == NULL) {
		ConfigLoader::saveToFile(cfg, cfgFilePath);
		return;
	}

	TiXmlNode *node = tcfg->getTiXmlNode();
	TiXmlDocument *pdoc = node->ToDocument();
	if (pdoc != NULL) {
		pdoc->SaveFile(cfgFilePath);
	}
	else {
		TiXmlDocument doc;
		doc.LinkEndChild( node );
		doc.SaveFile( cfgFilePath );
	}
}

Config *TinyXmlConfigLoader::loadFromStream(istream &is) throw (IOException)
{
	throw new IOException("Not supported function : TinyXmlConfigLoader::loadFromStream");
}

void TinyXmlConfigLoader::saveToStream(Config *cfg, ostream &os) throw (IOException)
{
	throw new IOException("Not supported function : TinyXmlConfigLoader::saveToStream");
}
