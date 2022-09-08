#ifndef __WWDEVICE_PERIPHERAL_UART_HPP__
#define __WWDEVICE_PERIPHERAL_UART_HPP__

#include "peripheral.hpp"

namespace ww::peripheral
{
#define UART_PER_DECL

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

class UART : public Initializable
{
  public:
    UART(UART_CTOR_ARG);
    Result _init() override;
    void _deinit() override;
    UARTConfig &config_get();
    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);
    Result start(uint8_t *data, uint32_t size, WaitHandler &waitHandler);
    Result stop();

  private:
    UART_FIELD_DECL
    UARTConfig _config;
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
    Buffer _txBuffer;
    Buffer _rxBuffer;

  protected:
    static void _on_read_complete_callback(UART_CALLBACK_ARG);
    static void _on_write_complete_callback(UART_CALLBACK_ARG);
    static void _on_circular_data_received_callback(UART_CALLBACK_ARG, uint16_t pos);
    static void _on_error_callback(UART_CALLBACK_ARG);
};

} // namespace ww::peripheral

extern "C"
{
    void uart_send_byte(const char *data, uint16_t len);
}
#endif // __WWDEVICE_PERIPHERAL_UART_HPP__