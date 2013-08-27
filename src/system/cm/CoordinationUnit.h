/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 9. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CoordinationUnit.h
 *
 *
 */

#ifndef COORDINATIONUNIT_H_
#define COORDINATIONUNIT_H_

#include <list>
#include <map>
#include <string>


/**
 * @author sby
 * @since  2009. 9. 11
 * @version 1.0
 *
 */

class CoordinationUnit;

typedef std::map<CoordinationUnit *, int> CoordinationFollowListType;
typedef std::map<CoordinationUnit *, int> CoordinationTriggerListType;
typedef std::map<CoordinationUnit *, int> CoordinationSubListType;

class CoordinationUnit
{
	std::map<CoordinationUnit *, int> m_follower_list;
	std::map<CoordinationUnit *, int> m_trigger_list;
	std::map<CoordinationUnit *, int> m_subjoin_list;  // composite의 경우 sub가 모두 complete 되어야만 composite이 complete됨.

	bool enabled;

	CoordinationUnit *m_parent;

public:
	CoordinationUnit();
	virtual ~CoordinationUnit();

	virtual bool isEnabled();
	virtual void setEnable();
	virtual void setDisable();

	virtual void setParent(CoordinationUnit *parent);
	virtual CoordinationUnit *getParent();

	virtual void addTrigger(CoordinationUnit *ca);
	virtual void removeTrigger(CoordinationUnit *ca);
	virtual void removeAllTrigger();

	virtual void addSub(CoordinationUnit *ca);
	virtual void removeSub(CoordinationUnit *ca);
	virtual void removeAllSub();

	virtual void addFollower(CoordinationUnit *ca);
	virtual void removeFollower(CoordinationUnit *ca);
	virtual void removeAllFollower();

	virtual int getTrigger(CoordinationUnit *ca);
	virtual void setTrigger(CoordinationUnit *ca);
	virtual void clearTrigger(CoordinationUnit *ca);
	virtual void clearAllTrigger();

	virtual bool isAllSubCompleted();
	virtual void setSubComplete(CoordinationUnit *ca);
	virtual void clearAllSub();

	virtual void setAllFollower();
	virtual bool isTriggered();

	virtual CoordinationFollowListType::iterator getFollowerEnd();
	virtual CoordinationTriggerListType::iterator getTriggerEnd();
	virtual CoordinationFollowListType::iterator getFollowerFirst();
	virtual CoordinationTriggerListType::iterator getTriggerFirst();


};

#endif /* COORDINATIONUNIT_H_ */
