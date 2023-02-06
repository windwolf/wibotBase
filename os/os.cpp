#include "os.hpp"
#include "arch.hpp"

#define LOG_MODULE "os"
#include "log.h"

namespace wibot::os
{
    uint32_t Utils::tick_diff(uint32_t tick)
    {
        return Utils::tick_get() - tick;
    };

    EventFlag EventGroup::fetch_flag()
    {
        uint32_t of, cf;
        do
        {
            of = used_flags_;
            cf = 1;
            while (of & cf)
            {
                if (cf == 0x80000000)
                {
                    cf = 0;
                    break;
                }
                else
                {
                    cf <<= 1;
                }
            }
            if (cf == 0)
            {
                return cf;
            }
        } while (!arch::sync_compare_and_swap(&used_flags_, of, of | cf));
        LOG_D("eg:%s flags:%#010x", name_, cf);
        return static_cast<EventFlag>(cf);
    };

    void EventGroup::release_flag(EventFlag flag)
    {
        uint32_t of, cf;
        do
        {
            of = used_flags_;
            cf = of & ~flag;
        } while (!arch::sync_compare_and_swap(&used_flags_, of, cf));
    };

} // namespace wibot::os
