#ifndef __WWBASE_CODEX_HPP__
#define __WWBASE_CODEX_HPP__

#include "base.hpp"
namespace ww {
class Codex {
  public:
    static uint8_t byte_to_bcd(uint8_t Value);

    static uint8_t bcd_to_byte(uint8_t Value);
};
} // namespace ww

#endif // __WWBASE_CODEX_HPP__