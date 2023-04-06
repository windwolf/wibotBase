//
// Created by zhouj on 2023/2/21.
//

#ifndef BASE_CHECKPARITYVALIDATOR_HPP_
#define BASE_CHECKPARITYVALIDATOR_HPP_
#include "Validator.hpp"
#include "base.hpp"

namespace wibot {

class ParityValidator : Validator<uint8_t> {
   public:
    /**
     * @brief Construct a new Check Parity Validator object
     * @param odd 是否偶校验
     */
    explicit ParityValidator(bool even = false) : parity_(0), even_(even){};
    void reset() override;
    void calculate(uint8_t* data, uint32_t length) override;
    /**
     *
     * @param sum Not used.
     * @return
     */
    bool validate(uint8_t* sum) override;

   private:
    uint8_t parity_;  //
    bool even_;       // 是否偶校验
};

}  // namespace wibot

#endif  // BASE_CHECKPARITYVALIDATOR_HPP_
