/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 1
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentLoaderDll.cpp
 *
 */

#include "ComponentLoaderDll.h"

#include "system/profile/ComponentProfile.h"

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

ComponentLoaderDll::ComponentLoaderDll()
{
	m_dll = INVALID_DLL;
	m_createFunc = NULL;
	m_releaseFunc = NULL;
}

ComponentLoaderDll::~ComponentLoaderDll()
{
	trace_enter();
	if (m_dll != INVALID_DLL) OSDll::freeLibrary(m_dll);	   	
}

//-------------------------------------------------------
// ������Ʈ �δ� ��ü open �Լ�
//
bool ComponentLoaderDll::open(ComponentProfile *profile)
{
	trace_enter();

	if (profile == NULL) {
		log_error("ComponentProfile is NULL");
		return false;
	}

	// �������� ������ �̿��Ͽ� DLL ������ ���丮 ����
	m_dllFile = profile->execEnv.libraryName;
	m_dllPath = profile->getPath() + m_dllFile;

	trace("dllPath= " << m_dllPath);

	// DLL �ε�
	return loadLibrary();
}

//-------------------------------------------------------
// ������Ʈ �δ� ��ü close �Լ�
//
bool ComponentLoaderDll::close()
{
	trace_enter();

	// ���̺귯�� ����
	return freeLibrary();
}

//-------------------------------------------------------
// DLL/so ���̺귯������ ������Ʈ�� �����ϴ� �Լ�
//
Component *ComponentLoaderDll::loadComponent()
{
	trace_enter_msg("dllPath= " + m_dllPath);

	// ���̺귯���� �ε���� �ʾ����� NULL ����
	if (!isLibraryLoaded()) {
		log_error("Library is not loaded. Call Open()");
		return NULL;
	}

	// ���̺귯���� ������Ʈ ���� �Լ�(getComponent())�� ȣ���Ͽ�
	// ������Ʈ�ν��Ͻ� ����
	Component *component = NULL;
	if (isExistsCreateFunction()) {
		component = (*m_createFunc)();
	}

	return component;
}

//-------------------------------------------------------
// DLL/so ���̺귯������ ������ ������Ʈ�� �����ϴ� �Լ�
//
bool ComponentLoaderDll::unloadComponent(Component *component)
{
	trace_enter();

	if (component == NULL) {
		log_error("component is NULL");
		return true;
	}

	// DLL/so ������ �ε�Ǿ����� �˻�
	if (!isLibraryLoaded()) {
		log_error("Library is not loaded. Call Open().");
		return false;
	}

	// ������Ʈ�� �����ϴ� �Լ��� �����ϴ� �� �˻�
	// DLL ���Ͽ��� ������ ��ü�� �ش� DLL���� �����ؾ� ��
	if (isExistsReleaseFunction()) {
		m_releaseFunc(component);
	}
	else {
		log_error("Threre is no release function. So delete directly????????????");
		delete component;
	}

	component = NULL;

	return true;
}

//----------------------------------
// DLL/so ���̺귯�� �ε� �Լ�
//
bool ComponentLoaderDll::loadLibrary()
{
	trace_enter_msg("dllPath= " + m_dllPath);

	// �̹� �ε� �Ǿ� ������ �ش� ���̺귯���� ����
	if (isLibraryLoaded()) {
		freeLibrary();
	}

	// ���̺귯�� �ε�
	m_dll = OSDll::loadLibrary(m_dllPath);
	if (m_dll == INVALID_DLL) {
		log_error("Invalid dll path : dllpath=" << m_dllPath <<":"<< OS::getLastError());
		return false;
	}

	// ���̺귯������ ������Ʈ �ν��Ͻ��� �����ϴ� �Լ��� ���� �����͸� ����
	m_createFunc = (create_func_t) OSDll::getProcAddress(m_dll, "getComponent");
	if (m_createFunc == NULL) {
		log_error("Cannot create Factory Function : dllpath=" << m_dllPath);
		OSDll::freeLibrary(m_dll);
		return false;
	}

	// ���̺귯������ ������Ʈ �ν��Ͻ��� �����ϴ� �Լ��� ���� �����͸� ����
	// DLL���� ������ ��ü�� �ش� DLL���� �����ؾ� ��
	m_releaseFunc = (release_func_t) OSDll::getProcAddress(m_dll, "releaseComponent");
	if (m_releaseFunc == NULL) {
		log_error("Cannot create Factory Function : dllpath=" << m_dllPath);
		OSDll::freeLibrary(m_dll);
		return false;
	}

	return true;
}


//----------------------------------
// DLL/so ���̺귯�� ��ε� �Լ�
//
bool ComponentLoaderDll::freeLibrary()
{
	trace_enter_msg("dllPath= " + m_dllPath);

	// ���̺귯���� �ε� �Ǿ� ���� ���� ����
	if (isLibraryLoaded()) {
		OSDll::freeLibrary(m_dll);

		m_dll = INVALID_DLL;
		m_createFunc = NULL;
		m_releaseFunc = NULL;
	}

	return true;
}