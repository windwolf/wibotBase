#include "os.hpp"

namespace ww::os
{
uint32_t Utils::tick_diff(uint32_t tick)
{
    return Utils::tick_get() - tick;
};

} // namespace ww::os
