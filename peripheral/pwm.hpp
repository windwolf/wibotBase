#ifndef __WWDEVICE_PERIPHERAL_TIM_HPP__
#define __WWDEVICE_PERIPHERAL_TIM_HPP__

#include "base.hpp"
#include "peripheral.hpp"

namespace ww::peripheral
{
#define PWM_PER_DECL
typedef uint32_t PwmChannel;
#define TimerChannel_1 LL_TIM_CHANNEL_CH1
#define TimerChannel_1P LL_TIM_CHANNEL_CH1
#define TimerChannel_1N LL_TIM_CHANNEL_CH1N
#define TimerChannel_2 LL_TIM_CHANNEL_CH2
#define TimerChannel_2P LL_TIM_CHANNEL_CH2
#define TimerChannel_2N LL_TIM_CHANNEL_CH2N
#define TimerChannel_3 LL_TIM_CHANNEL_CH3
#define TimerChannel_3P LL_TIM_CHANNEL_CH3
#define TimerChannel_3N LL_TIM_CHANNEL_CH3N
#define TimerChannel_4 LL_TIM_CHANNEL_CH4
#define TimerChannel_4P
#define TimerChannel_4N

struct PwmConfig
{

    PwmChannel channelsEnable;

    uint16_t fullScaleDuty;
};

class Pwm : public Initializable
{
  public:
    Pwm(PWM_CTOR_ARG);
    Result _init() override;
    void _deinit() override;
    PwmConfig &config_get();

    Result start();
    Result stop();
    Result duty_set(PwmChannel channel, uint16_t duty);

  private:
    PWM_FIELD_DECL
    PwmConfig _config;
};
} // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_TIM_HPP__