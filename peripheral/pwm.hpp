#ifndef __WWDEVICE_PERIPHERAL_TIM_HPP__
#define __WWDEVICE_PERIPHERAL_TIM_HPP__

#include "base.hpp"
#include "peripheral.hpp"

namespace wibot::peripheral
{
#define PWM_PER_DECL
	typedef uint32_t PwmChannel;
#define PwmChannel_1 LL_TIM_CHANNEL_CH1
#define PwmChannel_1P LL_TIM_CHANNEL_CH1
#define PwmChannel_1N LL_TIM_CHANNEL_CH1N
#define PwmChannel_2 LL_TIM_CHANNEL_CH2
#define PwmChannel_2P LL_TIM_CHANNEL_CH2
#define PwmChannel_2N LL_TIM_CHANNEL_CH2N
#define PwmChannel_3 LL_TIM_CHANNEL_CH3
#define PwmChannel_3P LL_TIM_CHANNEL_CH3
#define PwmChannel_3N LL_TIM_CHANNEL_CH3N
#define PwmChannel_4 LL_TIM_CHANNEL_CH4
#define PwmChannel_4P
#define PwmChannel_4N

	struct PwmConfig
	{

		PwmChannel channelsEnable;

		uint16_t fullScaleDuty;
	};

	class Pwm : public Initializable, public Configurable<PwmConfig>
	{
	 public:
		Pwm(PWM_CTOR_ARG);
		Result _init() override;
		void _deinit() override;

		Result all_enable();
		Result all_disable();
		Result duty_set(PwmChannel channel, uint16_t duty);
		Result channel_enable(PwmChannel channels);
		Result channel_disable(PwmChannel channels);

	 private:
		PWM_FIELD_DECL

	};
} // namespace wibot::peripheral

#endif // __WWDEVICE_PERIPHERAL_TIM_HPP__
