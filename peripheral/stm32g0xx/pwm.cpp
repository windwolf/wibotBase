#include "pwm.hpp"

namespace ww::peripheral
{

PwmConfig &Pwm::config_get()
{
    return _config;
};
Result Pwm::init()
{
    return Result_OK;
};
Result Pwm::deinit()
{
    return Result_OK;
};

Result Pwm::start()
{
    LL_TIM_CC_EnableChannel(_handle.Instance, _config.channelsEnable);

    return Result_OK;
};

Result Pwm::stop()
{
    LL_TIM_CC_DisableChannel(_handle.Instance, _config.channelsEnable);

    return Result_OK;
};

Result Pwm::duty_set(PwmChannel channels, uint16_t duty)
{
    uint32_t ar = LL_TIM_GetAutoReload(_handle.Instance);

    if (channels && (TimerChannel_1 | TimerChannel_1P | TimerChannel_1N))
    {
        LL_TIM_OC_SetCompareCH1(_handle.Instance,
                                ar * duty / _config.channelsEnable);
    }
    if (channels && (TimerChannel_2 | TimerChannel_2P | TimerChannel_2N))
    {
        LL_TIM_OC_SetCompareCH2(_handle.Instance,
                                ar * duty / _config.channelsEnable);
    }
    if (channels && (TimerChannel_3 | TimerChannel_3P | TimerChannel_3N))
    {
        LL_TIM_OC_SetCompareCH3(_handle.Instance,
                                ar * duty / _config.channelsEnable);
    }
    if (channels && (TimerChannel_4))
    {
        LL_TIM_OC_SetCompareCH4(_handle.Instance,
                                ar * duty / _config.channelsEnable);
    }

    return Result_OK;
};

} // namespace ww::peripheral