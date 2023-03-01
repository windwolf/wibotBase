//
// Created by zhouj on 2023/2/20.
//

#include "CheckSum8Calculator.hpp"

namespace wibot
{
    void CheckSum8Calculator::reset()
    {
        sum_ = 0;

    }
    void CheckSum8Calculator::calculate(uint8_t* data, uint32_t length)
    {
        for (uint32_t i = 0; i < length; i++)
        {
            sum_ += data[i];
        }
    }

    bool CheckSum8Calculator::validate(uint8_t* sum, uint32_t length)
    {
        return sum[0] == sum_;
    }
} // wibot
