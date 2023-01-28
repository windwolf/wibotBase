#include "adc.hpp"
#include "misc.hpp"
#include "os.hpp"

#ifdef HAL_ADC_MODULE_ENABLED

namespace wibot::peripheral
{

    Adc::Adc(ADC_HandleTypeDef& handle) : _handle(handle)
    {
    };

    Adc::~Adc()
    {
    };

    Result Adc::_init()
    {
        HAL_ADC_RegisterCallback(&_handle, HAL_ADC_CONVERSION_COMPLETE_CB_ID,
            &Adc::_on_conversion_complete_callback);
        HAL_ADC_RegisterCallback(&_handle, HAL_ADC_ERROR_CB_ID, &Adc::_on_error_callback);
        Peripherals::register_peripheral("adc", this, &_handle);
        return Result::OK;
    };
    void Adc::_deinit()
    {
        Peripherals::unregister_peripheral("adc", this);
    };

//Result Adc::read(Buffer32 buffer)
//{
//    return Result::NotSupport;
//};

    Result Adc::start(Buffer32 buffer, WaitHandler& waitHandler)
    {
        if (_waitHandler != nullptr)
        {
            return Result::Busy;
        }
        _buffer = buffer;
        _waitHandler = &waitHandler;
        return (Result)HAL_ADC_Start_DMA(&_handle, (uint32_t*)buffer.data, buffer.size);
    };

    Result Adc::stop()
    {
        _waitHandler = nullptr;
        return (Result)HAL_ADC_Stop_DMA(&_handle);
    };

    void Adc::_on_conversion_complete_callback(ADC_HandleTypeDef* instance)
    {
        Adc* perip = (Adc*)Peripherals::get_peripheral(instance);
        auto wh = perip->_waitHandler;
        if (wh != nullptr)
        {
            wh->done_set(perip);
        }
    };
    void Adc::_on_error_callback(ADC_HandleTypeDef* instance)
    {
        Adc* perip = (Adc*)Peripherals::get_peripheral(instance);
        auto wh = perip->_waitHandler;
        if (wh != nullptr)
        {
            perip->_waitHandler = nullptr;
            wh->done_set(perip);
        }
        wh = perip->_waitHandler;
        if (wh != nullptr)
        {
            perip->_waitHandler = nullptr;
            wh->done_set(perip);
        }
    };

} // namespace wibot::peripheral

#endif
