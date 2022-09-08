#include "pwm.hpp"

#ifdef HAL_TIM_MODULE_ENABLED

namespace ww::peripheral
{

Pwm::Pwm(TIM_HandleTypeDef &handle) : _handle(handle){};

Result Pwm::_init()
{
    return Result::OK;
};
void Pwm::_deinit(){};

PwmConfig &Pwm::config_get()
{
    return _config;
};

Result Pwm::start()
{
    LL_TIM_CC_EnableChannel(_handle.Instance, _config.channelsEnable);

    return Result::OK;
};

Result Pwm::stop()
{
    LL_TIM_CC_DisableChannel(_handle.Instance, _config.channelsEnable);

    return Result::OK;
};

Result Pwm::duty_set(PwmChannel channels, uint16_t duty)
{
    uint32_t period = LL_TIM_GetAutoReload(_handle.Instance) + 1;

    if (channels & (TimerChannel_1 | TimerChannel_1P | TimerChannel_1N))
    {
        LL_TIM_OC_SetCompareCH1(_handle.Instance, period * duty / _config.fullScaleDuty);
    }
    if (channels & (TimerChannel_2 | TimerChannel_2P | TimerChannel_2N))
    {
        LL_TIM_OC_SetCompareCH2(_handle.Instance, period * duty / _config.fullScaleDuty);
    }
    if (channels & (TimerChannel_3 | TimerChannel_3P | TimerChannel_3N))
    {
        LL_TIM_OC_SetCompareCH3(_handle.Instance, period * duty / _config.fullScaleDuty);
    }
    if (channels & (TimerChannel_4))
    {
        LL_TIM_OC_SetCompareCH4(_handle.Instance, period * duty / _config.fullScaleDuty);
    }

    return Result::OK;
};

} // namespace ww::peripheral

#endif // HAL_TIM_MODULE_ENABLED