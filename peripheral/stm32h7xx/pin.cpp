#include "pin.hpp"
#include "stm32h7xx_ll_gpio.h"

namespace wibot::peripheral
{

Pin::Pin(GPIO_TypeDef &port, uint16_t pinMask) : _port(port), _pinMask(pinMask){};

Result Pin::_init()
{
    return Result::OK;
};
void Pin::_deinit(){};


Result Pin::read(PinStatus &value)
{
    PinStatus rst;
    rst = static_cast<PinStatus>(HAL_GPIO_ReadPin(&_port, this->_pinMask));
    value = static_cast<PinStatus>(to_underlying(rst) ^ this->config.inverse);
    if (config.enable_debounce) {
        if (rst != last_buffered_status_) {
            last_buffered_status_ = rst;
            last_debounce_time_ = HAL_GetTick();
        }
        else {
            if (HAL_GetTick() - last_debounce_time_ > this->config.debounce_time) {
                last_output_status_ = rst;
            }
        }
        value = last_output_status_;
    }
    else {
        value = rst;
    }
    return Result::OK;
};

Result Pin::write(PinStatus value)
{
    HAL_GPIO_WritePin(&_port, this->_pinMask, (GPIO_PinState)(to_underlying(value) ^ this->_config.inverse));
    return Result::OK;
};

Result Pin::toggle()
{
    HAL_GPIO_TogglePin(&_port, this->_pinMask);
    return Result::OK;
};

Result Pin::mode_set(PinMode mode)
{
    LL_GPIO_SetPinMode(&_port, this->_pinMask,
                       (mode == PinMode::Input) ? LL_GPIO_MODE_INPUT : LL_GPIO_MODE_OUTPUT);
    return Result::OK;
};
} // namespace wibot::peripheral
