//
// Created by zhouj on 2023/2/20.
//

#ifndef BASE_CHECKSUMVALIDATOR_HPP_
#define BASE_CHECKSUMVALIDATOR_HPP_
#include "base.hpp"

namespace wibot
{

    class CheckSum8Calculator
    {
     public:
        void reset();
        void calculate(uint8_t* data, uint32_t length);
        bool validate(uint8_t* sum, uint32_t length);
     private:
        uint8_t sum_;
    };

} // wibot

#endif //BASE_CHECKSUMVALIDATOR_HPP_
