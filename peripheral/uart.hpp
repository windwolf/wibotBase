#ifndef __WWDEVICE_PERIPHERAL_UART_HPP__
#define __WWDEVICE_PERIPHERAL_UART_HPP__

#include "CircularBuffer.hpp"
#include "peripheral.hpp"
#include "ringbuffer.hpp"

namespace wibot::peripheral {
#define UART_PER_DECL

union UARTConfig {
    struct {
        DataWidth dataWidth            : 2;
        bool      ignore_parity_error  : 1;
        bool      ignore_frame_error   : 1;
        bool      ignore_overrun_error : 1;
        bool      ignore_noise_error   : 1;
        uint32_t                       : 26;
    };
    uint32_t value;
};

class UART : public Initializable, public Configurable<UARTConfig> {
   public:
    UART(UART_CTOR_ARG, const char* name);
    Result _init() override;
    void   _deinit() override;

    Result read(void* data, uint32_t size, WaitHandler& waitHandler);
    Result write(void* data, uint32_t size, WaitHandler& waitHandler);
    Result start(CircularBuffer<uint8_t>& rxBuffer, WaitHandler& waitHandler);
    Result stop();

   public:
    uint32_t _errorCount;
    uint32_t _rxCount;
    uint32_t _txCount;

   private:
    UART_FIELD_DECL
    struct {
        uint16_t _lastPos;
    } _status;

    WaitHandler*             _writeWaitHandler;
    WaitHandler*             _readWaitHandler;
    CircularBuffer<uint8_t>* cirRxBuffer_;
    const char*              name_;

   protected:
    static void _on_read_complete_callback(UART_CALLBACK_ARG);
    static void _on_write_complete_callback(UART_CALLBACK_ARG);
    static void _on_circular_data_received_callback(UART_CALLBACK_ARG, uint16_t pos);
    static void _on_error_callback(UART_CALLBACK_ARG);
};

}  // namespace wibot::peripheral

extern "C" {
void uart_send_byte(const char* data, uint16_t len);
}
#endif  // __WWDEVICE_PERIPHERAL_UART_HPP__
