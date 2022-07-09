#ifndef __THREADX_PORT_H__
#define __THREADX_PORT_H__

#include "cmsis_os.h"
#include "cmsis_os2.h"

#define DRIVER_MUTEX StaticQueue_t
#define DRIVER_EVENTS StaticEventGroup_t

#endif // __THREADX_PORT_H__