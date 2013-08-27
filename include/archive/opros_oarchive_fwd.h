/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2010. 8. 11
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_archive_stl.h
 *
 *
 *
 *  forward declaration
 */

#ifndef _OPROS_OARHIVE_FWD_H
#define _OPROS_OARHIVE_FWD_H


#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <list>
#include <valarray>
#include <stack>
#include <set>
#include <map>
#include <complex>

namespace opros{
	namespace archive{
	class oarchive;
	}
}


template <typename  T>
void save_this(opros::archive::oarchive &ar, std::vector<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::deque<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::list<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::valarray<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::stack<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::set<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::multiset<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::queue<T> &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::priority_queue<T> &t);

template <typename K,typename T>
void save_this(opros::archive::oarchive &ar, std::map<K,T> &t);

template <typename K,typename T>
void save_this(opros::archive::oarchive &ar, std::multimap<K,T> &t);

void save_this(opros::archive::oarchive &ar, std::string &t);

void save_this(opros::archive::oarchive &ar, std::wstring &t);

template <typename T>
void save_this(opros::archive::oarchive &ar, std::complex<T> &t);


#endif // _OPROS_OARHIVE_FWD_H
