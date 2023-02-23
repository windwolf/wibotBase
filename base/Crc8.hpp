//
// Created by zhouj on 2023/2/21.
//

#ifndef AQ_DJ_2022_LIBS_WIBOTBASE_BASE_CRC8_HPP_
#define AQ_DJ_2022_LIBS_WIBOTBASE_BASE_CRC8_HPP_

#include "base.hpp"

namespace wibot
{

    class Crc8
    {
     public:
        Crc8(uint8_t poly = 0x31, uint8_t init = 0x00, uint8_t xorout = 0x00, bool ref_in = false, bool ref_out = false);
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

#endif //AQ_DJ_2022_LIBS_WIBOTBASE_BASE_CRC8_HPP_
