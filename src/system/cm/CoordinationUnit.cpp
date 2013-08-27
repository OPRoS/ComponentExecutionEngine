/*
 *  OPRoS Component Engine (OCE)
 *  Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 *  Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2009. 9. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : CoordinationUnit.cpp
 *
 *
 */

#include "CoordinationUnit.h"

CoordinationUnit::CoordinationUnit()
{
	enabled = false;

	m_parent = NULL;
}

CoordinationUnit::~CoordinationUnit()
{
	m_trigger_list.clear();
	m_follower_list.clear();
	m_subjoin_list.clear();
}

bool CoordinationUnit::isEnabled()
{
	return enabled;
}

bool CoordinationUnit::isTriggered()
{
	if (m_parent != NULL)
	{
		if (!m_parent->isTriggered()) // 상위 컴포넌트(컴포짓 컴포넌트)가 trigger되지 않았다면 하위 컴포넌트는 실행 되지 않는다.`
		{
			return false;
		}
	}

	if (!isEnabled()) return true; // Trigger가 사용되지 않는 다면 항상 on 임

	if (m_trigger_list.empty()) return true;  // 트리거가 없으면 항상 on 임

	CoordinationTriggerListType::iterator t_it = m_trigger_list.begin();

	for (;t_it != m_trigger_list.end(); ++t_it)
	{
		if ((*t_it).second==0) return false;
	}

	return true;
}


bool CoordinationUnit::isAllSubCompleted()
{
	if (!isEnabled()) return true;

	if (m_subjoin_list.empty()) return true;

	CoordinationSubListType::iterator t_it = m_subjoin_list.begin();

	for (;t_it != m_subjoin_list.end(); ++t_it)
	{
		if ((*t_it).second==0) return false;
	}

	return true;
}


void CoordinationUnit::setEnable()
{
	enabled = true;

	if (!m_subjoin_list.empty())
	{
		// all sub set to complte

		CoordinationSubListType::iterator t_it = m_subjoin_list.begin();

		for (;t_it != m_subjoin_list.end(); ++t_it)
		{
			(*t_it).first->setEnable();
		}
	}
}

void CoordinationUnit::setDisable()
{
	enabled = false;
}

void CoordinationUnit::setParent(CoordinationUnit *parent)
{
	m_parent = parent;

	if (m_parent != NULL)
	{
		m_parent->addSub(this);
	}
}

CoordinationUnit *CoordinationUnit::getParent()
{
	return m_parent;
}


// Composite인 경우에만 addSub함.
// sub가 중요한 것은 컴포짓의 경우 complete 조건이 내부 컴포넌트의 complete 이기 때문이다.
void CoordinationUnit::addSub(CoordinationUnit *ca)
{
	CoordinationSubListType::iterator t_it;

	t_it = m_subjoin_list.find(ca);

	if (t_it== m_subjoin_list.end())
	{
		m_subjoin_list[ca] = 0;
	}
}

void CoordinationUnit::addTrigger(CoordinationUnit *ca)
{
	CoordinationTriggerListType::iterator t_it;

	t_it = m_trigger_list.find(ca);

	if (t_it== m_trigger_list.end())
	{
		m_trigger_list[ca] = 0;
		ca->addFollower(this);
	}
	setEnable();
}

void CoordinationUnit::addFollower(CoordinationUnit *ca)
{
	CoordinationFollowListType::iterator t_it;

	t_it = m_follower_list.find(ca);

	if (t_it== m_follower_list.end())
	{
		m_follower_list[ca] = 0;
		ca->addTrigger(this);
	}
	setEnable();
}

void CoordinationUnit::removeSub(CoordinationUnit *ca)
{
	CoordinationSubListType::iterator t_it;

	t_it = m_subjoin_list.find(ca);


	if (t_it!= m_subjoin_list.end())
	{
		m_subjoin_list.erase(ca);
		ca->setParent(NULL);
	}
}

void CoordinationUnit::removeTrigger(CoordinationUnit *ca)
{
	CoordinationTriggerListType::iterator t_it;

	t_it = m_trigger_list.find(ca);


	if (t_it!= m_trigger_list.end())
	{
		m_trigger_list.erase(ca);
		ca->removeFollower(this);
	}
}

void CoordinationUnit::removeFollower(CoordinationUnit *ca)
{
	CoordinationFollowListType::iterator t_it;

	t_it = m_follower_list.find(ca);

	if (t_it!= m_follower_list.end())
	{
		m_follower_list.erase(ca);
		ca->removeTrigger(this);
	}
}

void CoordinationUnit::removeAllSub()
{
	CoordinationSubListType::iterator t_it;

	t_it = m_subjoin_list.begin();

	for (;t_it != m_subjoin_list.end(); ++t_it)
	{
		CoordinationUnit *t_ca = (*t_it).first;

		m_subjoin_list.erase(t_it);

		t_ca->setParent(NULL);
	}

	m_subjoin_list.clear();
}

void CoordinationUnit::removeAllTrigger()
{
	CoordinationTriggerListType::iterator t_it;

	t_it = m_trigger_list.begin();

	for (;t_it != m_trigger_list.end(); ++t_it)
	{
		CoordinationUnit *t_ca = (*t_it).first;

		m_trigger_list.erase(t_it);

		t_ca->removeFollower(this);

	}

	m_trigger_list.clear();
}

void CoordinationUnit::removeAllFollower()
{
	CoordinationFollowListType::iterator t_it;

	t_it = m_follower_list.begin();

	for (;t_it != m_follower_list.end(); ++t_it)
	{
		CoordinationUnit *t_ca = (*t_it).first;

		m_follower_list.erase(t_it);

		t_ca->removeTrigger(this);

	}

	m_follower_list.clear();
}

int CoordinationUnit::getTrigger(CoordinationUnit *ca)
{
	CoordinationTriggerListType::iterator t_ti;

	t_ti = m_trigger_list.find(ca);

	if (t_ti != m_trigger_list.end())
	{
		return (*t_ti).second;
	}

	return -1;
}


void CoordinationUnit::setTrigger(CoordinationUnit *ca)
{
	CoordinationTriggerListType::iterator t_ti;

	t_ti = m_trigger_list.find(ca);

	int v;

	if (t_ti != m_trigger_list.end())
	{
		v = (*t_ti).second;
		if (v<0) v= 0;
		(*t_ti).second = v + 1;
	}
}

void CoordinationUnit::clearTrigger(CoordinationUnit *ca)
{
	CoordinationTriggerListType::iterator t_ti;

	t_ti = m_trigger_list.find(ca);

	if (t_ti != m_trigger_list.end())
	{
		(*t_ti).second = 0;
	}
}

void CoordinationUnit::clearAllTrigger()
{
	CoordinationTriggerListType::iterator t_ti;

	for (t_ti = m_trigger_list.begin(); t_ti != m_trigger_list.end(); t_ti++)
	{
		if ((*t_ti).second > 0)
		{
			(*t_ti).second = 0;
		}
	}
}

void CoordinationUnit::clearAllSub()
{
	CoordinationSubListType::iterator t_ti;

	for (t_ti = m_subjoin_list.begin(); t_ti != m_subjoin_list.end(); t_ti++)
	{
		if ((*t_ti).second > 0)
		{
			(*t_ti).second = 0;
		}
	}
}

void CoordinationUnit::setSubComplete(CoordinationUnit *ca)
{
	CoordinationSubListType::iterator t_ti;

	t_ti = m_subjoin_list.find(ca);

	int v;

	if (t_ti != m_subjoin_list.end())
	{
		v = (*t_ti).second;
		if (v<0) v= 0;
		(*t_ti).second = v + 1;
	}

	if (isAllSubCompleted())
	{
		setAllFollower();	// follower들을 ON 시킴
		clearAllTrigger();	// 다시 선수 작업들 기다릴 수 있도록 Trigger를 OFF 시킴
	}
}

void CoordinationUnit::setAllFollower()
{
	CoordinationFollowListType::iterator t_it;

	t_it = m_follower_list.begin();

	for (;t_it != m_follower_list.end(); ++t_it)
	{
		CoordinationUnit *t_ca = (*t_it).first;

		t_ca->setTrigger(this);
	}

	if (m_parent != NULL)
	{
		m_parent->setSubComplete(this);
	}
}


CoordinationFollowListType::iterator CoordinationUnit::getFollowerEnd()
{
	return m_follower_list.end();
}

CoordinationTriggerListType::iterator CoordinationUnit::getTriggerEnd()
{
	return m_trigger_list.end();
}

CoordinationFollowListType::iterator CoordinationUnit::getFollowerFirst()
{
	return m_follower_list.begin();
}

CoordinationTriggerListType::iterator CoordinationUnit::getTriggerFirst()
{
	return m_trigger_list.begin();
}