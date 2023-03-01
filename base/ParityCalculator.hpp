//
// Created by zhouj on 2023/2/21.
//

#ifndef BASE_CHECKPARITYVALIDATOR_HPP_
#define BASE_CHECKPARITYVALIDATOR_HPP_
#include "base.hpp"

namespace wibot
{

    class ParityCalculator
    {
     public:
        /**
         * @brief Construct a new Check Parity Validator object
         * @param odd 是否偶校验
         */
        ParityCalculator(bool even = false) : even_(even)
        {
        };
        void reset();
        void calculate(uint8_t* data, uint32_t length);
        bool validate();
     private:
        uint8_t parity_; //
        bool even_; // 是否偶校验
    };

} // wibot

#endif //BASE_CHECKPARITYVALIDATOR_HPP_
