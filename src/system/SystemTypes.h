/*
 * OPRoS Component Engine (OCE)
 * Copyright (c) 2008 ETRI. http://www.etri.re.kr.
 * Distributed under the OPRoS License, Version 1.0.
 *
 *  @Created : 2008. 9. 2
 *  @Author  : swjung (swjung@etri.re.kr)
 *
 *  @File    : SystemTypes.h
 *
 */

#ifndef _SYSTEM_TYPES_H_
#define _SYSTEM_TYPES_H_

/** ComponentAdapter 객체의 타입 정의*/
typedef enum { CA_INVALID, CA_ATOMIC, CA_COMPOSITE, CA_REMOTE_PROXY, CA_LOCAL_PROXY } ComponentAdapterType;

/** ComponentAdapter의 상태 정의*/
typedef enum { CA_CREATED, CA_LOADED, CA_CONNECTED, CA_ACTIVATED } ComponentAdapterStatus;

/** 컴포넌트 포트의 타입 정의*/
typedef enum { CA_PORT_SERVICE, CA_PORT_DATA, CA_PORT_EVENT } ComponentPortType;

/** 컴포넌트 포트의 방향 정의*/
typedef enum { CA_PORT_REQUIRED, CA_PORT_PROVIDED, CA_PORT_INPUT, CA_PORT_OUTPUT } ComponentPortDir;


#define DEFAUT_PARENT_NAME "SYSTEM"






#endif
