#ifndef __WW_OS_FREERTOS_PORT_HPP__
#define __WW_OS_FREERTOS_PORT_HPP__

#include "cmsis_os.h"
#include "cmsis_os2.h"

namespace wibot::os {
#define MESSAGEQUEUE_TYPEDEF StaticQueue_t
#define MUTEX_TYPEDEF StaticSemaphore_t
#define EVENTGROUP_TYPEDEF StaticEventGroup_t
#define SEMAPHORE_TYPEDEF StaticSemaphore_t
#define THREAD_TYPEDEF StaticTask_t
}  // namespace wibot::os

#endif  // __WW_OS_FREERTOS_PORT_HPP__
