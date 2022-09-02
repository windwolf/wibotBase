#ifndef __WWDEVICE_PERIPHERAL_MISC_HPP__
#define __WWDEVICE_PERIPHERAL_MISC_HPP__

#include "stdint.h"

namespace ww::peripheral
{
class Misc
{
  public:
    static void us_delay(uint32_t us);
    static void ms_delay(uint32_t ms);
    static uint32_t get_tick();
};
} // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_MISC_HPP__