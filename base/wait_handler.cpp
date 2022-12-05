#include "wait_handler.hpp"

#include "stdint.h"

namespace wibot
{

	using namespace wibot::os;

	WaitHandler::WaitHandler(EventGroup& eventGroup, uint32_t doneFlag, uint32_t errorFlag)
		: _eventGroup(eventGroup), _doneFlag(doneFlag), _errorFlag(errorFlag)
	{
	};


	void WaitHandler::set_value(void* value)
	{
		_value = value;
	};
	void* WaitHandler::get_value()
	{
		return _value;
	};
	void* WaitHandler::get_sender()
	{
		return _sender;
	};

	void WaitHandler::done_callback_set(void* receiver, Callback onDone)
	{
		_receiver = receiver;
		_onDone = onDone;
	};
	void WaitHandler::error_callback_set(void* receiver, Callback onError)
	{
		_receiver = receiver;
		_onError = onError;
	};

// Result WaitHandler::wait()
// {
//     return wait(0, TIMEOUT_FOREVER);
// };
// Result WaitHandler::wait(uint32_t level)
// {
//     return wait(level, TIMEOUT_FOREVER);
// };
	Result WaitHandler::wait(uint32_t level, uint32_t timeout)
	{
		uint32_t events;
		uint32_t startTick = Utils::tick_get();
		uint32_t duration = 0;
		Result rst = Result::OK;
		for (; timeout > duration; duration = Utils::tick_diff(startTick))
		{
			rst = _eventGroup.wait(_doneFlag | _errorFlag, events,
				EventOptions_WaitForAny | EventOptions_NoClear, timeout - duration);
			if (rst == Result::OK)
			{
				if (events & _errorFlag)
				{
					rst = Result::GeneralError;
					break;
				}
				else if (events & _doneFlag)
				{
					if (level == _level)
					{
						rst = Result::OK;
						break;
					}
					else
					{
						_eventGroup.reset(_doneFlag);
					}
				}
				else
				{
					// TODO: no possible to reach here
					rst = Result::StatusReserved;
					break;
				}
			}
			else
			{
				rst = Result::Timeout;
				break;
			}
		}
		if (!config.disableAutoReset)
		{
			_eventGroup.reset(_doneFlag | _errorFlag);
		}
		return rst;
	};

	bool WaitHandler::is_busy()
	{
		uint32_t events;
		auto rst = _eventGroup.wait(_doneFlag | _errorFlag, events,
			EventOptions_WaitForAny | EventOptions_NoClear, 0);
		if (rst == Result::OK)
		{
			return false;
		}
		else
		{
			return true;
		}
	};

	Result WaitHandler::reset()
	{
		if (is_busy())
		{
			return Result::Busy;
		}
		_eventGroup.reset(_doneFlag | _errorFlag);
		return Result::OK;
	};

	void WaitHandler::done_set(void* sender)
	{
		_sender = sender;
		if (_onDone)
		{
			_onDone(_sender, _value, _receiver);
		}
		_eventGroup.set(_doneFlag);
	};

	void WaitHandler::error_set(void* sender)
	{
		_sender = sender;
		if (_onError)
		{
			_onError(_sender, _value, _receiver);
		}
		_eventGroup.set(_errorFlag);
	};

	uint32_t WaitHandler::scope_begin()
	{
		return ++_level;
	};

	void WaitHandler::scope_end()
	{
		--_level;
	};

	uint32_t WaitHandler::scope_get()
	{
		return _level;
	};

} // namespace wibot
