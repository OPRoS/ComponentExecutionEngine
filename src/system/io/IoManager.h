/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 8. 22
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoManager.h
 *
 */

#ifndef _COMMUNICATION_MANAGER_H_
#define _COMMUNICATION_MANAGER_H_

#include <string>

#include "system/Manager.h"

#include "IoEventHandler.h"
#include "IoConnectionHandler.h"
#include "IoEventProcessor.h"
#include "IoTransport.h"
#include "IoEventTargetManager.h"
#include "IoInfo.h"

typedef std::map<std::string, IoConnectionHandler *> IoConnectionHandlerMap;
typedef std::map<std::string, IoEventProcessor *> IoEventProcessorMap;

/**
	컴포넌트 실행엔진의 네트워크 관련 기능을 총괄하는 클래스.
	- io 관리.
	- 이벤트 처리기(IoEventProcessor) 관리
	- 트랜스포트(IoTransport) 관리
	- 커넥션 핸들러(IoConnectionHandler) 관리
*/
class IoManager : public Manager, public IoEventTargetManager, public IoEventHandler
{
	/** 커넥션 핸들러(IoConnectionHandler) 저장을 위한 맵 */
	IoConnectionHandlerMap m_dpMap;

	/** 이벤트 처리기(IoEventProcessor) 저장을 위한 맵 */
	IoEventProcessorMap m_epMap;

	/** 트랜스포트(IoTransport) 저장을 위한 맵 */
	IoTransportMap m_tpMap;

protected:
	/**
		특정 타입의 이벤트 처리기 생성 함수.
		id는 "selector", "wsaevent", ..." 등이 가능함
		현재는 "selector"(IoSelector)만 제공함.

		@param type 생성할 이벤트 프로세서 타입

		@return IoEventProcessor * 생성된 이벤트 처리기
	*/
	IoEventProcessor *createEventProcessor(const std::string &type);

	/**
		특정 타입(예, tcp,...)의 트랜스포트 생성 함수

		@param type 생성할 트랜스포트 타입

		@return IoEventProcessor * 생성된 트랜스포트 처리기
	*/
	IoTransport *createTransport(const std::string &type);

	/**
		XML 정보를 이용하여 이벤트 처리기를 등록하는 함수

		@param cfg XML로부터 생성된 Config 객체

		@return bool 성공 여부
	*/
	bool registerEventProcessor(Config *cfg);

	/**
		XML 정보를 이용하여 Io 객체를 등록하는 함수

		@param cfg XML로부터 생성된 Config 객체

		@return bool 성공 여부
	*/
	bool registerIo(Config *cfg);

	/**
		모든 이벤트 처리기를 시작시키는 함수

		@return bool 성공 여부
	*/
	bool startEventProcessor();

	/**
		모든 Io 객체를 시작시키는 함수

		@return bool 성공 여부
	*/
	bool startIo();

	/**
		모든 이벤트 처리기를 중지시키는 함수

		@return bool 성공 여부
	*/
	bool stopEventProcessor();

	/**
		모든 Io 객체를 중지시키는 함수

		@return bool 성공 여부
	*/
	bool stopIo();

public:
	IoManager();
	virtual ~IoManager();

	/**
		커넥션 핸들러를 추가하는 함수.

		@param ch 추가할 커넥션 핸들러 객체

		@return bool 성공 여부
	*/
	bool addConnectionHandler(IoConnectionHandler *ch);

	/**
		커넥션 핸들러를 삭제하는 함수. 삭제한 커넥션 핸들러 반환.

		@param id 삭제할 커넥션 핸들러 아이디

		@return IoConnectionHandler * 삭제한 커넥션 핸들러 객체
	*/
	IoConnectionHandler *removeConnectionHandler(const std::string &id);

	/**
		커넥션 핸들러를 검색하는 함수. 

		@param id 검색할 커넥션 핸들러 아이디

		@return IoConnectionHandler * 검색한 커넥션 핸들러 객체
	*/
	IoConnectionHandler *getConnectionHandler(const std::string &id);

	/**
		이벤트 처리기를 추가(attatch)하는 함수. 

		@param processor 추가할 이벤트 처리기 객체

		@return bool 성공여부
	*/
	bool attachEventProcessor(IoEventProcessor *processor);

	/**
		이벤트 처리기를 제거하는 함수. 삭제한 이벤트 처리기 객체 반환.

		@param id 삭제할 이벤트 처리기 아이디

		@return IoEventProcessor * 삭제한 이벤트 처리기 객체
	*/
	IoEventProcessor *detachEventProcessor(const std::string &id);

	/**
		이벤트 처리기를 검색하는 함수. 

		@param id 검색할 이벤트 처리기 아이디

		@return IoEventProcessor * 검색한 이벤트 처리기 객체
	*/
	IoEventProcessor *getEventProcessor(const std::string &id);

	/**
		이벤트 처리기를 제거하는 함수. 
		이벤트 처리기를 메모리에서 삭제함.

		@param id 제거할 이벤트 처리기 아이디
	*/
	void releaseEventProcessor(const std::string &id);

	/**
		등록된 모든 이벤트 처리기를 제거하는 함수. 
	*/
	void releaseEventProcessorAll();

	/**
		트랜스포트 객체를 추가(attatch)하는 함수. 

		@param transport 추가할 트랜스포트 객체 객체

		@return bool 성공여부
	*/
	bool attachTransport(IoTransport *transport);

	/**
		트랜스포트 객체를 제거하는 함수. 삭제한 트랜스포트 객체 반환.

		@param type 삭제할 트랜스포트 객체 타입

		@return IoTransport * 삭제한 트랜스포트 객체
	*/
	IoTransport *detachTransport(const std::string &type);

	/**
		트랜스포트 객체를 검색하는 함수. 

		@param type 검색할 트랜스포트 객체 타입

		@return IoTransport * 검색한 트랜스포트 객체
	*/
	IoTransport *getTransport(const std::string &type);

	/**
		트랜스포트 객체를 제거하는 함수. 
		트랜스포트 객체를 메모리에서 삭제함.

		@param type 제거할 트랜스포트 객체 타입
	*/
	void releaseTransport(const std::string &type);

	/**
		등록된 모든 트랜스포트 객체를 제거하는 함수. 
	*/
	void releaseTransportAll();

	/**
		이벤트 처리기를 등록하는 함수.


		@param id 이벤트 처리기 아이디
		@param type 이벤트 처리기 타입
		@param timeout 이벤트 처리기의 타입아웃 값


		@return bool 성공 여부
	*/
	bool registerEventProcessor(const std::string &id, const std::string &type, long timeout);
	bool startEventProcessor(const std::string &id);
	bool startEventProcessor(IoEventProcessor *processor);
	bool stopEventProcessor(const std::string &id);
	bool stopEventProcessor(IoEventProcessor *processor);

	bool registerIo(IoInfo &info);
	bool startIo(const std::string &id);
	bool startIo(IoEventTarget *target);
	bool stopIo(const std::string &id);
	bool stopIo(IoEventTarget *target);

	virtual bool onInit();
	virtual bool onStart();
	virtual bool onStop();
	virtual bool onDestroy();

	// IoEventHandler
	virtual bool handleAccept(IoEventTarget *target);
	virtual bool handleConnect(IoEventTarget *target);
	virtual bool handleRead(IoEventTarget *target);
	virtual bool handleWrite(IoEventTarget *target);
	virtual bool handleClose(IoEventTarget *target);
	virtual bool handleOther(IoEventTarget *target, IoEvent evt);
};

#endif
