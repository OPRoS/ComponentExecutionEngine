/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 19
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : PackageManager.h
 *
 */

#ifndef _PACKAGE_MANAGER_H_
#define _PACKAGE_MANAGER_H_

#include <string>
#include "system/SystemTypes.h"

#include "Package.h"

typedef std::list<Package *> PackageList;

/** 
	배포된 패키지를 관리하는 클래스.
*/
class PackageManager
{
protected :
	/** 패키지를 저장하는 벡터 */
	PackageList m_pkgList;

	/**
		패키지를 타입에 따라 생성하는 함수.

		@param path 프로파일이 있는 디렉토리 이름
		@param reference 프로파일 이름

		@return Package * 생성된 패키지 객체
	*/
	Package *createPackage(const std::string &path, const std::string &reference);

public:
	PackageManager();
	virtual ~PackageManager();

	/**
		패키지를 타입에 따라 로드하는 함수.

		@param path 프로파일이 있는 디렉토리 이름
		@param reference 프로파일 이름

		@return Package * 로드한 패키지 객체
	*/
	Package *loadPackage(const std::string  &path, const std::string  &reference);

	/**
		패키지를 언로드하는 함수.

		@param pkgName 패키지 이름

		@return bool 성공여부
	*/
	bool unloadPackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들 간의 포트를 연결하는 함수.

		@param pkgName 포트 연결할 패키지 이름

		@return bool 성공여부
	*/
	bool connectPackage(const std::string  &pkgName);


	/**
		패키지 내의 컴포넌트들 간의 포트를 연결을 해제하는 함수.

		@param pkgName 포트 연결을 해제할 패키지 이름

		@return bool 성공여부
	*/
	bool disconnectPackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들의 실행을 시작하는 함수.

		@param pkgName 패키지 이름

		@return bool 성공여부
	*/
	bool activatePackage(const std::string  &pkgName);

	/**
		패키지 내의 컴포넌트들의 실행을 중지하는 함수.

		@param pkgName 패키지 이름

		@return bool 성공여부
	*/
	bool passivatePackage(const std::string  &pkgName);

	/**
		패키지를 검색하는 함수.

		@param pkgName 패키지 이름

		@return Package * 검색한 패키지. 없음면 NULL 반환.
	*/
	Package *getPackage(const std::string  &pkgName);

	/**
		등록된 모든 패키지의 이름을 반환하는 함수.

		@return StringList 패키지 이름 리스트
	*/
	StringList getPackageNames();

	/**
		등록된 모든 패키지 리스트를 반환하는 함수.

		@return PackageList & 패키지 리스트
	*/
	PackageList &getPackageList();

	/**
		패키지를 리스트에 추가하는 함수.

		@param pkg 추가할 패키지 객체

		@return bool 성공여부
	*/
	bool addPackage(Package *pkg);

	/**
		패키지를 리스트에서 삭제하는 함수.

		@param pkgName 삭제할 패키지 이름
	*/
	void removePackage(const std::string  &pkgName);

	/**
		모든 패키지를 리스트에서 삭제하는 함수.
	*/
	void removePackageAll();
};

#endif  

