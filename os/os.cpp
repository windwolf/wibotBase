#include "os.hpp"
#include "cmsis_gcc.h"

namespace wibot::os
{
uint32_t Utils::tick_diff(uint32_t tick)
{
    return Utils::tick_get() - tick;
};

    uint32_t EventGroup::fetch_empty_flag()
    {
        __sync_fetch_and_add(&used_flags_, 1);
        auto event_flags = __LDREXB(&used_flags_)+1;
        __STREXB(event_flags, &used_flags_);
        return 0;
    };


} // namespace wibot::os
