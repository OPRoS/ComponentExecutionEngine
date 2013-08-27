/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : ComponentVerifier.cpp
 *
 */

#ifndef _COMPONENT_COMPONENT_VERIFIER_H
#define _COMPONENT_COMPONENT_VERIFIER_H

#include <modconfig/ModConfig.h>

#ifdef	MOD_VERIFY_COMPONENT

#include <string>


#include "system/config/ConfigLoaderFactory.h"

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/bn.h>

class Signature
{
public:
	unsigned char sig_data[4096];
	int sig_len;
};


class ComponentVerifier
{
protected:

	EVP_PKEY *pkey;	// sign cert	
	ConfigLoader *loader;

public:
	ComponentVerifier();
	virtual ~ComponentVerifier();

	int init();

	int verifyComponent(const std::string &path, Config *cfg);

	
	int decodePEM(Signature &sigdata, const char *buf, int buflen);
	int verifyIt(string &libpath, Signature & sig);

};

#endif

#endif

