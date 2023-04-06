#ifndef __WW_OS_FREERTOS_PORT_HPP__
#define __WW_OS_FREERTOS_PORT_HPP__

#include <stdbool.h>

#include "base.hpp"
#include "ringbuffer.hpp"

namespace wibot::os {

struct _MessageQueueInstance_ {
    _MessageQueueInstance_(void* queue_addr, uint32_t message_size, uint32_t queue_size)
        : buffer(queue_addr, message_size, queue_size), message_size(message_size),
          queue_size(queue_size){};
    RingBuffer buffer;
    uint32_t   message_size;
    uint32_t   queue_size;
};
#define MESSAGEQUEUE_TYPEDEF _MessageQueueInstance_
#define MUTEX_TYPEDEF        bool
#define EVENTGROUP_TYPEDEF   uint32_t
#define SEMAPHORE_TYPEDEF    uint32_t
#define THREAD_TYPEDEF       uint32_t
}  // namespace wibot::os

#endif  // __WW_OS_FREERTOS_PORT_HPP__
