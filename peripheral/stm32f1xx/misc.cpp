#include "misc.hpp"

#include "stm32f1xx_hal.h"
namespace wibot::peripheral {

__STATIC_INLINE uint32_t LL_SYSTICK_IsActiveCounterFlag() {
    return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}

void Misc::us_delay(uint32_t us) {
    volatile uint32_t remain = us << 1;
    if (us < 1000) {
        while (remain--) {
            __NOP();
        }
    } else {
        ms_delay(us / 1000);
        remain = (us % 1000) << 1;
        while (remain--) {
            __NOP();
        }
    }
}
void Misc::ms_delay(uint32_t ms) {
    HAL_Delay(ms);
}

uint32_t Misc::get_tick_ms() {
    return HAL_GetTick();
};

uint64_t Misc::get_tick_us() {
    /* Ensure COUNTFLAG is reset by reading SysTick control and status register_instance */
    LL_SYSTICK_IsActiveCounterFlag();
    __IO uint32_t m   = HAL_GetTick();
    __IO uint32_t tms = SysTick->LOAD + 1;
    __IO uint32_t u   = tms - SysTick->VAL;
    if (LL_SYSTICK_IsActiveCounterFlag()) {
        m = HAL_GetTick();
        u = tms - SysTick->VAL;
    }
    return (m * 1000 + (u * 1000) / tms);
};

uint64_t Misc::get_tick_ns() {
    /* Ensure COUNTFLAG is reset by reading SysTick control and status register_instance */
    LL_SYSTICK_IsActiveCounterFlag();
    __IO uint32_t m   = HAL_GetTick();
    __IO uint32_t tms = SysTick->LOAD + 1;
    __IO uint32_t u   = tms - SysTick->VAL;
    if (LL_SYSTICK_IsActiveCounterFlag()) {
        m = HAL_GetTick();
        u = tms - SysTick->VAL;
    }
    return (m * 1000000 + (u * 1000000) / tms);
};
}  // namespace wibot::peripheral
