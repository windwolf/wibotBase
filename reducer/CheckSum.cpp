//
// Created by zhouj on 2023/2/20.
//

#include "CheckSum.hpp"

namespace wibot {

void CheckSum8Validator::reset() {
    sum_ = 0;
}

void CheckSum8Validator::calculate(uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        sum_ += data[i];
    }
}

bool CheckSum8Validator::validate(uint8_t* sum) {
    return sum[0] == sum_;
}

}  // namespace wibot
