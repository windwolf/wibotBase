#include "pin.hpp"
#include "stm32g0xx_ll_gpio.h"

namespace ww::peripheral
{

Pin::Pin(GPIO_TypeDef &port, uint16_t pinMask) : _port(port), _pinMask(pinMask){};

Result Pin::_init()
{
    return Result::OK;
};
void Pin::_deinit(){};

PinConfig &Pin::config_get()
{
    return _config;
};

Result Pin::read(PinStatus &value)
{
    PinStatus rst;
    rst = static_cast<PinStatus>(HAL_GPIO_ReadPin(&_port, this->_pinMask));
    value = static_cast<PinStatus>(to_underlying(rst) ^ this->_config.inverse);
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
} // namespace ww::peripheral