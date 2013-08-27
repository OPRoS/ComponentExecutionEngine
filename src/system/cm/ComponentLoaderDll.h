/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 1
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentLoaderDll.h
 *
 */

#ifndef _COMPONENT_LOADER_DLL_H_
#define _COMPONENT_LOADER_DLL_H_

#include <string>
#include "system/profile/ComponentProfile.h"
#include "system/os/OSIncludes.h"
#include <Component.h>

#include "ComponentLoader.h"

class AtomicComponentPackage;

typedef Component *(*create_func_t)();
typedef void (*release_func_t)(Component *);

/**
	 DLL 혹은 SO 파일에서 컴포넌트를 로딩하는 클래스. 
 */
class ComponentLoaderDll : public ComponentLoader
{
	/** dll (so) handle */
	OSDllHandle m_dll;

	/** dll(so) 파일 이름 */
	std::string  m_dllFile;

	/** dll 파일 이름을 포함한 full path 이름 */
	std::string  m_dllPath;

	/** dll 내의 컴포넌트를 생성하는 함수 주소 */
	create_func_t m_createFunc;

	/** dll 내의 컴포넌트를 삭제하는 함수 주소 */
	release_func_t m_releaseFunc;

public:
	ComponentLoaderDll();
	virtual ~ComponentLoaderDll();

	virtual bool open(ComponentProfile *profile);
	virtual bool close();

	virtual Component *loadComponent();
	virtual bool unloadComponent(Component *component);

protected:
	/** dll(so) 라이브러리를 메모리로 로딩하는 함수. */
	bool loadLibrary();

	/** dll(so) 라이브러리를 메모리에서 해제하는 함수. */
	bool freeLibrary();

	/** 라이브러리가 로딩되었는지 여부를 반환하는 함수. */
	bool isLibraryLoaded() { return m_dll != INVALID_DLL; };

	/** 라이브러리에 컴포넌트 객체를 생성하는 함수가 존재하는지 여부를 반환하는 함수. */
	bool isExistsCreateFunction()	{ return m_createFunc != NULL; };

	/** 라이브러리에 컴포넌트 객체를 삭제하는 함수가 존재하는지 여부를 반환하는 함수. */
	bool isExistsReleaseFunction() { return m_releaseFunc != NULL; };

};

#endif 
