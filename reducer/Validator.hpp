//
// Created by zhouj on 2023/4/1.
//

#ifndef WWMOTOR_LIBS_WWBASE_CHECKSUM_CHECKSUM8_HPP_
#define WWMOTOR_LIBS_WWBASE_CHECKSUM_CHECKSUM8_HPP_

#include "base.hpp"

namespace wibot {

template <typename TE>
class Validator {
   public:
    virtual void reset() = 0;
    virtual void calculate(TE* data, uint32_t length) = 0;
    virtual bool validate(TE* sum) = 0;
};

}  // namespace wibot

#endif  // WWMOTOR_LIBS_WWBASE_CHECKSUM_CHECKSUM8_HPP_
