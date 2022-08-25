#ifndef __WWDEVICE_PERIPHERAL_PIN_HPP__
#define __WWDEVICE_PERIPHERAL_PIN_HPP__

#include "base.hpp"
#include "stm32f1xx_hal.h"

namespace ww::peripheral
{
union PinConfig {
    struct
    {
        bool inverse : 1;
        uint32_t : 31;
    };
    uint32_t value;
};

class Pin
{
  public:
    enum Status : uint32_t
    {
        STATUS_RESET = 0U,
        STATUS_SET = 1U,
    };

    enum Mode : uint8_t
    {
        MODE_INPUT = 0U,
        MODE_OUTPUT = 1U,
    };

    Pin(GPIO_TypeDef &port, uint16_t pinMask)
        : _port(port), _pinMask(pinMask){};
    ~Pin(){};
    PinConfig &config_get();
    Result init();
    Result deinit();

    Result read(Status &value);
    Result write(Status value);

    Result toggle();
    Result mode_set(Mode mode);

  private:
    GPIO_TypeDef &_port;
    uint16_t _pinMask;
    PinConfig _config;
};
} // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_PIN_HPP__