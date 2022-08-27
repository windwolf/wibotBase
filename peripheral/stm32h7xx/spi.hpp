#ifndef __WWDEVICE_PERIPHERAL_SPI_HPP__
#define __WWDEVICE_PERIPHERAL_SPI_HPP__

#include "base.hpp"
#include "pin.hpp"
#include "spi.hpp"
#include "wait_handler.hpp"
#include "stm32h7xx_hal.h"

#ifndef HAL_SPI_MODULE_ENABLED
#define SPI_HandleTypeDef uint32_t
#endif // HAL_SPI_MODULE_ENABLED

namespace ww::peripheral
{
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

class Spi
{
  public:
    Spi(SPI_HandleTypeDef &handle)
        : _handle(handle), _txBuffer{0}, _rxBuffer{0} {};
    ~Spi(){};
    SpiConfig &config_get();
    Result init();
    Result deinit();

    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);

  private:
    SPI_HandleTypeDef &_handle;
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
    BUffer _rxBuffer;

  protected:
    static void _on_read_complete_callback(SPI_HandleTypeDef *instance);
    static void _on_write_complete_callback(SPI_HandleTypeDef *instance);
    static void _on_error_callback(SPI_HandleTypeDef *instance);
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
    SpiWithPins(SPI_HandleTypeDef &handle, Pin *cs, Pin *rw, Pin *dc)
        : Spi(handle), _handle(handle), _cs(cs), _rw(rw), _dc(dc){};
    ~SpiWithPins(){};

    SpiWithPinsConfig &pinconfig_get();
    Result init();
    Result deinit();
    Result read(bool isData, void *data, uint32_t size,
                WaitHandler &waitHandler);
    Result write(bool isData, void *data, uint32_t size,
                 WaitHandler &waitHandler);
    Result session_begin();
    Result session_end();

  private:
    SPI_HandleTypeDef &_handle;
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
    static void _on_read_complete_callback(SPI_HandleTypeDef *instance);
    static void _on_write_complete_callback(SPI_HandleTypeDef *instance);
};
} // namespace ww::peripheral
#endif // __WWDEVICE_PERIPHERAL_SPI_HPP__