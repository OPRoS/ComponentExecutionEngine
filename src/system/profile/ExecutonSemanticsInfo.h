/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ExecutonSemanticsInfo.h
 *
 */

#ifndef _EXECUTON_SEMANTICS_INFO_H_
#define _EXECUTON_SEMANTICS_INFO_H_

#include <string>

#include "Profile.h"

/** 
	컴포넌트 실행 시멘틱스 정보를 나타내는 클래스.
*/
class ExecutonSemanticsInfo : public ProfileInfo
{
public :
	/** 컴포넌트 실행 타입 (periodic, aperiodic, passive). */
	std::string  type;

	/** 컴포넌트 실행 주기. */
	int period;

	/** 컴포넌트 실행 우선 순위 */
	int priority;
	/** 컴포넌트 스케쥴링 정책 : SCHED_FIFO, SCHED_RR, SCHED_OTHER(default)*/	
	std::string policy;

	/** 컴포넌트 생성하는 방식. 현재 사용 안함. */
	std::string  creationType;

	/** 
		컴포넌트 실행 타입으로 컴포넌트 배포 시 
		컴포넌트의 실행 여부를 나타냄. 
		init : 컴포넌트의 initialize()까지만 호출
		start : 컴포넌트의 initialize(), start() 까지 호출
	*/
	std::string  lifecycleType;

public:
	ExecutonSemanticsInfo();
	virtual ~ExecutonSemanticsInfo();

	virtual void parse(Config *cfg);
	virtual void print(std::ostream &os, int level = 0);
};

#endif /* _EXECUTON_SEMANTICS_INFO_H_ */
