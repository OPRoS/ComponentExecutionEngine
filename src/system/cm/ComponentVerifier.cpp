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

/*
class ComponentVerifier
{
protected:

	EVP_PKEY *pkey;	// sign cert	

public:
	ComponentVerifier();
	virtual ~ComponentVerifier();


	bool verifyComponent(Config *cfg);
};
*/
#ifdef	MOD_VERIFY_COMPONENT

#include "ComponentVerifier.h"
#include <stdio.h>
#include <iostream>
#include "system/Registry.h"


#define DEFAULT_SIG_CERT "authcert.pem"
#define COMPONENT_PROFILE "component_profile"
#define COMPONENT_NAME "name"
#define EXE_ENV "execution_environment"
#define LIBRARY_NAME "library_name"

#define BINARY_VERIFICATION "binary_verification"

using namespace std;


ComponentVerifier::ComponentVerifier()
{
	pkey = NULL;
	loader = NULL;
	ERR_load_crypto_strings();
}

ComponentVerifier::~ComponentVerifier()
{
	if (pkey != NULL) EVP_PKEY_free(pkey);
}

int ComponentVerifier::init()
{
	string filename = DEFAULT_SIG_CERT;

	FILE *fp;

	fp = fopen(filename.c_str(), "r");
	if (fp == NULL)
	{
		cout <<"Cannot find key file:"<<filename<<endl;
		return -1;
	}
	
	X509 *x509;

	x509 = PEM_read_X509(fp, NULL, NULL, NULL);

	fclose(fp);

	if (x509 == NULL)
	{
		ERR_print_errors_fp(stderr);
		return -2;
	}

	pkey = X509_get_pubkey(x509);

	X509_free(x509);

	if (pkey == NULL)
	{	
		ERR_print_errors_fp(stderr);

		return -3;
	}


	ConfigLoaderFactory *clf = Registry::getRegistry()->getConfigLoaderFactory();
	if (clf==NULL)
	{
		return -4;
	}
	ConfigLoader *loader = clf->getConfigLoader(".xml");

	return 0;
}

	
int ComponentVerifier::decodePEM(Signature &sigdata, const char *buf, int buflen)
{

	unsigned char decode_buf[4096];
	unsigned char decode_out[4096];
	int decode_len=0;;

	int i=0;
		
	while (buf[i]<=' ') i++;

	do {

		if (strncmp(&buf[i],"-----BEGIN ",11)==0)
		{
			i+=11;
			break;
		}
		else
		{
			i++;
			if (i >= buflen)
			{
				//cout << "format error";
				return -2;
			}
		}
	}while(true);

	do {
		if (strncmp(&buf[i],"-----",5)==0)
		{
			i+=5;
			break;
		}
		else
		{
			i++;
			if (i >= buflen)
			{
				//cout << "format error 2";
				return -2;
			}
		}
	}while(true);

	while (buf[i]<=' ')
	{
		i++;
		if (i>=buflen) 
		{
			//cout <<"format error 3";
			return -3;
		}
	}

	do {
		if (strncmp(&buf[i],"-----END",8)==0)
		{
			break;
		}
		else
		{
			if (buf[i]!=' ')
			{
				decode_buf[decode_len]=buf[i];
				decode_len++;
			}
			else
			{
				decode_buf[decode_len]='\n';
				decode_len++;
			}
		}
		i++;
		if (i >= buflen)
		{
			//cout << "format error 3";
			return -2;
		}
	}while(true);

	EVP_ENCODE_CTX ctx;

	EVP_DecodeInit(&ctx);

	int bl = decode_len;

	int ii=EVP_DecodeUpdate(&ctx, decode_out, &bl, decode_buf, decode_len);

	if (ii <0)
	{
		ERR_print_errors_fp(stderr);
		return -2;
	}
	int k = 0;
	ii = EVP_DecodeFinal(&ctx, (unsigned char *)&(decode_out[bl]), &k);

	if (ii <0)
	{
		ERR_print_errors_fp(stderr);

		return -2;
	}

	bl+=k;

	memcpy(sigdata.sig_data,decode_out, bl);
	sigdata.sig_len = bl;

	return 0;
}

int ComponentVerifier::verifyIt(string &filename, Signature & sig)
{
	EVP_MD_CTX	md_ctx;

	unsigned char data[4096];
	int len;

	FILE *fp = fopen(filename.c_str(),"rb");

	if (fp==NULL)
	{
		cout<<"cannot find file "<<filename<<endl;
		return -1;
	}

	if (pkey==NULL)
	{
		return -2;
	}

	const EVP_MD *md = EVP_sha1();

	EVP_MD_CTX_init(&md_ctx);

	EVP_VerifyInit(&md_ctx,(EVP_MD *) md);

	do {
		len = fread(data, 1, 4096, fp);
		if (len <=0) break;
		EVP_VerifyUpdate(&md_ctx, data, len);
	}
	while(len >0);

	fclose(fp);

	
	int err = EVP_VerifyFinal(&md_ctx, sig.sig_data, sig.sig_len, pkey);


	if (err != 1)
	{
		//cout<<"component binary is damaged :"<<filename<<endl;
		return -3;
	}

	return 0;
}

int ComponentVerifier::verifyComponent(const string &path,Config *cfg)
{
	Config *d_cfg = cfg->getChild(COMPONENT_PROFILE);

	if (d_cfg==NULL)
	{
		// this file is not component profile
		return -1;
	}

	Config *cn_cfg = d_cfg->getChild(COMPONENT_NAME);

	// component Name

	if (cn_cfg==NULL)
	{
		// cannot find component name
		return -2;	
	}

	string name = cn_cfg->getValue();	

	cn_cfg = d_cfg->getChild(EXE_ENV);
	if (cn_cfg==NULL) {
		// cannot find execution_environment
		return -3;   
	}

	Config *libname_cfg = cn_cfg->getChild(LIBRARY_NAME);
	if (libname_cfg==NULL) {
		 // cannot find library_name
		return -4;  
	}

	string libname = libname_cfg->getValue();

	Config *verification_cfg = cn_cfg->getChild(BINARY_VERIFICATION);
	if (verification_cfg==NULL)
	{
		// cannot find verification information
		return 0;
	}

	string sigtext = verification_cfg->getValue();


	const char *decode_buf = sigtext.c_str();
	int decode_len = sigtext.length();

	Signature sig;

	int ret = decodePEM(sig, decode_buf, decode_len);

	if (ret!=0)
	{
		// signature is damaged

		return 1;
	}

	string libpath = path + libname;

	ret = verifyIt(libpath, sig);

	if (ret!=0)
	{
		return 2;
	}

	


	return 0;
	
}

#endif