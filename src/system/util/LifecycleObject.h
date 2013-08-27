/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 8
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : LifecycleObject.h
 *
 */

#ifndef _LIFECYCLE_OBJECT_H_
#define _LIFECYCLE_OBJECT_H_

#include <string>
#include <map>
#include <iostream>

using namespace std;
/**

*/
template <typename StateKey, typename Event>
class LifecycleObject
{
public :

	typedef  StateKey StateKey_t;
	typedef  Event Event_t;
	typedef void (*Action_t)(const Event_t &);
	typedef bool (*Guard_t)(const Event_t &);

	typedef struct Transition {
		Event_t		fireEvent;
		StateKey_t	nextState;
		Guard_t		guardFn;

		Transition() : guardFn(NULL) {};

		bool doGuard(const Event_t &evt) {
			if (guardFn != NULL) return guardFn(evt);
			else return true;
		};

	} Transition_t;

	typedef  std::map<Event_t , Transition_t *> TransitionMap;

	typedef struct State {
		StateKey_t	key;
		Action_t	entryFn;
		Action_t	stayFn;
		Action_t	exitFn;
		TransitionMap transitions;

		State() : entryFn(NULL), stayFn(NULL), exitFn(NULL) {};
		~State() {
			clearTransitions();
		};

		void doEntry(const Event_t &evt) {
			if (entryFn != NULL) entryFn(evt);
		};

		void doStay(const Event_t &evt) {
			if (stayFn != NULL) stayFn(evt);
		};

		void doExit(const Event_t &evt) {
			if (exitFn != NULL) exitFn(evt);
		};

		void addTransition(const Event_t& fireEvent, const StateKey_t& nextState, Guard_t guardFn) {
			removeTransition(fireEvent);

			Transition_t *t = new Transition_t();
			t->fireEvent = fireEvent;
			t->nextState = nextState;
			t->guardFn = guardFn;

			transitions.insert(make_pair(fireEvent, t));
		};

		Transition_t* findTransition(const Event_t& fireEvent) {
			typename TransitionMap::iterator pos= transitions.find(fireEvent);
			if (pos != transitions.end()) {
				return pos->second;
			}
			else {
				return NULL;
			}
		};

		void removeTransition(const Event_t& fireEvent) {
			typename TransitionMap::iterator pos = transitions.find(fireEvent);
			if (pos != transitions.end()) {
				delete pos->second;
				transitions.erase(pos);
			}
		};

		void clearTransitions() {
			typename TransitionMap::iterator pos = transitions.begin();
			for (; pos != transitions.end(); ++pos) {
				delete pos->second;
			}
			transitions.clear();
		};
	} State_t;

protected :

	/** */
	StateKey_t m_initialState;

	/**  */
	StateKey_t m_currentState;

	typedef std::map<StateKey_t, State_t *> StateMap;
	StateMap m_states;

public:
	LifecycleObject() {};
	virtual ~LifecycleObject() { clearStates(); };


	/**
		
	*/
	StateKey_t &getInitialState() { return m_initialState; };

	/**

	*/
	void getInitialState(const StateKey_t &initialState) {
		m_initialState = initialState;
		setCurrentState(m_initialState);
	};

	/**
	
	*/
	StateKey_t &getCurrentState() { return m_currentState; };

	/**
	
	*/
	void setCurrentState(const StateKey_t &currentState) { m_currentState = currentState; };

	/**

	*/
	void addState(const StateKey_t &key, Action_t entryFn = NULL, Action_t stayFn = NULL, Action_t exitFn = NULL) {
		removeState(key);

		State_t *s = new State_t();
		s->key = key;
		s->stayFn = stayFn;
		s->entryFn = entryFn;
		s->exitFn = exitFn;

		m_states.insert(make_pair(key, s));
	};

	/**

	*/
	void removeState(const StateKey_t &key) {
		typename StateMap::iterator pos = m_states.find(key);
		if (pos != m_states.end()) {
			delete pos->second;
			m_states.erase(pos);
		}
	};

	/**

	*/
	void clearStates() {
		typename StateMap::iterator pos = m_states.begin();
		for (; pos != m_states.end(); ++pos) {
			delete pos->second;
		}
		m_states.clear();
	};

	/**

	*/
	void addTransition(const StateKey_t& fromSate, const Event_t& fireEvent, const StateKey_t& nextState, Guard_t guardFn = NULL) {
		typename StateMap::iterator pos = m_states.find(fromSate);
		if (pos != m_states.end()) {
			pos->second->addTransition(fireEvent, nextState, guardFn);
		}
	};

	/**

	*/
	void removeTransition(const StateKey_t &fromSate, const Event_t &fireEvent)	{
		typename StateMap::iterator pos = m_states.find(fromSate);
		if (pos != m_states.end()) {
			pos->second->removeTransition(fireEvent);
		}
	};

	/**

	*/
	void clearTransitions(const StateKey_t &fromSate) {
		typename StateMap::iterator pos = m_states.find(fromSate);
		if (pos != m_states.end()) {
			pos->second->clearTransitions();
		}
	};

	/**
	
	*/
	void prcessEvent(const Event_t &evt) {
		typename StateMap::iterator pos = m_states.find(m_currentState);
		if (pos == m_states.end()) {
			return;
		}

		State_t *curState = pos->second;
		Transition_t *tx = curState->findTransition(evt);
		if (tx == NULL) {
			return;
		}

		State_t *nextState = NULL;
		pos = m_states.find(tx->nextState);
		if (pos != m_states.end()) {
			nextState = pos->second;
		}

		if (nextState == NULL || tx->doGuard(evt) == false) {
			curState->doStay(evt);
			return;
		}
		curState->doExit(evt);

		nextState->doEntry(evt);
		nextState->doStay(evt);

		setCurrentState(nextState->key);
	};

	void  test() {
		LifecycleObject<string, int> lco;

		lco.addState("State1");
		lco.addState("State2");
		lco.addTransition("State1", 1, "State2");
		lco.addTransition("State2", 2, "State1");
		lco.getInitialState("State1");

		cout << "Initial State : " << lco.getInitialState() << endl;
		cout << "Current State : " << lco.getCurrentState() << endl;

		lco.prcessEvent(1);
		cout << "Current State : " << lco.getCurrentState() << endl;

		lco.prcessEvent(2);
		cout << "Current State : " << lco.getCurrentState() << endl;
	};
};


#endif
