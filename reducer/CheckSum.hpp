//
// Created by zhouj on 2023/2/20.
//

#ifndef BASE_CHECKSUMVALIDATOR_HPP_
#define BASE_CHECKSUMVALIDATOR_HPP_
#include "Validator.hpp"
#include "base.hpp"

namespace wibot {

class CheckSum8Validator : Validator<uint8_t> {
   public:
    void reset() override;
    void calculate(uint8_t* data, uint32_t length) override;
    bool validate(uint8_t* sum) override;

   private:
    uint8_t sum_;
};

}  // namespace wibot

#endif  // BASE_CHECKSUMVALIDATOR_HPP_
