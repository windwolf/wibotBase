#ifndef __WWBASE_ADC_HPP__
#define __WWBASE_ADC_HPP__

#include "buffer.hpp"
#include "peripheral.hpp"

namespace wibot::peripheral {

ADC_PER_DECL

union AdcConfig {
    struct {
        uint8_t channelCount;
        uint8_t bitWidths;
    };
    uint32_t value;
};

class Adc : public Initializable, public Configurable<AdcConfig> {
   public:
    Adc(ADC_CTOR_ARG);
    ~Adc();
    Result _init() override;
    void   _deinit() override;

    Result read(Buffer32 buffer);
    Result start(Buffer32 buffer, WaitHandler& waitHandler);
    Result stop();

   private:
    ADC_FIELD_DECL
    union {
        struct {
            bool isRunning : 1;
        };
        uint32_t value;
    } _status;
    WaitHandler* _waitHandler;
    Buffer32     _buffer;
    static void  _on_conversion_complete_callback(ADC_CALLBACK_ARG);
    static void  _on_error_callback(ADC_CALLBACK_ARG);
};

};      // namespace wibot::peripheral

#endif  // __WWBASE_ADC_HPP__
