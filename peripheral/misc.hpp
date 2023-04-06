#ifndef __WWDEVICE_PERIPHERAL_MISC_HPP__
#define __WWDEVICE_PERIPHERAL_MISC_HPP__

#include "stdint.h"

namespace wibot::peripheral {
class Misc {
   public:
    static void     us_delay(uint32_t us);
    static void     ms_delay(uint32_t ms);
    static uint32_t get_tick_ms();
    static uint64_t get_tick_us();
    static uint64_t get_tick_ns();
};
}  // namespace wibot::peripheral

#endif  // __WWDEVICE_PERIPHERAL_MISC_HPP__
