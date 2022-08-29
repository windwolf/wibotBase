#include "cmsis_os2.h"
#include "os.hpp"

namespace ww::os
{
void Utils::delay(uint32_t ms)
{
    osDelay(ms);
};

uint32_t Utils::tick_get()
{
    return osKernelGetTickCount();
};

void Thread::sleep(uint32_t ms)
{
    osDelay(ms);
};

Mutex::Mutex(const char *name) : _name(name)
{
    osMutexAttr_t attr = {
        .name = _name,
        .cb_mem = &(this->_instance),
        .cb_size = sizeof(this->_instance),

    };
    initErrorCode = (osMutexNew(&attr) != NULL) ? Result_OK : Result_NoResource;
};

void Mutex::~Mutex()
{
    osMutexDelete(&(this->_instance));
};

bool Mutex::lock(uint32_t timeout)
{
    return osMutexAcquire(&(this->_instance), timeout) == osOK;
};

void Mutex::unlock()
{
    osMutexRelease(&(this->_instance));
};

EventGroup::EventGroup(const char *name) : _name(name)
{

    osEventFlagsAttr_t attr = {
        .name = _name,
        .cb_mem = &(this->_instance),
        .cb_size = sizeof(this->_instance),
    };
    initErrorCode =
        (osEventFlagsNew(&attr) != NULL) ? Result_OK : Result_NoResource;
};
EventGroup::~EventGroup()
{
    osEventFlagsDelete(&(this->_instance));
};

Result EventGroup::set(uint32_t flags)
{
    auto rst = osEventFlagsSet(&(this->_instance), flags);
    if ((int32_t)rst < 0)
    {
        return (Result)rst;
    }
    else
    {
        return Result::Result_OK;
    }
};

Result EventGroup::reset(uint32_t flags)
{
    auto rst = osEventFlagsClear(&(this->_instance), flags);
    if ((int32_t)rst < 0)
    {
        return (Result)rst;
    }
    else
    {
        return Result::Result_OK;
    }
};

Result EventGroup::wait(uint32_t flags, uint32_t &actualFlags,
                        EventOptions options, uint32_t timeout)
{
    auto rst = osEventFlagsWait(&(this->_instance), flags, options, timeout);
    if ((int32_t)rst < 0)
    {
        return (Result)rst;
    }
    else
    {
        actualFlags = rst;
        return Result::Result_OK;
    }
};
} // namespace ww::os