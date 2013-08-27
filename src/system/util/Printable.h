/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 10. 29
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : Printable.h
 *
 */

#ifndef _PRINTABLE_H_
#define _PRINTABLE_H_

#include <string>
#include <iostream>

/**
	std::ostream에 출력하려고 하는 객체들의 기본 클래스.
*/
class Printable
{
public:
	Printable();
	virtual ~Printable();

	/**
		std::ostream에 메시지를 출력하는 함수.
		Printable을 상속한 클래스에서 구현해야 함.

		@param std::ostream 출력할 스트림
		@param level 탭(들여쓰기) 갯수
	*/
	virtual void print(std::ostream &os, int level = 0) = 0;

	/**
		std::cout에 메시지를 출력하는 함수.
	*/
	void print() { print(0); };

	/**
		std::cout에 메시지를 출력하는 함수.

		@param level 탭(들여쓰기) 갯수
	*/
	void print(int level);

	/**
		파일에 메시지를 출력하는 함수.

		@param filename 출력할 파일의 이름
		@param level 탭(들여쓰기) 갯수
	*/
	void print(const std::string &filename, int level = 0);

	/**
		std::ostream에 메시지를 출력하는 함수.

		@param std::ostream 출력할 스트림
		@param level 탭(들여쓰기) 갯수
	*/
	void printLevel(std::ostream &os, int level);

	/**
		std::ostream에 (name, value)를 출력하는 함수.

		@param std::ostream 출력할 스트림
		@param level 탭(들여쓰기) 갯수
		@param name 출력할 name 값
		@param value 출력할 value 값
	*/
	void printNV(std::ostream &os, int level, const std::string &name, const std::string &value);
};

#endif 
