#include "cmsis_os2.h"
#include "os.hpp"

namespace wibot::os {
void Utils::delay(uint32_t ms) {
    osDelay(ms);
};

uint32_t Utils::tick_get() {
    return osKernelGetTickCount();
};

void Thread::sleep(uint32_t ms) {
    osDelay(ms);
};

Mutex::Mutex(const char* name) : name_(name){};

Mutex::~Mutex(){};
Result Mutex::_init() {
    osMutexAttr_t attr = {
        .name    = name_,
        .cb_mem  = &(this->_instance),
        .cb_size = sizeof(this->_instance),
    };

    osMutexNew(&attr);
};
void Mutex::_deinit() {
    osMutexDelete(&(this->_instance));
};

bool Mutex::lock(uint32_t timeout) {
    return osMutexAcquire(&(this->_instance), timeout) == osOK;
};

void Mutex::unlock() {
    osMutexRelease(&(this->_instance));
};

EventGroup::EventGroup(const char* name) : name_(name){};
EventGroup::~EventGroup(){};
Result EventGroup::_init() {
    osEventFlagsAttr_t attr = {
        .name    = name_,
        .cb_mem  = &(this->_instance),
        .cb_size = sizeof(this->_instance),
    };

    osEventFlagsNew(&attr);
};
void EventGroup::_deinit() {
    osEventFlagsDelete(&(this->_instance));
};

Result EventGroup::set(uint32_t flags) {
    auto rst = osEventFlagsSet(&(this->_instance), flags);
    if ((int32_t)rst < 0) {
        return (Result)rst;
    } else {
        return Result::OK;
    }
};

Result EventGroup::reset(uint32_t flags) {
    auto rst = osEventFlagsClear(&(this->_instance), flags);
    if ((int32_t)rst < 0) {
        return (Result)rst;
    } else {
        return Result::OK;
    }
};

Result EventGroup::wait(uint32_t flags, uint32_t& actualFlags, EventOptions options,
                        uint32_t timeout) {
    uint32_t freertosOptions = 0;
    if ((options & EventOptions_Clear_Flag) == EventOptions_NoClear) {
        freertosOptions |= osFlagsNoClear;
    }
    if ((options & EventOptions_Wait_Flag) == EventOptions_WaitForAll) {
        freertosOptions |= osFlagsWaitAll;
    }
    // TODO: HANDLE options
    auto rst = osEventFlagsWait(&(this->_instance), flags, freertosOptions, timeout);
    if ((int32_t)rst < 0) {
        return (Result)rst;
    } else {
        actualFlags = rst;
        return Result::OK;
    }
};

MessageQueue::MessageQueue(const char* name, void* msg_addr, uint32_t msg_size, uint32_t queue_size)
    : name_(name), msg_addr_(msg_addr), msg_size_(msg_size), queue_size_(queue_size){};

Result MessageQueue::_init() {
    osMessageQueueAttr_t attr = {
        .name    = name_,
        .cb_mem  = &(this->_instance),
        .cb_size = sizeof(this->_instance),
        .mq_mem  = msg_addr_,
        .mq_size = msg_size_ * queue_size_ * 4,
    };

    osMessageQueueNew(queue_size_, msg_size_ * 4, &attr);
};
void MessageQueue::_deinit() {
    osEventFlagsDelete(&(this->_instance));
};
}  // namespace wibot::os
