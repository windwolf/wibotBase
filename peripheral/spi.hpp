#ifndef __WWDEVICE_PERIPHERAL_SPI_HPP__
#define __WWDEVICE_PERIPHERAL_SPI_HPP__

#include "base.hpp"
#include "peripheral.hpp"
#include "pin.hpp"

namespace ww::peripheral
{
#define PWM_PER_DECL

union SpiConfig {
    struct
    {
        DataWidth dataWidth : 2;
        bool useTxDma : 1;
        bool useRxDma : 1;
        uint8_t dummyCycles : 4;
        uint8_t txDmaThreshold : 8;
        uint8_t rxDmaThreshold : 8;
        uint32_t : 8;
    };
    uint32_t value;
};

class Spi : public Initializable
{
  public:
    Spi(SPI_CTOR_ARG);
    Result _init() override;
    void _deinit() override;
    SpiConfig &config_get();

    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);

  private:
    SPI_FIELD_DECL
    union {
        struct
        {
            bool isTxDmaEnabled : 1;
            bool isRxDmaEnabled : 1;
        };
        uint32_t value;
    } _status;
    SpiConfig _config;
    WaitHandler *_readWaitHandler;
    WaitHandler *_writeWaitHandler;
    Buffer _txBuffer;
    Buffer _rxBuffer;

  protected:
    static void _on_read_complete_callback(SPI_CALLBACK_ARG);
    static void _on_write_complete_callback(SPI_CALLBACK_ARG);
    static void _on_error_callback(SPI_CALLBACK_ARG);
};

union SpiWithPinsConfig {
    struct
    {
        bool autoCs : 1;
        bool dcPinHighIsCmd : 1;
        bool rwPinHighIsWrite : 1;
        bool csPinHighIsDisable : 1;

        uint32_t : 28;
    };
    uint32_t value;
};

class SpiWithPins : public Spi
{
  public:
    SpiWithPins(SPI_CTOR_ARG, Pin *cs, Pin *rw, Pin *dc);
    Result _init() override;
    void _deinit() override;
    SpiWithPinsConfig &pinconfig_get();
    Result read(bool isData, void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(bool isData, void *data, uint32_t size, WaitHandler &waitHandler);
    Result session_begin();
    Result session_end();

  private:
    SPI_FIELD_DECL
    Pin *_cs;
    Pin *_rw;
    Pin *_dc;
    SpiWithPinsConfig _config;
    union {
        uint8_t value;
        struct
        {
            uint8_t busy : 1;
        };
    } _status;

    void cs_set(bool isEnable);
    void dc_set(bool isData);
    void rw_set(bool isRead);
    static void _on_read_complete_callback(SPI_CALLBACK_ARG);
    static void _on_write_complete_callback(SPI_CALLBACK_ARG);
};
} // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_SPI_HPP__