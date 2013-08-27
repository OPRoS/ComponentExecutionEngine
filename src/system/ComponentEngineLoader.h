/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ComponentEngineLoader.h
 *
 */

#ifndef _COMPONENT_ENGINE_LOADER_H_
#define _COMPONENT_ENGINE_LOADER_H_

#include "system/util/Properties.h"

/**
	������Ʈ ���࿣�� ��ü�� �����ϰ� ������ ���۽�Ű�� Ŭ����. 
 */
class ComponentEngineLoader
{
	/** ����Ʈ ���� �̸� */
	std::string m_appName;

public:
	ComponentEngineLoader();
	~ComponentEngineLoader();

	/** 
		������Ʈ ���� ������ �ε��ϴ� �Լ� 

		@param argc Ŀ�ǵ� ���� �ƱԸ�Ʈ ����
		@param argv Ŀ�ǵ� ���� �ƱԸ�Ʈ

		@return bool ���� ����
	*/
	bool load(int argc, char *argv[]);

	/** 
		������Ʈ ���� ������ �ε��ϴ� �Լ� 

		@param props Ŀ�ǵ� ���� �ƱԸ�Ʈ�� name/value ���·� ������ ��ü

		@return bool ���� ����
	*/
	bool load(Properties &props);

	/** 
		Ŀ�ǵ� ���� �ƱԸ�Ʈ�� name/value ������ Properties ��ü�� �����ϴ� �Լ�.

		@param props  Ŀ�ǵ� ���� �ƱԸ�Ʈ�� name/value�� �����ϱ� ���� Properties ��ü
		@param argc Ŀ�ǵ� ���� �ƱԸ�Ʈ ����
		@param argv Ŀ�ǵ� ���� �ƱԸ�Ʈ
	*/
	void toProperties(Properties &props, int argc, char *argv[]);
};

#endif  
