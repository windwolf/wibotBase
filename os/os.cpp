#include "os.hpp"

namespace wibot::os
{
uint32_t Utils::tick_diff(uint32_t tick)
{
    return Utils::tick_get() - tick;
};

} // namespace wibot::os
