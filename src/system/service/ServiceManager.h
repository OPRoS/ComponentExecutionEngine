//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : OPRoS Component
//  @ File Name : ServiceManager.h
//  @ Date : 2008-08-22
//  @ Author :
//
//


#if !defined(_SERVICEMANAGER_H)
#define _SERVICEMANAGER_H

#include <string>

#include "system/Manager.h"
#include "Service.h"


class ServiceManagerProvider;
using namespace opros::engine;
/**
	Service ��ü�� �����ϴ� Ŭ����.
	���� �����Ǿ� ���� ����.
*/
class ServiceManager : public Manager {
	ServiceManagerProvider *m_provider;

public:
	ServiceManager();
	virtual ~ServiceManager();
	void setService(const std::string &name, Service *service);
	Service *getService(const std::string &serviceName);

	virtual bool onInit();
	virtual bool onStart();
	virtual bool onStop();
	virtual bool onDestroy();
};

#endif  