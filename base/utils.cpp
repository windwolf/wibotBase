//
// Created by zhouj on 2022/12/5.
//

#include "utils.hpp"
#include "misc.hpp"

namespace wibot
{
	void StopWatch::start()
	{
		_start_time = ::wibot::peripheral::Misc::get_tick_us();
	}
	uint64_t StopWatch::tick()
	{
		auto end_time = ::wibot::peripheral::Misc::get_tick_us();
		auto duration = end_time - _start_time;
		_start_time = end_time;
		return duration;
	}
} // wibot
