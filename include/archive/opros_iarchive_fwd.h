/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2012. 11. 27
 *  @Author  : sby (sby@etri.re.kr)
 *
 *  @File    : opros_iarchive_fwd.h
 *
 *
 *
 *  forward declaration
 */

#ifndef _OPROS_IARHIVE_FWD_H
#define _OPROS_IARHIVE_FWD_H


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
	class iarchive;
	}
}

//
//  std::vector
//

template <typename T>
void load_this(opros::archive::iarchive &ar, std::vector<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::deque<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::list<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::valarray<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::stack<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::set<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::multiset<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::queue<T> &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::priority_queue<T> &t);

template <typename K,typename T>
void load_this(opros::archive::iarchive &ar, std::map<K,T> &t);

template <typename K,typename T>
void load_this(opros::archive::iarchive &ar, std::multimap<K,T> &t);

void load_this(opros::archive::iarchive &ar, std::string &t);
void load_this(opros::archive::iarchive &ar, std::wstring &t);

template <typename T>
void load_this(opros::archive::iarchive &ar, std::complex<T> &t);

#endif // _OPROS_IARHIVE_FWD_H
