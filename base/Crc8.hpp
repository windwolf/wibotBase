//
// Created by zhouj on 2023/2/21.
//

#ifndef BASE_CRC8_HPP_
#define BASE_CRC8_HPP_

#include "base.hpp"

namespace wibot
{

    class Crc8
    {
     public:
        Crc8(uint8_t poly,
            uint8_t init = 0x00,
            uint8_t xorout = 0x00,
            bool ref_in = false,
            bool ref_out = false) : poly_(poly), init_(init), xorout_(xorout), ref_in_(ref_in), ref_out_(ref_out)
        {
        };
        void reset();
        void calculate(uint8_t* data, uint32_t length);
        bool validate(uint8_t sum);
     private:
        uint8_t poly_;
        uint8_t init_;
        uint8_t xorout_;
        bool ref_in_;
        bool ref_out_;
        uint8_t crc_;

    };

} // wibot

#endif // BASE_CRC8_HPP_
