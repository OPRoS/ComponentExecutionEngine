/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoSelector.cpp
 *
 */

#include "IoSelector.h"

#include "system/os/OSIncludes.h"

#include "system/Log.h"
#include "system/Trace.h"

IoSelector::IoSelector()
{
	trace_enter();

	m_maxSockHandle = INVALID_SOCKET;

	FD_ZERO(&m_rset);
	FD_ZERO(&m_wset);
	FD_ZERO(&m_eset);

	m_closed = false;
}

IoSelector::~IoSelector()
{
	trace_enter();

	close();
}

bool IoSelector::registerEventTarget(IoEventTarget *target)
{
	trace_enter();

	if (target == NULL) {
		log_error("IoEventTarget is NULL");
		return false;
	}

	OSSocketHandle sockH = target->getSocketHandle();
	if (sockH == INVALID_SOCKET) {
		log_error("Invalid Socket Handle");
		return false;
	}

	OSGuard guard(&m_mutex);

	IoEvent evt = target->getTargetEvent();
	bool registered = false;
	if (IoEventUtil::canAcceptEvent(evt) || IoEventUtil::canReadEvent(evt)) {
		FD_SET(sockH, &m_rset);
		registered = true;
	}
	if (IoEventUtil::canWriteEvent(evt) || IoEventUtil::canConnectEvent(evt)) {
		FD_SET(sockH, &m_wset);
		registered = true;
	}
	if (IoEventUtil::canCloseEvent(evt)) {
		FD_SET(sockH, &m_eset);
		registered = true;
	}

	if (registered) {
		m_readyList[sockH] = target;

		if (m_maxSockHandle == INVALID_SOCKET || m_maxSockHandle < sockH) {
			m_maxSockHandle = sockH;
		}
	}

	return true;
}

bool IoSelector::unregisterEventTarget(IoEventTarget *target)
{
	trace_enter();

	if (target == NULL) {
		log_error("IoEventTarget is NULL");
		return false;
	}

	OSSocketHandle sockH = target->getSocketHandle();;
	if (sockH == INVALID_SOCKET) {
		log_error("Invalid Socket Handle");
		return false;
	}

	OSGuard guard(&m_mutex);

	FD_CLR(sockH, &m_rset);
	FD_CLR(sockH, &m_wset);
	FD_CLR(sockH, &m_eset);

	IoEventTargetSelectorMap ::iterator iter = m_readyList.begin();
    for (; iter != m_readyList.end(); iter++) {
		if (iter->first == sockH) {
			m_readyList.erase(iter);
			break;
		}
    }
	target->close();

	return true;
}

void IoSelector::close()
{
	trace_enter();

	m_closed = true;
}

void IoSelector::removeEventTarget(IoEventTarget *target)
{
	trace_enter();

	OSGuard guard(&m_mutex);
	if (target != NULL) {
		trace("target.id=" << target->getId());

		OSSocketHandle sockH = target->getSocketHandle();
		IoEventTargetSelectorMap ::iterator pos = m_readyList.find(sockH);
		if (pos != m_readyList.end()) {
			trace("target.id=" << target->getId() << " removing...");

			m_readyList.erase(pos);
		}

		FD_CLR(sockH, &m_rset);
		FD_CLR(sockH, &m_wset);
		FD_CLR(sockH, &m_eset);

		target->close();

		//delete target;   // CAUTION
	}
}

void IoSelector::removeEventTargetAll()
{
	trace_enter();

	OSGuard guard(&m_mutex);
	IoEventTargetSelectorMap::iterator pos = m_readyList.begin();
	for (; pos != m_readyList.end(); ++pos) {
		IoEventTarget *target = pos->second;

		if (target != NULL) {
			OSSocketHandle sockH = target->getSocketHandle();

			FD_CLR(sockH, &m_rset);
			FD_CLR(sockH, &m_wset);
			FD_CLR(sockH, &m_eset);

			target->close();

			///delete target;  // CAUTION
		}
	}
	m_readyList.clear();
}

void IoSelector::run()
{
	trace_enter_msg("***************** RUN!!! : timeout=" << getTimeout());

	fd_set t_rset;
    fd_set t_wset;
    fd_set t_eset;

	IoEventTargetList targetList;
	while (!m_closed) {
		m_mutex.lock();
			t_rset = m_rset;
			t_wset = m_wset;
			t_eset = m_eset;
		m_mutex.unlock();

		long timeout = getTimeout();
		if ( timeout <= 0) timeout = IO_SELECTOR_TIMEOUT;

		int retval;
		if (m_maxSockHandle != INVALID_SOCKET && m_readyList.size() > 0) {
			retval = OS::select(m_maxSockHandle+1, &t_rset, &t_wset, &t_eset, timeout);
		}
		else {
			uint32 mstime = timeout < IO_SELECTOR_TIMEOUT ? timeout : IO_SELECTOR_TIMEOUT;
			if (mstime <= 0) mstime = IO_SELECTOR_TIMEOUT;
			OS::sleepMS(mstime);
			continue;
		}

		if (m_closed) {
			log_info("Select closed...");
			removeEventTargetAll();
			break;
		}
		if (retval == SOCKET_ERROR) {
			log_error("Select socket error");
			removeEventTargetAll();
			break;
		}

		m_mutex.lock();
		targetList.clear();
		IoEventTargetSelectorMap::iterator pos = m_readyList.begin();
		for (; pos != m_readyList.end(); ++pos) {
			IoEventTarget *target = pos->second;
			targetList.push_back(target);
		}
		m_mutex.unlock();

		IoEventTargetList::iterator listPos = targetList.begin();
		for (; listPos != targetList.end(); ++listPos) {
			IoEventTarget *target = *listPos;
			IoEvent tevt = target->getTargetEvent();
			OSSocketHandle sockH = target->getSocketHandle();

			if (FD_ISSET(sockH, &t_rset)) {
				if (IoEventUtil::canAcceptEvent(tevt)) {
					IoEventHandler *evtHandler = target->getEventHandler();
					log_info("***** handleAccept : id=" << target->getId());
					if (evtHandler != NULL) {
						evtHandler->handleAccept(target);
					}
				}

				if (IoEventUtil::canConnectEvent(tevt)) {
					IoEventHandler *evtHandler = target->getEventHandler();
					log_info("***** handleConnect : id=" << target->getId());
					if (evtHandler != NULL) {
						evtHandler->handleConnect(target);
					}
				}

				if (IoEventUtil::canReadEvent(tevt)) {
					IoEventHandler *evtHandler = target->getEventHandler();
					if (evtHandler != NULL && !evtHandler->handleRead(target)) {						
						removeEventTarget(target);         // CAUTION
						evtHandler->handleClose(target);
						continue;
					}
				}
			}

			if (FD_ISSET(sockH, &t_wset)) {
				if (IoEventUtil::canWriteEvent(tevt)) {
					IoEventHandler *evtHandler = target->getEventHandler();
					if (evtHandler != NULL && !evtHandler->handleWrite(target)) {						
						removeEventTarget(target);         // CAUTION
						evtHandler->handleClose(target);
						continue;
					}
				}
			} // end of if

			// OOB(out-of-band) data
			/*
			if (FD_ISSET(sockH, &t_eset)) {
				IoEventHandler *evtHandler = target->getEventHandler();
				if (evtHandler != NULL) {
					evtHandler->handleOther(target, IoEventUtil::noneEvent());
				}
			}
			*/
		}
	}
}



