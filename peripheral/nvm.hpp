//
// Created by zhouj on 2023/3/24.
//

#ifndef WWMOTOR_LIBS_WWBASE_PERIPHERAL_NVM_HPP_
#define WWMOTOR_LIBS_WWBASE_PERIPHERAL_NVM_HPP_

#include "base.hpp"
#include "peripheral.hpp"

namespace wibot::peripheral {
struct NvmConfig {
    uint32_t address;
    uint32_t size;
};

class Nvm : public Initializable, public Configurable<NvmConfig> {
   public:
    Nvm(NVM_CTOR_ARG);
    Result _init() override;
    void   _deinit() override;

    Result read(uint8_t* data, uint32_t size);
    Result write(uint8_t* data, uint32_t size);
    Result erase(uint32_t size);

   private:
    NVM_FIELD_DECL;
};
}  // namespace wibot::peripheral

#endif  // WWMOTOR_LIBS_WWBASE_PERIPHERAL_NVM_HPP_
