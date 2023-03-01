//
// Created by zhouj on 2023/2/21.
//

#include "ParityCalculator.hpp"

namespace wibot
{
    void ParityCalculator::reset()
    {
        parity_ = 0;

    }
    void ParityCalculator::calculate(uint8_t* data, uint32_t length)
    {
        for (uint32_t i = 0; i < length; i++)
        {
            parity_ = parity_ ^ data[i];
        }
    }
    bool ParityCalculator::validate()
    {
        bool parity = even_;
        while (parity_)
        {
            parity = !parity;
            parity_ &= parity_ - 1;
        }
        return parity;
    }
} // wibot
