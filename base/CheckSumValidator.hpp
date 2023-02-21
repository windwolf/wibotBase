//
// Created by zhouj on 2023/2/20.
//

#ifndef AQ_DJ_2022_LIBS_WIBOTTALK_BASIC_CHECKSUMVALIDATOR_HPP_
#define AQ_DJ_2022_LIBS_WIBOTTALK_BASIC_CHECKSUMVALIDATOR_HPP_
#include "base.hpp"

namespace wibot
{

    class CheckSum8Validator
    {
     public:
        void reset();
        void calculate(uint8_t* data, uint32_t length);
        bool validate(uint8_t* sum, uint32_t length);
     private:
        uint8_t sum_;
    };

} // wibot

#endif //AQ_DJ_2022_LIBS_WIBOTTALK_BASIC_CHECKSUMVALIDATOR_HPP_
