#include "os.hpp"

namespace wibot::os
{
uint32_t Utils::tick_diff(uint32_t tick)
{
    return Utils::tick_get() - tick;
};

    uint32_t EventGroup::fetch_empty_flag()
    {
        return __sync_add_and_fetch(&used_flags_, 1);
    };

    bool EventGroup::isFlagOverflow()
    {
        return used_flags_ > 32;
    }


} // namespace wibot::os
