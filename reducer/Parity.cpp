//
// Created by zhouj on 2023/2/21.
//

#include "Parity.hpp"

namespace wibot {
void ParityValidator::reset() {
    parity_ = 0;
}
void ParityValidator::calculate(uint8_t* data, uint32_t length) {
    for (uint32_t i = 0; i < length; i++) {
        parity_ = parity_ ^ data[i];
    }
}
bool ParityValidator::validate(uint8_t* sum) {
    bool parity = even_;
    while (parity_) {
        parity = !parity;
        parity_ &= parity_ - 1;
    }
    return parity;
}
}  // namespace wibot
