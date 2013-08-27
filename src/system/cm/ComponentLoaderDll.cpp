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
// 컴포넌트 로더 객체 open 함수
//
bool ComponentLoaderDll::open(ComponentProfile *profile)
{
	trace_enter();

	if (profile == NULL) {
		log_error("ComponentProfile is NULL");
		return false;
	}

	// 프로파일 정보를 이용하여 DLL 파일의 디렉토리 설정
	m_dllFile = profile->execEnv.libraryName;
	m_dllPath = profile->getPath() + m_dllFile;

	trace("dllPath= " << m_dllPath);

	// DLL 로드
	return loadLibrary();
}

//-------------------------------------------------------
// 컴포넌트 로더 객체 close 함수
//
bool ComponentLoaderDll::close()
{
	trace_enter();

	// 라이브러리 해제
	return freeLibrary();
}

//-------------------------------------------------------
// DLL/so 라이브러리에서 컴포넌트를 생성하는 함수
//
Component *ComponentLoaderDll::loadComponent()
{
	trace_enter_msg("dllPath= " + m_dllPath);

	// 라이브러리가 로드되지 않았으면 NULL 리턴
	if (!isLibraryLoaded()) {
		log_error("Library is not loaded. Call Open()");
		return NULL;
	}

	// 라이브러리의 컴포넌트 생성 함수(getComponent())를 호출하여
	// 컴포넌트인스턴스 생성
	Component *component = NULL;
	if (isExistsCreateFunction()) {
		component = (*m_createFunc)();
	}

	return component;
}

//-------------------------------------------------------
// DLL/so 라이브러리에서 생성된 컴포넌트를 삭제하는 함수
//
bool ComponentLoaderDll::unloadComponent(Component *component)
{
	trace_enter();

	if (component == NULL) {
		log_error("component is NULL");
		return true;
	}

	// DLL/so 파일이 로드되었는지 검사
	if (!isLibraryLoaded()) {
		log_error("Library is not loaded. Call Open().");
		return false;
	}

	// 컴포넌트를 삭제하는 함수가 존재하는 지 검사
	// DLL 파일에서 생성된 객체는 해당 DLL에서 삭제해야 함
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
// DLL/so 라이브러리 로드 함수
//
bool ComponentLoaderDll::loadLibrary()
{
	trace_enter_msg("dllPath= " + m_dllPath);

	// 이미 로드 되어 있으면 해당 라이브러리를 삭제
	if (isLibraryLoaded()) {
		freeLibrary();
	}

	// 라이브러리 로드
	m_dll = OSDll::loadLibrary(m_dllPath);
	if (m_dll == INVALID_DLL) {
		log_error("Invalid dll path : dllpath=" << m_dllPath <<":"<< OS::getLastError());
		return false;
	}

	// 라이브러리에서 컴포넌트 인스턴스를 생성하는 함수에 대한 포인터를 얻어옴
	m_createFunc = (create_func_t) OSDll::getProcAddress(m_dll, "getComponent");
	if (m_createFunc == NULL) {
		log_error("Cannot create Factory Function : dllpath=" << m_dllPath);
		OSDll::freeLibrary(m_dll);
		return false;
	}

	// 라이브러리에서 컴포넌트 인스턴스를 삭제하는 함수에 대한 포인터를 얻어옴
	// DLL에서 생성된 객체는 해당 DLL에서 삭제해야 함
	m_releaseFunc = (release_func_t) OSDll::getProcAddress(m_dll, "releaseComponent");
	if (m_releaseFunc == NULL) {
		log_error("Cannot create Factory Function : dllpath=" << m_dllPath);
		OSDll::freeLibrary(m_dll);
		return false;
	}

	return true;
}


//----------------------------------
// DLL/so 라이브러리 언로드 함수
//
bool ComponentLoaderDll::freeLibrary()
{
	trace_enter_msg("dllPath= " + m_dllPath);

	// 라이브러리가 로드 되어 있을 때만 삭제
	if (isLibraryLoaded()) {
		OSDll::freeLibrary(m_dll);

		m_dll = INVALID_DLL;
		m_createFunc = NULL;
		m_releaseFunc = NULL;
	}

	return true;
}
