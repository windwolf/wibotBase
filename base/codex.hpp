#ifndef __WWBASE_CODEX_HPP__
#define __WWBASE_CODEX_HPP__

#include "base.hpp"
namespace wibot
{
    class Codex
    {
     public:
        static uint8_t byte_to_bcd(uint8_t Value);

        static uint8_t bcd_to_byte(uint8_t Value);
    };
} // namespace wibot

#endif // __WWBASE_CODEX_HPP__
