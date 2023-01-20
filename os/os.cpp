#include "os.hpp"

namespace wibot::os
{
uint32_t Utils::tick_diff(uint32_t tick)
{
    return Utils::tick_get() - tick;
};

    EventFlag EventGroup::fetch_flag()
    {
        uint32_t of, cf;
        do {
            of = used_flags_;
            cf = 1;
            while (of & cf) {
                if (cf == 32) {
                    return 0;
                }
                cf <<= 1;
            }
        } while(!__sync_bool_compare_and_swap(&used_flags_, of, of | cf));
        return static_cast<EventFlag>(cf);
    };

    void EventGroup::release_flag(EventFlag flag) {
        uint32_t of, cf;
        do {
            of = used_flags_;
            cf = of & ~flag;
        } while(!__sync_bool_compare_and_swap(&used_flags_, of, cf));
    };

} // namespace wibot::os
