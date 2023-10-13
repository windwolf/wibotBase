#include "pin.hpp"

#include "perip_port.hpp"

namespace wibot::peripheral {

Pin::Pin(GPIO_TypeDef& port, uint16_t pinMask) : _port(port), _pinMask(pinMask){};

Result Pin::_init() {
    return Result::OK;
};
void Pin::_deinit(){};

Result Pin::read(PinStatus& value) {
    PinStatus curr = static_cast<PinStatus>(HAL_GPIO_ReadPin(&_port, this->_pinMask));
    if (config.enable_debounce) {
        if (curr != last_buffered_status_) {
            last_buffered_status_ = curr;
            last_debounce_time_   = HAL_GetTick();

        } else {
            if (HAL_GetTick() - last_debounce_time_ > this->config.debounce_time) {
                last_output_status_ = static_cast<PinStatus>(to_underlying(last_buffered_status_) ^ this->config.inverse);
            }
        }
        value = last_output_status_;
    } else {
        value = static_cast<PinStatus>(to_underlying(curr) ^ this->config.inverse);
    }
    return Result::OK;
};


bool Pin::read() {
    PinStatus sta;
    read(sta);
    return sta == PinStatus::Set;
}

Result Pin::write(PinStatus value) {
    HAL_GPIO_WritePin(&_port, this->_pinMask,
                      (GPIO_PinState)(to_underlying(value) ^ this->config.inverse));
    return Result::OK;
};

Result Pin::toggle() {
    HAL_GPIO_TogglePin(&_port, this->_pinMask);
    return Result::OK;
};

Result Pin::mode_set(PinMode mode) {
    LL_GPIO_SetPinMode(&_port, this->_pinMask,
                       (mode == PinMode::Input) ? LL_GPIO_MODE_INPUT : LL_GPIO_MODE_OUTPUT);
    return Result::OK;
};
}  // namespace wibot::peripheral
