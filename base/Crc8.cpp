//
// Created by zhouj on 2023/2/21.
//

#include "Crc8.hpp"

namespace wibot
{

    void Crc8::reset()
    {
        crc_ = init_;
    }
    void Crc8::calculate(uint8_t* data, uint32_t length)
    {
        for (uint32_t i = 0; i < length; i++)
        {
            uint8_t d = data[i];
            if (ref_in_)
            {
                d = (d & 0xF0) >> 4 | (d & 0x0F) << 4;
                d = (d & 0xCC) >> 2 | (d & 0x33) << 2;
                d = (d & 0xAA) >> 1 | (d & 0x55) << 1;
            }
            crc_ ^= d;
            for (uint8_t j = 0; j < 8; j++)
            {
                if (crc_ & 0x80)
                {
                    crc_ = (crc_ << 1) ^ poly_;
                }
                else
                {
                    crc_ <<= 1;
                }
            }
        }
    }
    bool Crc8::validate(uint8_t sum)
    {
        if (ref_out_)
        {
            sum = (sum & 0xF0) >> 4 | (sum & 0x0F) << 4;
            sum = (sum & 0xCC) >> 2 | (sum & 0x33) << 2;
            sum = (sum & 0xAA) >> 1 | (sum & 0x55) << 1;
        }
        return sum == (crc_ ^ xorout_);
    }
//    void Crc8::init_table()
//    {
//        for (uint16_t i = 0; i < 256; i++)
//        {
//            uint8_t crc = i;
//            for (uint8_t j = 0; j < 8; j++)
//            {
//                if (crc & 0x80)
//                {
//                    crc = (crc << 1) ^ poly_;
//                }
//                else
//                {
//                    crc <<= 1;
//                }
//            }
//            table_[i] = crc;
//        }
//    }
} // wibot
