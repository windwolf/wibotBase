#include "pin.hpp"
#include "stm32f1xx_ll_gpio.h"

namespace ww::peripheral
{

Pin::Pin(GPIO_TypeDef &port, uint16_t pinMask) : _port(port), _pinMask(pinMask)
{
    initErrorCode = Result_OK;
};
Pin::~Pin(){};

PinConfig &Pin::config_get()
{
    return _config;
};

Result Pin::read(PinStatus &value)
{
    PinStatus rst;
    rst = (PinStatus)HAL_GPIO_ReadPin(&_port, this->_pinMask);
    value = (PinStatus)(rst ^ this->_config.inverse);
    return Result_OK;
};

Result Pin::write(PinStatus value)
{
    HAL_GPIO_WritePin(&_port, this->_pinMask, (GPIO_PinState)(value ^ this->_config.inverse));
    return Result_OK;
};

Result Pin::toggle()
{
    HAL_GPIO_TogglePin(&_port, this->_pinMask);
    return Result_OK;
};

Result Pin::mode_set(PinMode mode)
{
    LL_GPIO_SetPinMode(&_port, this->_pinMask,
                       (mode == PinMode_Input) ? LL_GPIO_MODE_INPUT : LL_GPIO_MODE_OUTPUT);
    return Result_OK;
};
} // namespace ww::peripheral