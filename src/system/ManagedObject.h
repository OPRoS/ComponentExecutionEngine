/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 8
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : ManagedObject.h
 *
 */

#ifndef _MANAGED_OBJECT_H_
#define _MANAGED_OBJECT_H_

#include <string>
#include <list>

#include "system/config/Configurable.h"
#include "system/util/Properties.h"
#include "system/os/OSRunnable.h"

class ManagedObjectListener;
typedef std::list<ManagedObjectListener *> ManagedObjectListenerList;

typedef enum { MS_CREATED, MS_INITED, MS_STARTED, MS_STOPPED, MS_DESTROYED } ManagedObjectState;

/**
	init, start, stop, reset, destroy 라이프 싸이클을 가진 
	객체의 상태를 관리하기 위한 클래스
*/
class ManagedObject : public OSRunnable, public Configurable
{
protected :
	/** 라이프 싸이클이 변화되었을 때 콜백하기 위한 리스너 리스트 */
	ManagedObjectListenerList m_listeners;

	/** ManagedObject의 상태 */
	ManagedObjectState m_state;

	/** ManagedObject의 프로퍼티 */
	Properties m_props;

	/** 
		ManagedObject의 상태가 바뀌었을 때 리스너에게
		상태 변화를 알리기 위한 함수

		@param state 바뀐 상태
	*/
	void stateChanged(ManagedObjectState state);

	/** 
		Config 객체를 프로퍼티로 변환하는 함수

		@param cfg 프로퍼티로 변환하고자 하는 Config 객체
	*/
	void toProperties(Config *cfg);

public:
	ManagedObject();
	virtual ~ManagedObject();

	/**
		ManagedObject의 프로퍼티를 반환하는 함수.
	*/
	Properties *getProperties() { return &m_props; };

	/**
		초기화 함수. 
		객체의 현재 상태를 검사하여 초기화가 가능한 경우만 onInit() 함수를 호출함.

		@return bool 성공 여부
	*/
	virtual bool init();

	/**
		실행 시작 함수. 
		객체의 현재 상태를 검사하여 실행 시작이 가능한 경우만 onStart() 함수를 호출함.

		@return bool 성공 여부
	*/
	virtual bool start();

	/**
		실행 중지 함수. 
		객체의 현재 상태를 검사하여 실행 중지가 가능한 경우만 onStop() 함수를 호출함.

		@return bool 성공 여부
	*/
	virtual bool stop();

	/**
		객체의 상태를 초기 상태(CREATED)로 바꾸는 함수. 
		객체의 현재 상태를 검사하여 reset이 가능한 경우만 onReset() 함수를 호출함.

		@return bool 성공 여부
	*/
	virtual bool reset();

	/**
		객체를 메모리에서 삭제하기 전에 호출하는 함수.
		객체의 현재 상태를 검사하여 destroy가 가능한 경우만 onDestroy() 함수를 호출함.

		@return bool 성공 여부
	*/
	virtual bool destroy();

	/**
		실제 초기화를 수행하는 함수로 ManagedObject를 상속한 객체가 구현해야하는 함수. 
		init() 함수에서 호출되는 callback 함수.

		@return bool 성공 여부
	*/
	virtual bool onInit();

	/**
		실제 객체의 실행 시작을 수행하는 함수로 ManagedObject를 상속한 객체가 구현해야하는 함수. 
		start() 함수에서 호출되는 callback 함수.

		@return bool 성공 여부
	*/
	virtual bool onStart();

	/**
		실제 객체의 실행 중지을 수행하는 함수로 ManagedObject를 상속한 객체가 구현해야하는 함수. 
		stop() 함수에서 호출되는 callback 함수.

		@return bool 성공 여부
	*/
	virtual bool onStop();

	/**
		실제 객체의 reset을 수행하는 함수로 ManagedObject를 상속한 객체가 구현해야하는 함수. 
		reset() 함수에서 호출되는 callback 함수.

		@return bool 성공 여부
	*/
	virtual bool onReset();

	/**
		실제 객체의 destroy를 수행하는 함수로 ManagedObject를 상속한 객체가 구현해야하는 함수. 
		destroy() 함수에서 호출되는 callback 함수.

		@return bool 성공 여부
	*/
	virtual bool onDestroy();

	/**
		execution function in a dedicated thread of this manager.
		the thread is allocated by the component engine.
		this function is call only if this manager is "runnable".
	*/
	virtual void run();

	/**
		라이프 싸이클 리스너를 추가하는 함수.

		@param listener 추가할 리스터 객체
	*/
	void addListener(ManagedObjectListener *listener);
	/**
		라이프 싸이클 리스너를 삭제하는 함수.

		@param listener 삭제할 리스터 객체
	*/
	void removeListener(ManagedObjectListener *listener);

	/**
		모든 라이프 싸이클 리스너 객체를 반환하는 함수.

		@return ManagedObjectListenerList 리스터 객체 리스트
	*/
	ManagedObjectListenerList getListenerList();

	/**
		현재 상태를 반환하는 함수.

		@return ManagedObjectState 객체의 현재 상태
	*/
	ManagedObjectState getState();

	/**
		현재 상태를 스트링 형태로 반환하는 함수.

		@return std::string 객체의 현재 상태를 나타내는 스트링
	*/
	virtual std::string stateToStr();

};

#endif 
