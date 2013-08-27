/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 11. 7
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : IoEventUtil.h
 *
 */

#ifndef _IO_EVENT_UTIL_H_
#define _IO_EVENT_UTIL_H_

#define IO_EVT_NONE		0x0000
#define IO_EVT_ACCEPT	0x0001
#define IO_EVT_CONNECT	0x0002
#define IO_EVT_READ 	0x0004
#define IO_EVT_WRITE	0x0008
#define IO_EVT_CLOSE	0x0010
#define IO_EVT_ALL		0xFFFF

typedef unsigned int IoEvent;

class IoEventUtil
{
public:
	IoEventUtil();
	virtual ~IoEventUtil();

	static IoEvent noneEvent() { return IO_EVT_NONE; };
	static IoEvent allEvent() { return IO_EVT_ALL; };
	static IoEvent acceptEvent() { return IO_EVT_ACCEPT; };
	static IoEvent connectEvent() { return IO_EVT_CONNECT; };
	static IoEvent readEvent() { return IO_EVT_READ; };
	static IoEvent writeEvent() { return IO_EVT_WRITE; };
	static IoEvent closeEvent() { return IO_EVT_CLOSE; };
	static IoEvent connectionEvent() { return readEvent() | closeEvent(); };

	static IoEvent setEvent(IoEvent evt, IoEvent sevt) { return evt | sevt; };
	static IoEvent maskEvent(IoEvent evt, IoEvent mask) { return evt & mask; };
	static IoEvent clearEvent(IoEvent evt, IoEvent cevt) { return evt & ~cevt; };
	static bool canEvent(IoEvent evt, IoEvent isEvt) { return (evt & isEvt) == isEvt; };
	static bool isNoneEvent(IoEvent evt) { return evt == IO_EVT_NONE; };
	static bool isAllEvent(IoEvent evt) { return evt == IO_EVT_ALL; };

	static IoEvent setAcceptEvent(IoEvent evt) { return setEvent(evt, IO_EVT_ACCEPT); };
	static IoEvent clearAcceptEvent(IoEvent evt) { return clearEvent(evt, IO_EVT_ACCEPT); };
	static bool canAcceptEvent(IoEvent evt) { return canEvent(evt, IO_EVT_ACCEPT); };

	static IoEvent setConnectEvent(IoEvent evt) { return setEvent(evt, IO_EVT_CONNECT); };
	static IoEvent clearConnectEvent(IoEvent evt) { return clearEvent(evt, IO_EVT_CONNECT); };
	static bool canConnectEvent(IoEvent evt) { return canEvent(evt, IO_EVT_CONNECT); };

	static IoEvent setReadEvent(IoEvent evt) { return setEvent(evt, IO_EVT_READ); };
	static IoEvent clearReadEvent(IoEvent evt) { return clearEvent(evt, IO_EVT_READ); };
	static bool canReadEvent(IoEvent evt) { return canEvent(evt, IO_EVT_READ); };

	static IoEvent setWriteEvent(IoEvent evt) { return setEvent(evt, IO_EVT_WRITE); };
	static IoEvent clearWriteEvent(IoEvent evt) { return clearEvent(evt, IO_EVT_WRITE); };
	static bool canWriteEvent(IoEvent evt) { return canEvent(evt, IO_EVT_WRITE); };

	static IoEvent setCloseEvent(IoEvent evt) { return setEvent(evt, IO_EVT_CLOSE); };
	static IoEvent clearCloseEvent(IoEvent evt) { return clearEvent(evt, IO_EVT_CLOSE); };
	static bool canCloseEvent(IoEvent evt) { return canEvent(evt, IO_EVT_CLOSE); };

};

#endif /* IOEVENTUTIL_H_ */
