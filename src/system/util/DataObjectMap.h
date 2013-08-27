/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 7. 4
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : DataObjectMap.h
 *
 */

#ifndef _DATA_MAP_H_
#define _DATA_MAP_H_

#include <string>
#include <map>

#include "system/util/BasicTypes.h"
#include "DataObject.h"

/**
	(std::string, DataObject)를 저장하기 위한 맵 클래스.
*/
class DataObjectMap
{
	/**
		DataObject 객체를 담고 있는 홀더(holder) 클래스.
	*/
	struct DataObjectHolderTag {
		/** 
			객체가 삭제될 때 내부에 저장하고 있는 DataObject를 
			메모리에서 삭제할 지 나타내는 플래그 
		*/
		bool owned;
		DataObject *data;

		/** 생성자. */
		DataObjectHolderTag() : data(NULL) {};

		/** 소멸자. 만약 owned가 true이면 내부에 저장한 DataObject 객체를 메모리에서도 삭제함. */
		~DataObjectHolderTag() { if (owned && data != NULL) delete data; };
	} ;

	typedef struct DataObjectHolderTag DataHolder;
	typedef std::map<std::string , DataHolder *> DataObjectMapType;

	/** DataObject를 저장할 맵 */
	DataObjectMapType m_map;

public:
	DataObjectMap();
	virtual ~DataObjectMap();

	/** 
		저장하고 있는 모든 객체를 맵에서 삭제.
	*/
	void clear();

	/** 
		DataObject 객체를 추가.

		@param name DataObject 이름
		@param data 저장할 DataObject
		@param owned 자동 삭제 여부 (true이면 맵에서 삭제시 메모리에서 자동 삭제 됨)
	*/
	void setData(const std::string  &name, DataObject *data, bool owned = true);

	/**
		DataObject 객체 검색 함수.

		@param name 검색할 DataObject 객체 이름

		@return DataObject* 검색된 DataObject 객체
	*/
	DataObject *getData(const std::string  &name);

	/**
		DataObject 객체 검색 함수.

		@param name 검색할 DataObject 객체 이름
		@param defaultValue DataObject이 존재하지 않을 때 반환할 객체

		@return DataObject* 검색된 DataObject 객체
	*/
	DataObject *getData(const std::string  &name, DataObject *defaultValue);

	/**
		DataObject 객체 삭제 함수.

		@param name 삭제할 DataObject 객체 이름

		@return bool 성공 여부
	*/
	bool removeData(const std::string  &name);

	/**
		DataObject 객체가 존재하는 지 검사하는 함수.

		@param name 검사할 DataObject 객체 이름

		@return bool 존재 여부
	*/
	bool isExist(const std::string  &name);

	/**
		저장된 모든 DataObject 객체의 이름을 반환하는 함수.

		@return StringList DataObject 객체 이름 리스트
	*/
	StringList getNames();
};

#endif 
