#ifndef __THREADX_PORT_HPP__
#define __THREADX_PORT_HPP__

#include "tx_api.h"

namespace ww::os
{
#define QUEUE_TYPEDEF TX_QUEUE
#define MUTEX_TYPEDEF TX_MUTEX
#define EVENTGROUP_TYPEDEF TX_EVENT_FLAGS_GROUP
#define SEMAPHORE_TYPEDEF TX_SEMAPHORE
#define THREAD_TYPEDEF TX_THREAD
} // namespace ww::os

#endif // __THREADX_PORT_HPP__