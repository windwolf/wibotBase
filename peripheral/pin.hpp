#ifndef __WWDEVICE_PERIPHERAL_PIN_HPP__
#define __WWDEVICE_PERIPHERAL_PIN_HPP__

#include "base.hpp"
#include "peripheral.hpp"

namespace wibot::peripheral
{

PIN_PER_DECL
enum class PinStatus : uint32_t
{
    Reset = 0U,
    Set = 1U,
};

enum class PinMode : uint8_t
{
    Input = 0U,
    Output = 1U,
};
union PinConfig {
    struct
    {
        bool inverse : 1;
        bool enable_debounce : 1;
        uint8_t : 6;
        /**
         * Debounce time in ms.
         */
        uint8_t debounce_time : 8;
        uint32_t : 16;
    };
    uint32_t value;

};

class Pin : public Initializable, public Configurable<PinConfig>
{
  public:
    Pin(PIN_CTOR_ARG, uint16_t pinMask);
    Result _init() override;
    void _deinit() override;

    Result read(PinStatus &value);
    Result write(PinStatus value);

    Result toggle();
    Result mode_set(PinMode mode);

  private:
    PIN_FIELD_DECL;
    uint16_t _pinMask;
    PinStatus last_output_status_ = PinStatus::Reset;
    PinStatus last_buffered_status_ = PinStatus::Reset;
    uint32_t last_debounce_time_ = 0;
};
} // namespace wibot::peripheral

#endif // __WWDEVICE_PERIPHERAL_PIN_HPP__
