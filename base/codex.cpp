#include "codex.hpp"

namespace wibot {
uint8_t Codex::byte_to_bcd(uint8_t Value) {
    uint32_t bcdhigh = 0U;
    uint8_t  Param   = Value;

    while (Param >= 10U) {
        bcdhigh++;
        Param -= 10U;
    }

    return ((uint8_t)(bcdhigh << 4U) | Param);
};
uint8_t Codex::bcd_to_byte(uint8_t Value) {
    uint32_t tmp;
    tmp = (((uint32_t)Value & 0xF0U) >> 4U) * 10U;
    return (uint8_t)(tmp + ((uint32_t)Value & 0x0FU));
};
}  // namespace wibot
