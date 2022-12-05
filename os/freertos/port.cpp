#include "cmsis_os2.h"
#include "os.hpp"

namespace wibot::os
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

	Mutex::Mutex(const char* name) : _name(name)
	{
		osMutexAttr_t attr = {
			.name = _name,
			.cb_mem = &(this->_instance),
			.cb_size = sizeof(this->_instance),

		};
		osMutexNew(&attr);
	};

	Mutex::~Mutex()
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

	EventGroup::EventGroup(const char* name) : _name(name)
	{

		osEventFlagsAttr_t attr = {
			.name = _name,
			.cb_mem = &(this->_instance),
			.cb_size = sizeof(this->_instance),
		};

		osEventFlagsNew(&attr);
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
			return Result::OK;
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
			return Result::OK;
		}
	};

	Result EventGroup::wait(uint32_t flags, uint32_t& actualFlags, EventOptions options,
		uint32_t timeout)
	{
		uint32_t freertosOptions = 0;
		if ((options & EventOptions_Clear_Flag) == EventOptions_NoClear)
		{
			freertosOptions |= osFlagsNoClear;
		}
		if ((options & EventOptions_Wait_Flag) == EventOptions_WaitForAll)
		{
			freertosOptions |= osFlagsWaitAll;
		}
		//TODO: HANDLE options
		auto rst = osEventFlagsWait(&(this->_instance), flags, freertosOptions, timeout);
		if ((int32_t)rst < 0)
		{
			return (Result)rst;
		}
		else
		{
			actualFlags = rst;
			return Result::OK;
		}
	};
} // namespace wibot::os
