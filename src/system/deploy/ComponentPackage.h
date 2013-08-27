/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 31
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentPackage.h
 *
 */

#ifndef _COMPONENT_PACKAGE_H_
#define _COMPONENT_PACKAGE_H_

#include <string>
#include "Package.h"
#include "system/profile/ComponentProfile.h"
#include "system/cm/ComponentAdapter.h"

/**
	배포의 대상이 component profile(atomic, composite)일 경우
	profile을 로드/언로드, 컴포넌트 간의 포트 연결/해제,
	컴포넌트의 실행 시작/중지를 수행하는 클래스
*/ 

class ComponentPackage : public Package
{
	/** 해당 컴포넌트가 존재하는 노드의 아이디 */
	std::string m_nodeId;

	/** ComponentPackage의 Parent ID */
	std::string m_parent;

	ComponentAdapter *m_ca;


protected:
	/** 패키지 로딩 함수 */
	virtual bool doLoad();

	/** 패키지 언로딩 함수 */
	virtual bool doUnload();

	/** 패키지 내에 포함된 컴포넌트 간의 포트 연결 함수 */
	virtual bool doConnectPort();

	/** 패키지 내에 포함된 컴포넌트 간의 포트 연결 해제 함수 */
	virtual bool doDisconnectPort();

	/** 패키지 내에 포함된 컴포넌트 들을 실행시키는 함수 */
	virtual bool doActivate();

	/** 패키지 내에 포함된 컴포넌트 들의 실행을 중지시키는 함수 */
	virtual bool doPassivate();

public:
	ComponentPackage();
	virtual ~ComponentPackage();

	std::list<std::string> beforeComponentList;
	std::list<std::string> afterComponentList;

	void setParentId(const std::string &parent);
	std::string &getParentId();

	void setNodeId(const std::string &nodeId) { m_nodeId = nodeId; };
	std::string getNodeId() { return m_nodeId; };

	ComponentAdapter *getComponentAdapter();
};

#endif
