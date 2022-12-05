#include "spi.hpp"
#include "peripheral.hpp"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_spi.h"

#ifdef HAL_SPI_MODULE_ENABLED

namespace wibot::peripheral
{
struct SizeInfo
{
  public:
    uint32_t sizeInBytes;
    uint32_t sizeInDMADataWidth;
    uint32_t sizeInSPIDataWidth;
};

void Spi::_on_write_complete_callback(SPI_HandleTypeDef *instance)
{
    Spi *perip = (Spi *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_writeWaitHandler;
    if (wh != nullptr)
    {
        perip->_writeWaitHandler = nullptr;
        wh->done_set(perip);
    }
};

void Spi::_on_read_complete_callback(SPI_HandleTypeDef *instance)
{
    Spi *perip = (Spi *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_readWaitHandler;
    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data, perip->_rxBuffer.size);
    }
    if (wh != nullptr)
    {
        perip->_readWaitHandler = nullptr;
        wh->done_set(perip);
    }
};

void Spi::_on_error_callback(SPI_HandleTypeDef *instance)
{
    Spi *perip = (Spi *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_readWaitHandler;
    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data, perip->_rxBuffer.size);
    }
    if (wh != nullptr)
    {
        perip->_readWaitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set(perip);
    }
    wh = perip->_writeWaitHandler;
    if (wh != nullptr)
    {
        perip->_writeWaitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set(perip);
    }
};

static void bits_switch(SPI_HandleTypeDef &handle, SpiConfig option, uint32_t size,
                        wibot::peripheral::SizeInfo &sizeInfo)
{
    uint32_t stream_number_tx =
        (((uint32_t)((uint32_t *)handle.hdmatx->Instance) & 0xFFU) - 0x08UL) / 0x014UL;
    uint32_t dma_base_tx =
        (uint32_t)((uint32_t *)handle.hdmatx->Instance) - stream_number_tx * 0x014UL - 0x08UL;
    uint32_t stream_number_rx =
        (((uint32_t)((uint32_t *)handle.hdmarx->Instance) & 0xFFU) - 0x08UL) / 0x014UL;
    uint32_t dma_base_rx =
        (uint32_t)((uint32_t *)handle.hdmarx->Instance) - stream_number_rx * 0x014UL - 0x08UL;
    switch (option.dataWidth)
    {
    case DataWidth::Bit8:
        handle.Init.DataSize = SPI_DATASIZE_8BIT;
        LL_SPI_SetDataWidth(handle.Instance, LL_SPI_DATAWIDTH_8BIT);
        handle.hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        handle.hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        handle.hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        handle.hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_BYTE);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_BYTE);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_BYTE);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_BYTE);
        sizeInfo.sizeInBytes = size;
        sizeInfo.sizeInDMADataWidth = size;
        sizeInfo.sizeInSPIDataWidth = size;
        break;
    case DataWidth::Bit16:
        handle.Init.DataSize = SPI_DATASIZE_16BIT;
        LL_SPI_SetDataWidth(handle.Instance, LL_SPI_DATAWIDTH_16BIT);
        handle.hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        handle.hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        handle.hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        handle.hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx,
                             LL_DMA_MDATAALIGN_HALFWORD);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx,
                             LL_DMA_MDATAALIGN_HALFWORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx,
                             LL_DMA_PDATAALIGN_HALFWORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx,
                             LL_DMA_PDATAALIGN_HALFWORD);
        sizeInfo.sizeInBytes = size << 1;
        sizeInfo.sizeInDMADataWidth = size;
        sizeInfo.sizeInSPIDataWidth = size;
        break;
    case DataWidth::Bit24:
        handle->Init.DataSize = SPI_DATASIZE_24BIT;
        LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_24BIT);
        handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_WORD);

        sizeInfo.sizeInBytes = size * 3;
        sizeInfo.sizeInDMADataWidth = size * 3;
        sizeInfo.sizeInSPIDataWidth = size << 2;
        break;
    case DataWidth::Bit32:
        handle->Init.DataSize = SPI_DATASIZE_32BIT;
        LL_SPI_SetDataWidth(handle->Instance, LL_SPI_DATAWIDTH_32BIT);
        handle->hdmatx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmarx->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        handle->hdmatx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        handle->hdmarx->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetMemorySize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_WORD);
        LL_DMA_SetPeriphSize((DMA_TypeDef *)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_WORD);

        sizeInfo.sizeInBytes = size << 2;
        sizeInfo.sizeInDMADataWidth = size << 2;
        sizeInfo.sizeInSPIDataWidth = size << 2;
        break;
    default:
        break;
    }
    // return;
};

Spi::Spi(SPI_HandleTypeDef &handle) : _handle(handle){};

Result Spi::_init()
{
    HAL_SPI_RegisterCallback(&_handle, HAL_SPI_TX_COMPLETE_CB_ID,
                             &wibot::peripheral::Spi::_on_write_complete_callback);
    HAL_SPI_RegisterCallback(&_handle, HAL_SPI_RX_COMPLETE_CB_ID,
                             &wibot::peripheral::Spi::_on_read_complete_callback);
    HAL_SPI_RegisterCallback(&_handle, HAL_SPI_ERROR_CB_ID,
                             &wibot::peripheral::Spi::_on_error_callback);
    Peripherals::peripheral_register("spi", this, &_handle);
    return Result::OK;
};
void Spi::_deinit()
{
    Peripherals::peripheral_unregister("spi", this);
};


Result Spi::read(void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result::OK;
    if (_readWaitHandler != nullptr)
    {
        return Result::Busy;
    }
    _readWaitHandler = &waitHandler;

    wibot::peripheral::SizeInfo sizeInfo;
    wibot::peripheral::bits_switch(_handle, this->_config, size, sizeInfo);
    if (size < 0)
    {
        return Result::GeneralError;
    }
    if (this->_config.useRxDma && (size > this->_config.rxDmaThreshold))
    {
        this->_status.isRxDmaEnabled = 1;
        _rxBuffer.data = data;
        _rxBuffer.size = size * (to_underlying(_config.dataWidth) - 1);
        return (Result)HAL_SPI_Receive_DMA(&_handle, (uint8_t *)data, sizeInfo.sizeInDMADataWidth);
    }
    else
    {
        this->_status.isRxDmaEnabled = 0;
        return (Result)HAL_SPI_Receive_IT(&_handle, (uint8_t *)data, sizeInfo.sizeInSPIDataWidth);
    }
};
Result Spi::write(void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result::OK;
    if (_writeWaitHandler != nullptr)
    {
        return Result::Busy;
    }
    _writeWaitHandler = &waitHandler;

    wibot::peripheral::SizeInfo sizeInfo;
    wibot::peripheral::bits_switch(_handle, this->_config, size, sizeInfo);
    if (size < 0)
    {
        return Result::GeneralError;
    }
    if (this->_config.useTxDma && (size > this->_config.txDmaThreshold))
    {
        this->_status.isTxDmaEnabled = 1;
        _txBuffer.data = data;
        _txBuffer.size = size * (to_underlying(_config.dataWidth) - 1);
        SCB_CleanDCache_by_Addr((uint32_t *)data, size * (to_underlying(_config.dataWidth) - 1));
        return (Result)HAL_SPI_Transmit_DMA(&_handle, static_cast<uint8_t *>(data),
                                            sizeInfo.sizeInDMADataWidth);
    }
    else
    {
        this->_status.isTxDmaEnabled = 0;
        return (Result)HAL_SPI_Transmit_IT(&_handle, static_cast<uint8_t *>(data),
                                           sizeInfo.sizeInSPIDataWidth);
    }
};

// SpiWithPins--------------------

SpiWithPins::SpiWithPins(SPI_HandleTypeDef &handle, Pin *cs, Pin *rw, Pin *dc)
    : Spi(handle), _handle(handle), _cs(cs), _rw(rw), _dc(dc){};

Result SpiWithPins::_init()
{
    INIT_BEGIN()
    PTR_INIT_ERROR_CHECK(_cs)
    PTR_INIT_ERROR_CHECK(_rw)
    PTR_INIT_ERROR_CHECK(_dc)
    if (_cs)
    {
        _cs->config.inverse = _config.csPinHighIsDisable;
    }
    if (_dc)
    {
        _dc->config.inverse = _config.dcPinHighIsCmd;
    }
    if (_rw)
    {
        _dc->config.inverse = _config.rwPinHighIsWrite;
    }
    HAL_SPI_RegisterCallback(&_handle, HAL_SPI_TX_COMPLETE_CB_ID,
                             &wibot::peripheral::SpiWithPins::_on_write_complete_callback);
    HAL_SPI_RegisterCallback(&_handle, HAL_SPI_RX_COMPLETE_CB_ID,
                             &wibot::peripheral::SpiWithPins::_on_read_complete_callback);
    HAL_SPI_RegisterCallback(&_handle, HAL_SPI_ERROR_CB_ID,
                             &wibot::peripheral::Spi::_on_error_callback);
    Peripherals::peripheral_register("spiwithpin", this, &_handle);
    INIT_END()
};
void SpiWithPins::_deinit()
{
    Peripherals::peripheral_unregister("spiwithpin", this);
    PTR_DEINIT(_cs)
    PTR_DEINIT(_rw)
    PTR_DEINIT(_dc)
};

SpiWithPinsConfig &SpiWithPins::pinconfig_get()
{
    return _config;
};

Result SpiWithPins::read(bool isData, void *data, uint32_t size, WaitHandler &waitHandler)
{
    if (_config.autoCs || _status.busy)
    {
        cs_set(true);
    }
    rw_set(true);
    dc_set(isData);
    return Spi::read(data, size, waitHandler);
};
Result SpiWithPins::write(bool isData, void *data, uint32_t size, WaitHandler &waitHandler)
{
    if (_config.autoCs || _status.busy)
    {
        cs_set(true);
    }

    rw_set(false);
    dc_set(isData);
    return Spi::write(data, size, waitHandler);
};

Result SpiWithPins::session_begin()
{
    cs_set(false);
    if (_status.busy)
    {
        return Result::Busy;
    }

    _status.busy = 1;
    return Result::OK;
};
Result SpiWithPins::session_end()
{
    _status.busy = 0;
    cs_set(false);
    return Result::OK;
};

void SpiWithPins::_on_read_complete_callback(SPI_HandleTypeDef *instance)
{
    SpiWithPins *spi = (SpiWithPins *)Peripherals::peripheral_get_by_instance(instance);
    if (spi->_config.autoCs || !spi->_status.busy)
    {
        spi->cs_set(false);
    }
    Spi::_on_read_complete_callback(instance);
};
void SpiWithPins::_on_write_complete_callback(SPI_HandleTypeDef *instance)
{
    SpiWithPins *spi = (SpiWithPins *)Peripherals::peripheral_get_by_instance(instance);
    if (spi->_config.autoCs || !spi->_status.busy)
    {
        spi->cs_set(false);
    }
    Spi::_on_write_complete_callback(instance);
};

void SpiWithPins::cs_set(bool isEnable)
{
    if (_cs != NULL)
        _cs->write(isEnable ? PinStatus::Set : PinStatus::Reset);
};

void SpiWithPins::dc_set(bool isData)
{
    if (_dc != NULL)
        _dc->write(isData ? PinStatus::Set : PinStatus::Reset);
};

void SpiWithPins::rw_set(bool isRead)
{
    if (_rw != NULL)
        _rw->write(isRead ? PinStatus::Set : PinStatus::Reset);
};

}; // namespace wibot::peripheral

#endif
