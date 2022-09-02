#ifndef __WWDEVICE_PERIPHERAL_PIN_HPP__
#define __WWDEVICE_PERIPHERAL_PIN_HPP__

#include "base.hpp"
#include "peripheral.hpp"

namespace ww::peripheral
{

PIN_PER_DECL
enum PinStatus : uint32_t
{
    PinStatus_Reset = 0U,
    PinStatus_Set = 1U,
};

enum PinMode : uint8_t
{
    PinMode_Input = 0U,
    PinMode_Output = 1U,
};
union PinConfig {
    struct
    {
        bool inverse : 1;
        uint32_t : 31;
    };
    uint32_t value;
};

class Pin : public Initializable
{
  public:
    Pin(PIN_CTOR_ARG, uint16_t pinMask);
    ~Pin();
    PinConfig &config_get();
    Result read(PinStatus &value);
    Result write(PinStatus value);

    Result toggle();
    Result mode_set(PinMode mode);

  private:
    PIN_FIELD_DECL;
    uint16_t _pinMask;
    PinConfig _config;
};
} // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_PIN_HPP__