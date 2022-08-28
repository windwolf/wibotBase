#include "wait_handler.hpp"

#include "stdint.h"

namespace ww
{

using namespace ww::os;

void WaitHandler::set_value(void *value)
{
    _value = value;
};
void *WaitHandler::get_value()
{
    return _value;
};
void *WaitHandler::get_sender()
{
    return _sender;
};

Result WaitHandler::wait()
{
    return wait(TIMEOUT_FOREVER);
};

void CallbackWaitHandler::done_callback_set(Callback onDone)
{
    _onDone = onDone;
};
void CallbackWaitHandler::error_callback_set(Callback onError)
{
    _onError = onError;
};
Result CallbackWaitHandler::reset()
{
    if (is_busy())
    {
        return Result_Busy;
    }
    _status.value = 0;
    return Result_OK;
};
bool CallbackWaitHandler::is_busy()
{
    return !_status.is_done && !_status.is_error;
};
Result CallbackWaitHandler::wait(uint32_t timeout)
{
    if (timeout == TIMEOUT_NOWAIT)
    {
        if (_status.is_done)
        {
            return Result_OK;
        }
        else if (_status.is_error)
        {
            return Result_GeneralError;
        }
        else
        {
            return Result_Timeout;
        }
    }
    else
    {
        uint32_t start = Utils::tick_get();
        while (!_status.is_done && !_status.is_error)
        {
            if (Utils::tick_diff(start) > timeout)
            {
                return Result_Timeout;
            }
        }
        if (_status.is_done)
        {
            return Result_OK;
        }
        else if (_status.is_error)
        {
            return Result_GeneralError;
        }
        else
        {
            return Result_StatusReserved;
        }
    }
};
void CallbackWaitHandler::done_set(void *sender)
{
    _sender = sender;
    _status.is_done = true;
    if (_onDone)
    {
        _onDone(_sender, _value, _receiver);
    }
};
void CallbackWaitHandler::error_set(void *sender)
{
    _sender = sender;
    _status.is_error = true;
    if (_onError)
    {
        _onError(_sender, _value, _receiver);
    }
};

Result PollingWaitHandler::reset()
{
    if (is_busy())
    {
        return Result_Busy;
    }
    _status.value = 0;
    return Result_OK;
};

bool PollingWaitHandler::is_busy()
{
    return !_status.is_done && !_status.is_error;
};
Result PollingWaitHandler::wait(uint32_t timeout)
{
    if (timeout == TIMEOUT_NOWAIT)
    {
        if (_status.is_done)
        {
            return Result_OK;
        }
        else if (_status.is_error)
        {
            return Result_GeneralError;
        }
        else
        {
            return Result_Timeout;
        }
    }
    else
    {
        uint32_t start = Utils::tick_get();
        while (!_status.is_done && !_status.is_error)
        {
            if (Utils::tick_diff(start) > timeout)
            {
                return Result_Timeout;
            }
        }
        if (_status.is_done)
        {
            return Result_OK;
        }
        else if (_status.is_error)
        {
            return Result_GeneralError;
        }
        else
        {
            return Result_StatusReserved;
        }
    }
};
void PollingWaitHandler::done_set(void *sender)
{
    _sender = sender;
    _status.is_done = true;
};
void PollingWaitHandler::error_set(void *sender)
{
    _sender = sender;
    _status.is_error = true;
};

using namespace ww::os;

Result EventGroupWaitHandler::init()
{
    return _eventGroup.init();
};
void EventGroupWaitHandler::deinit()
{
    _eventGroup.deinit();
};

Result EventGroupWaitHandler::reset()
{
    if (is_busy())
    {
        return Result_Busy;
    }
    _eventGroup.reset(_doneFlag | _errorFlag);
    return Result_OK;
};
bool EventGroupWaitHandler::is_busy()
{
    uint32_t events;
    auto rst =
        _eventGroup.wait(_doneFlag | _errorFlag, events,
                         EventOptions_WaitForAny | EventOptions_NoClear, 0);
    if (rst == Result::Result_OK)
    {
        return false;
    }
    else
    {
        return true;
    }
};

void EventGroupWaitHandler::done_set(void *sender)
{
    _sender = sender;
    _eventGroup.set(_doneFlag);
};
void EventGroupWaitHandler::error_set(void *sender)
{
    _sender = sender;
    _eventGroup.set(_errorFlag);
};
Result EventGroupWaitHandler::wait(uint32_t timeout)
{

    uint32_t events;

    auto rst = _eventGroup.wait(_doneFlag | _errorFlag, events,
                                EventOptions_WaitForAny | EventOptions_NoClear,
                                timeout);
    if (rst == Result::Result_OK)
    {
        if (events & _doneFlag)
        {
            return Result_OK;
        }
        else if (events & _errorFlag)
        {
            return Result_GeneralError;
        }
        else
        {
            // TODO: no possible to reach here
            return Result_Timeout;
        }
    }
    else
    {
        return Result_Timeout;
    }
};
} // namespace ww
