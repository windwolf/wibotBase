#ifndef __WW_OS_FREERTOS_PORT_HPP__
#define __WW_OS_FREERTOS_PORT_HPP__

#include <stdbool.h>

namespace wibot::os
{
#define QUEUE_TYPEDEF uint32_t
#define MUTEX_TYPEDEF bool
#define EVENTGROUP_TYPEDEF uint32_t
#define SEMAPHORE_TYPEDEF uint32_t
#define THREAD_TYPEDEF uint32_t
} // namespace wibot::os

#endif // __WW_OS_FREERTOS_PORT_HPP__
