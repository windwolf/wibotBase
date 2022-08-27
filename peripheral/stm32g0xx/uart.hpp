#ifndef __WWDEVICE_PERIPHERAL_UART_HPP__
#define __WWDEVICE_PERIPHERAL_UART_HPP__

#include "peripheral.hpp"
#include "stm32g0xx_hal.h"

#ifndef HAL_UART_MODULE_ENABLED
#define UART_HandleTypeDef uint32_t
#endif // HAL_UART_MODULE_ENABLED

namespace ww::peripheral
{
union UARTConfig {
    struct
    {
        DataWidth dataWidth : 2;
        bool useTxDma : 1;
        bool useRxDma : 1;
        uint32_t : 4;
        uint8_t txDmaThreshold : 8;
        uint8_t rxDmaThreshold : 8;
        uint32_t : 8;
    };
    uint32_t value;
};

class UART
{
  public:
    UART(UART_HandleTypeDef &handle) : _handle(handle){};
    ~UART(){};
    UARTConfig &config_get();
    Result init();
    Result deinit();
    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);
    Result start(uint8_t *data, uint32_t size, WaitHandler &waitHandler);
    Result stop();

  private:
    UARTConfig _config;
    UART_HandleTypeDef &_handle;
    union {
        struct
        {
            bool isTxDmaEnabled : 1;
            bool isRxDmaEnabled : 1;
        };
        uint32_t value;
    } _status;
    WaitHandler *_writeWaitHandler;
    WaitHandler *_readWaitHandler;

  protected:
    static void _on_read_complete_callback(UART_HandleTypeDef *handle);
    static void _on_write_complete_callback(UART_HandleTypeDef *handle);
    static void _on_circular_data_received_callback(UART_HandleTypeDef *handle,
                                                    uint16_t pos);
    static void _on_error_callback(UART_HandleTypeDef *handle);
};

} // namespace ww::peripheral

extern "C"
{
    void uart_send_byte(const char *data, uint16_t len);
}
#endif // __WWDEVICE_PERIPHERAL_UART_HPP__