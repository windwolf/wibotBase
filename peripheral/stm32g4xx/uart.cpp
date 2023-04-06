#include "uart.hpp"

#include "peripheral.hpp"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_usart.h"
#define LOG_MODULE "uart"
#include "log.h"
#ifdef HAL_UART_MODULE_ENABLED

namespace wibot::peripheral {

void UART::_on_write_complete_callback(UART_HandleTypeDef *instance) {
    UART *perip = (UART *)Peripherals::get_peripheral(instance);
    perip->_txCount++;
    auto wh = perip->_writeWaitHandler;
    if (wh != nullptr) {
        perip->_writeWaitHandler = nullptr;
        wh->done_set(perip);
    }
};

void UART::_on_read_complete_callback(UART_HandleTypeDef *instance) {
    UART *perip = (UART *)Peripherals::get_peripheral(instance);
    perip->_rxCount++;
    auto wh = perip->_readWaitHandler;
    if (wh != nullptr) {
        perip->_readWaitHandler = nullptr;
        wh->done_set(perip);
    }
};

void UART::_on_circular_data_received_callback(UART_HandleTypeDef *instance, uint16_t pos) {
    UART *perip = (UART *)Peripherals::get_peripheral(instance);
    auto length = perip->cirRxBuffer_->getLengthByMemIndex(pos, perip->_lastPos);
    perip->_rxCount++;
    perip->_lastPos = pos;
    perip->cirRxBuffer_->writeVirtual(length);
    auto wh = perip->_readWaitHandler;
    if (wh != nullptr) {
        wh->done_set(perip);
    }
};

void UART::_on_error_callback(UART_HandleTypeDef *instance) {
    UART *perip = (UART *)Peripherals::get_peripheral(instance);
    perip->_errorCount++;
    LOG_E("%s: on error ISR=%lX,err=%lX,uec=%lu", perip->name_, perip->_handle.Instance->ISR,
          HAL_UART_GetError(instance), perip->_errorCount);

    auto wh = perip->_readWaitHandler;
    if (wh != nullptr) {
        perip->_readWaitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set(perip);
    }
    wh = perip->_writeWaitHandler;
    if (wh != nullptr) {
        perip->_writeWaitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set(perip);
    }
};

UART::UART(UART_HandleTypeDef &handle, const char *name) : _handle(handle), name_(name){};

Result UART::_init() {
    HAL_UART_RegisterCallback(&_handle, HAL_UART_TX_COMPLETE_CB_ID,
                              &wibot::peripheral::UART::_on_write_complete_callback);
    HAL_UART_RegisterCallback(&_handle, HAL_UART_RX_COMPLETE_CB_ID,
                              &wibot::peripheral::UART::_on_read_complete_callback);
    HAL_UART_RegisterRxEventCallback(&_handle,
                                     &wibot::peripheral::UART::_on_circular_data_received_callback);
    HAL_UART_RegisterCallback(&_handle, HAL_UART_ERROR_CB_ID,
                              &wibot::peripheral::UART::_on_error_callback);
    Peripherals::register_peripheral("uart", this, &_handle);
    return Result::OK;
};

void UART::_deinit() { Peripherals::unregister_peripheral("uart", this); };

Result UART::read(void *data, uint32_t size, WaitHandler &waitHandler) {
    if (_readWaitHandler != nullptr) {
        return Result::Busy;
    }

    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) {
        return Result::Busy;
    }
    _readWaitHandler = &waitHandler;

#if PERIPHERAL_UART_READ_DMA_ENABLED
    _status.isRxDmaEnabled = true;
    // TODO: if size greater then uint16_t max, should slice the data and
    //  send in multiple DMA transfers
    return (Result)HAL_UART_Receive_DMA(&_handle, (uint8_t *)data, (uint16_t)size);
#endif
#if PERIPHERAL_UART_READ_IT_ENABLED
    _status.isRxDmaEnabled = false;
    return (Result)HAL_UART_Receive_IT(&_handle, (uint8_t *)data, (uint16_t)size);
#endif
};
Result UART::write(void *data, uint32_t size, WaitHandler &waitHandler) {
    if (_writeWaitHandler != nullptr) {
        return Result::Busy;
    }

    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX) {
        return Result::Busy;
    }
    _writeWaitHandler = &waitHandler;

#if PERIPHERAL_UART_WRITE_DMA_ENABLED
    _status.isTxDmaEnabled = 1;
    // TODO: if size greater then uint16_t max, should slice the data and
    // send in multiple DMA transfers
    return (Result)HAL_UART_Transmit_DMA(&_handle, (uint8_t *)data, (uint16_t)size);
#endif
#if PERIPHERAL_UART_WRITE_IT_ENABLED
    _status.isTxDmaEnabled = 0;
    return (Result)HAL_UART_Transmit_IT(&_handle, (uint8_t *)data, (uint16_t)size);
#endif
};

Result UART::start(CircularBuffer<uint8_t>& rxBuffer, WaitHandler& waitHandler) {
    if (_readWaitHandler != nullptr) {
        return Result::Busy;
    }
    if (_handle.hdmarx->Init.Mode != DMA_CIRCULAR) {
        return Result::NotSupport;
    }
    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX) {
        return Result::Busy;
    }
    _readWaitHandler = &waitHandler;
    cirRxBuffer_ = &rxBuffer;
#if PERIPHERAL_UART_READ_DMA_ENABLED
    return (Result)HAL_UARTEx_ReceiveToIdle_DMA(&_handle, (uint8_t*)rxBuffer.getDataPtr(),
                                                rxBuffer.getMemCapacity());
#else
    return (Result)HAL_UARTEx_ReceiveToIdle_IT(&_handle, (uint8_t*)rxBuffer.getDataPtr(),
                                               rxBuffer.getMemCapacity());
#endif
};

Result UART::stop() {
    Result rst = Result::OK;
    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_RX) != HAL_UART_STATE_BUSY_RX) {
        return rst;
    }
#if PERIPHERAL_UART_READ_DMA_ENABLED
    rst = (Result)HAL_UART_DMAStop(&_handle);

#else
    rst = (Result)HAL_UART_AbortReceive_IT(&_handle);
    ;
#endif
    auto wh = _readWaitHandler;
    if (wh != nullptr) {
        _readWaitHandler = nullptr;
        wh->done_set(this);
    }
    if (cirRxBuffer_ != nullptr) {
        cirRxBuffer_ = nullptr;
    }
    return rst;
};

};  // namespace wibot::peripheral

void uart_send_byte(const char *data, uint16_t len) {
    for (uint16_t todo = 0; todo < len; todo++) {
        /* 堵塞判断串口是否发送完成 */
        while (LL_USART_IsActiveFlag_TC(USART1) == 0)
            ;

        /* 串口发送完成，将该字符发送 */
        LL_USART_TransmitData8(USART1, (uint8_t)*data++);
    }
}

#endif  // HAL_UART_MODULE_ENABLED
