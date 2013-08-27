/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 26
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventTarget.cpp
 *
 */

#include "IoEventTarget.h"

#include <algorithm>

#include "system/Log.h"
#include "system/Trace.h"

using namespace std;

IoEventTarget::IoEventTarget()
{
	m_isOpen = false;
	m_processor = NULL;
	m_cnnHandler = NULL;
}

IoEventTarget::~IoEventTarget()
{
	trace_enter_msg("IoEventTarget.id=" + m_id);
}


bool IoEventTarget::open()
{
	if (isOpen()) return true;

	if (onOpen()) {
		setOpen(true);
		fireOpened();

		return true;
	}
	else {
		return false;
	}
}

bool IoEventTarget::close()
{
	trace_enter();

	if (isOpen() && onClose()) {
		setOpen(false);
		fireClosed();
	}

	return true;
}

void IoEventTarget::addListener(IoEventTargetListener *l)
{
	m_listeners.push_back(l);
}

void IoEventTarget::removeListener(IoEventTargetListener *l)
{
	remove(m_listeners.begin(), m_listeners.end(), l);
}

IoEventTargetListenerList IoEventTarget::getListenerList()
{
	return m_listeners;
}

void IoEventTarget::fireOpened()
{
	IoEventTargetListenerList::iterator pos = m_listeners.begin();
	for (; pos != m_listeners.end(); ++pos) {
		IoEventTargetListener *l = *pos;
		l->onOpened(this);
	}
}

void IoEventTarget::fireClosed()
{
	IoEventTargetListenerList::iterator pos = m_listeners.begin();
	for (; pos != m_listeners.end(); ++pos) {
		IoEventTargetListener *l = *pos;
		l->onClosed(this);
	}
}
