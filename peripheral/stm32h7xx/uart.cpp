#include "uart.hpp"
#include "peripheral.hpp"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_usart.h"

namespace ww::peripheral
{

void UART::_on_write_complete_callback(UART_HandleTypeDef *instance)
{
    UART *perip = (UART *)Peripherals::peripheral_get_by_instance(instance);

    auto wh = perip->_writeWaitHandler;
    if (wh != nullptr)
    {
        perip->_writeWaitHandler = nullptr;
        wh->done_set();
    }
};

void UART::_on_read_complete_callback(UART_HandleTypeDef *instance)
{
    UART *perip = (UART *)Peripherals::peripheral_get_by_instance(instance);
    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data,
                                     perip->_rxBuffer.size);
    }
    auto wh = perip->_readWaitHandler;

    if (wh != nullptr)
    {
        perip->_readWaitHandler = nullptr;
        wh->done_set();
    }
};

void UART::_on_circular_data_received_callback(UART_HandleTypeDef *instance,
                                               uint16_t pos)
{
    UART *perip = (UART *)Peripherals::peripheral_get_by_instance(instance);
    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data,
                                     perip->_rxBuffer.size);
    }
    auto wh = perip->_readWaitHandler;

    if (wh != nullptr)
    {
        wh->set_value((void *)(uint32_t)pos);
        wh->done_set();
        wh->reset(perip);
    }
};

void UART::_on_error_callback(UART_HandleTypeDef *instance)
{
    UART *perip = (UART *)Peripherals::peripheral_get_by_instance(instance);
    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data,
                                     perip->_rxBuffer.size);
    }
    auto wh = perip->_readWaitHandler;
    if (wh != nullptr)
    {
        perip->_readWaitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set();
    }
    wh = perip->_writeWaitHandler;
    if (wh != nullptr)
    {
        perip->_writeWaitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set();
    }
};

Result UART::init()
{
    HAL_UART_RegisterCallback(
        &_handle, HAL_UART_TX_COMPLETE_CB_ID,
        &ww::peripheral::UART::_on_write_complete_callback);
    HAL_UART_RegisterCallback(
        &_handle, HAL_UART_RX_COMPLETE_CB_ID,
        &ww::peripheral::UART::_on_read_complete_callback);
    HAL_UART_RegisterRxEventCallback(
        &_handle, &ww::peripheral::UART::_on_circular_data_received_callback);
    HAL_UART_RegisterCallback(&_handle, HAL_UART_ERROR_CB_ID,
                              &ww::peripheral::UART::_on_error_callback);
    Peripherals::peripheral_register("uart", this, &_handle);
    return Result_OK;
};
Result UART::deinit()
{
    Peripherals::peripheral_unregister("uart", this);
    return Result_OK;
};

Result UART::read(void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result_OK;
    if (_readWaitHandler != nullptr)
    {
        return Result_Busy;
    }
    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_RX) ==
        HAL_UART_STATE_BUSY_RX)
    {
        return Result_Busy;
    }

    rst = waitHandler.reset(this);
    if (rst != Result_OK)
    {
        return Result_Busy;
    }
    _readWaitHandler = &waitHandler;

    if (_config.useRxDma && (size > _config.rxDmaThreshold))
    {
        _status.isRxDmaEnabled = 1;
        // TODO: if size greater then uint16_t max, should slice the data and
        // send in multiple DMA transfers
        return (Result)HAL_UART_Receive_DMA(&_handle, (uint8_t *)data,
                                            (uint16_t)size);
    }
    else
    {
        _status.isRxDmaEnabled = 0;
        return (Result)HAL_UART_Receive_IT(&_handle, (uint8_t *)data,
                                           (uint16_t)size);
    }
};
Result UART::write(void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result_OK;
    if (_writeWaitHandler != nullptr)
    {
        return Result_Busy;
    }

    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_TX) ==
        HAL_UART_STATE_BUSY_TX)
    {
        return Result_Busy;
    }

    rst = waitHandler.reset(this);
    if (rst != Result_OK)
    {
        return Result_Busy;
    }
    _writeWaitHandler = &waitHandler;

    if (_config.useTxDma && (size > _config.txDmaThreshold))
    {
        _status.isTxDmaEnabled = 1;
        SCB_CleanDCache_by_Addr((uint32_t *)data, size);
        // TODO: if size greater then uint16_t max, should slice the data and
        // send in multiple DMA transfers
        return (Result)HAL_UART_Transmit_DMA(&_handle, (uint8_t *)data,
                                             (uint16_t)size);
    }
    else
    {
        _status.isTxDmaEnabled = 0;
        return (Result)HAL_UART_Transmit_IT(&_handle, (uint8_t *)data,
                                            (uint16_t)size);
    }
};

Result UART::start(uint8_t *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result_OK;
    if (_readWaitHandler != nullptr)
    {
        return Result_Busy;
    }
    if (_handle.hdmarx->Init.Mode != DMA_CIRCULAR)
    {
        return Result_NotSupport;
    }
    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_RX) ==
        HAL_UART_STATE_BUSY_RX)
    {
        return Result_Busy;
    }
    rst = waitHandler.reset(this);
    if (rst != Result_OK)
    {
        return Result_Busy;
    }
    _readWaitHandler = &waitHandler;

    return (Result)HAL_UARTEx_ReceiveToIdle_DMA(&_handle, data, size);
};

Result UART::stop()
{

    Result rst = Result_OK;
    if ((HAL_UART_GetState(&_handle) & HAL_UART_STATE_BUSY_RX) !=
        HAL_UART_STATE_BUSY_RX)
    {
        rst = Result_OK;
    }
    else
    {
        rst = (Result)HAL_UART_DMAStop(&_handle);
    }
    auto wh = _readWaitHandler;
    if (wh != nullptr)
    {
        _readWaitHandler = nullptr;
        wh->done_set();
    }
    return rst;
};

}; // namespace ww::peripheral

void uart_send_byte(const char *data, uint16_t len)
{
    for (uint16_t todo = 0; todo < len; todo++)
    {

        /* 堵塞判断串口是否发送完成 */
        while (LL_USART_IsActiveFlag_TC(USART1) == 0)
            ;

        /* 串口发送完成，将该字符发送 */
        LL_USART_TransmitData8(USART1, (uint8_t)*data++);
    }
}