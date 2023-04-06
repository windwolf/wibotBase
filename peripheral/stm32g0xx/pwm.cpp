#include "pwm.hpp"

#include "stm32g0xx_ll_tim.h"

#ifdef HAL_TIM_MODULE_ENABLED

namespace wibot::peripheral {

Pwm::Pwm(TIM_HandleTypeDef &handle) : _handle(handle){};

Result Pwm::_init() {
    return Result::OK;
};
void Pwm::_deinit(){};

Result Pwm::all_enable() {
    init();
    LL_TIM_CC_EnableChannel(_handle.Instance, config.channelsEnable);

    return Result::OK;
};

Result Pwm::all_disable() {
    LL_TIM_CC_DisableChannel(_handle.Instance, config.channelsEnable);

    return Result::OK;
};

Result Pwm::duty_set(PwmChannel channels, uint16_t duty) {
    uint32_t period = LL_TIM_GetAutoReload(_handle.Instance) + 1;

    if (channels & (PwmChannel_1 | PwmChannel_1P | PwmChannel_1N)) {
        LL_TIM_OC_SetCompareCH1(_handle.Instance, period * duty / config.fullScaleDuty);
    }
    if (channels & (PwmChannel_2 | PwmChannel_2P | PwmChannel_2N)) {
        LL_TIM_OC_SetCompareCH2(_handle.Instance, period * duty / config.fullScaleDuty);
    }
    if (channels & (PwmChannel_3 | PwmChannel_3P | PwmChannel_3N)) {
        LL_TIM_OC_SetCompareCH3(_handle.Instance, period * duty / config.fullScaleDuty);
    }
    if (channels & (PwmChannel_4)) {
        LL_TIM_OC_SetCompareCH4(_handle.Instance, period * duty / config.fullScaleDuty);
    }

    return Result::OK;
};

Result Pwm::channel_enable(PwmChannel channels) {
    LL_TIM_CC_EnableChannel(_handle.Instance, config.channelsEnable);
    return Result::OK;
};

Result Pwm::channel_disable(PwmChannel channels) {
    LL_TIM_CC_DisableChannel(_handle.Instance, config.channelsEnable);
    return Result::OK;
};

}  // namespace wibot::peripheral

#endif  // HAL_TIM_MODULE_ENABLED
