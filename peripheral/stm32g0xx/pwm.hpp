#ifndef __WWDEVICE_PERIPHERAL_TIM_HPP__
#define __WWDEVICE_PERIPHERAL_TIM_HPP__

#include "base.hpp"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_tim.h"

namespace ww::peripheral
{

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

class Pwm
{
  public:
    Pwm(TIM_HandleTypeDef &handle) : _handle(handle){};
    ~Pwm(){};
    PwmConfig &config_get();
    Result init();
    Result deinit();

    Result start();
    Result stop();
    Result duty_set(PwmChannel channel, uint16_t duty);

  private:
    TIM_HandleTypeDef &_handle;
    PwmConfig _config;
};
} // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_TIM_HPP__