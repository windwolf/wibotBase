#include "spi.hpp"
#include "peripheral.hpp"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_spi.h"

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

    void Spi::_on_write_complete_callback(SPI_HandleTypeDef* instance)
    {
        Spi* perip = (Spi*)Peripherals::get_peripheral(instance);
        auto wh = perip->waitHandler_;
        if (wh != nullptr)
        {
            perip->waitHandler_ = nullptr;
            wh->done_set(perip);
        }
    };

    void Spi::_on_read_complete_callback(SPI_HandleTypeDef* instance)
    {
        Spi* perip = (Spi*)Peripherals::get_peripheral(instance);
        auto wh = perip->waitHandler_;
        if (wh != nullptr)
        {
            perip->waitHandler_ = nullptr;
            wh->done_set(perip);
        }
    };

    void Spi::_on_write_read_complete_callback(SPI_HandleTypeDef* instance)
    {
        Spi* perip = (Spi*)Peripherals::get_peripheral(instance);
        auto wh = perip->waitHandler_;
        if (wh != nullptr)
        {
            perip->waitHandler_ = nullptr;
            wh->done_set(perip);
        }
    };

    void Spi::_on_error_callback(SPI_HandleTypeDef* instance)
    {
        Spi* perip = (Spi*)Peripherals::get_peripheral(instance);
        auto wh = perip->waitHandler_;
        if (wh != nullptr)
        {
            perip->waitHandler_ = nullptr;
            wh->set_value((void*)instance->ErrorCode);
            wh->error_set(perip);
        }
    };

    static void bits_switch(SPI_HandleTypeDef& handle, SpiConfig option, uint32_t size,
        wibot::peripheral::SizeInfo& sizeInfo)
    {
        sizeInfo.sizeInBytes = size << to_underlying(option.dataWidth);
        switch (option.dataWidth)
        {
        case DataWidth::Bit8:
        case DataWidth::Bit16:
            sizeInfo.sizeInDMADataWidth = size;
            sizeInfo.sizeInSPIDataWidth = size;
            break;
        case DataWidth::Bit24:
            sizeInfo.sizeInDMADataWidth = size * 3;
            sizeInfo.sizeInSPIDataWidth = size * 3;
            break;
        case DataWidth::Bit32:
            sizeInfo.sizeInDMADataWidth = size << 1;
            sizeInfo.sizeInSPIDataWidth = size << 1;
            break;
        default:
            break;
        }

        uint32_t stream_number_tx =
            (((uint32_t)((uint32_t*)handle.hdmatx->Instance) & 0xFFU) - 0x08UL) / 0x014UL;
        uint32_t dma_base_tx =
            (uint32_t)((uint32_t*)handle.hdmatx->Instance) - stream_number_tx * 0x014UL - 0x08UL;
        uint32_t stream_number_rx =
            (((uint32_t)((uint32_t*)handle.hdmarx->Instance) & 0xFFU) - 0x08UL) / 0x014UL;
        uint32_t dma_base_rx =
            (uint32_t)((uint32_t*)handle.hdmarx->Instance) - stream_number_rx * 0x014UL - 0x08UL;

        switch (option.dataWidth)
        {
        case DataWidth::Bit8:
        case DataWidth::Bit24:
        {
            handle.Init.DataSize = SPI_DATASIZE_8BIT;
            LL_SPI_SetDataWidth(handle.Instance, LL_SPI_DATAWIDTH_8BIT);
            auto& init1 = handle.hdmatx->Init;
            init1.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            init1.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            init1.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            init1.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            LL_DMA_SetMemorySize((DMA_TypeDef*)dma_base_tx, stream_number_tx, LL_DMA_MDATAALIGN_BYTE);
            LL_DMA_SetMemorySize((DMA_TypeDef*)dma_base_rx, stream_number_rx, LL_DMA_MDATAALIGN_BYTE);
            LL_DMA_SetPeriphSize((DMA_TypeDef*)dma_base_tx, stream_number_tx, LL_DMA_PDATAALIGN_BYTE);
            LL_DMA_SetPeriphSize((DMA_TypeDef*)dma_base_rx, stream_number_rx, LL_DMA_PDATAALIGN_BYTE);
        }

            break;
        case DataWidth::Bit16:
        case DataWidth::Bit32:
        {
            handle.Init.DataSize = SPI_DATASIZE_16BIT;
            LL_SPI_SetDataWidth(handle.Instance, LL_SPI_DATAWIDTH_16BIT);
            auto& init2 = handle.hdmatx->Init;
            init2.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
            init2.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
            init2.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            init2.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            LL_DMA_SetMemorySize((DMA_TypeDef*)dma_base_tx, stream_number_tx,
                LL_DMA_MDATAALIGN_HALFWORD);
            LL_DMA_SetMemorySize((DMA_TypeDef*)dma_base_rx, stream_number_rx,
                LL_DMA_MDATAALIGN_HALFWORD);
            LL_DMA_SetPeriphSize((DMA_TypeDef*)dma_base_tx, stream_number_tx,
                LL_DMA_PDATAALIGN_HALFWORD);
            LL_DMA_SetPeriphSize((DMA_TypeDef*)dma_base_rx, stream_number_rx,
                LL_DMA_PDATAALIGN_HALFWORD);
        }

            break;
        default:
            break;
        }
        // return;
    };

    Spi::Spi(SPI_HandleTypeDef& handle) : _handle(handle)
    {
    };

    Result Spi::_init()
    {
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_TX_COMPLETE_CB_ID,
            &wibot::peripheral::Spi::_on_write_complete_callback);
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_RX_COMPLETE_CB_ID,
            &wibot::peripheral::Spi::_on_read_complete_callback);
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_TX_RX_COMPLETE_CB_ID,
            &wibot::peripheral::Spi::_on_write_read_complete_callback);
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_ERROR_CB_ID,
            &wibot::peripheral::Spi::_on_error_callback);
        Peripherals::register_peripheral("spi", this, &_handle);
        return Result::OK;
    };
    void Spi::_deinit()
    {
        Peripherals::unregister_peripheral("spi", this);
    };

    Result Spi::read(void* data, uint32_t size, WaitHandler& waitHandler)
    {
        if (waitHandler_ != nullptr)
        {
            return Result::Busy;
        }
        waitHandler_ = &waitHandler;

        wibot::peripheral::SizeInfo sizeInfo;
        wibot::peripheral::bits_switch(_handle, this->config, size, sizeInfo);
        if (size < 0)
        {
            return Result::GeneralError;
        }
#if PERIPHERAL_SPI_READ_DMA_ENABLED
        this->_status.isRxDmaEnabled = true;
        return (Result)HAL_SPI_Receive_DMA(&_handle, (uint8_t*)data, sizeInfo.sizeInDMADataWidth);
#endif
#if PERIPHERAL_SPI_READ_IT_ENABLED
        this->_status.isRxDmaEnabled = false;
        return (Result)HAL_SPI_Receive_IT(&_handle, (uint8_t*)data, sizeInfo.sizeInSPIDataWidth);
#endif
    };
    Result Spi::write(void* data, uint32_t size, WaitHandler& waitHandler)
    {
        if (waitHandler_ != nullptr)
        {
            return Result::Busy;
        }
        waitHandler_ = &waitHandler;

        wibot::peripheral::SizeInfo sizeInfo;
        wibot::peripheral::bits_switch(_handle, this->config, size, sizeInfo);
        if (size < 0)
        {
            return Result::GeneralError;
        }
#if PERIPHERAL_SPI_WRITE_DMA_ENABLED
        this->_status.isTxDmaEnabled = true;
        // SCB_CleanDCache_by_Addr((uint32_t *)data, sizeInfo.sizeInBytes);
        return (Result)HAL_SPI_Transmit_DMA(&_handle, static_cast<uint8_t*>(data),
            sizeInfo.sizeInDMADataWidth);
#endif
#if PERIPHERAL_SPI_WRITE_IT_ENABLED
        this->_status.isTxDmaEnabled = false;
        return (Result)HAL_SPI_Transmit_IT(&_handle, static_cast<uint8_t*>(data),
            sizeInfo.sizeInSPIDataWidth);
#endif
    }

    Result Spi::write_read(void* txData, void* rxData, uint32_t size, WaitHandler& waitHandler)
    {
        if (waitHandler_ != nullptr)
        {
            return Result::Busy;
        }
        waitHandler_ = &waitHandler;
        wibot::peripheral::SizeInfo sizeInfo;
        wibot::peripheral::bits_switch(_handle, this->config, size, sizeInfo);
        if (size < 0)
        {
            return Result::GeneralError;
        }
#if PERIPHERAL_SPI_WRITE_DMA_ENABLED
        this->_status.isTxDmaEnabled = true;
        // SCB_CleanDCache_by_Addr((uint32_t *)txData, byteSize);
        return (Result)HAL_SPI_TransmitReceive_DMA(&_handle,
            static_cast<uint8_t*>(txData),
            static_cast<uint8_t*>(rxData),
            sizeInfo.sizeInDMADataWidth);
#endif
#if PERIPHERAL_SPI_WRITE_IT_ENABLED
        this->_status.isTxDmaEnabled = false;
        return (Result)HAL_SPI_TransmitReceive_IT(&_handle,
            static_cast<uint8_t*>(txData),
            static_cast<uint8_t*>(rxData),
            sizeInfo.sizeInSPIDataWidth);
#endif

    };

// SpiWithPins--------------------

    SpiWithPins::SpiWithPins(SPI_HandleTypeDef& handle, Pin* cs, Pin* rw, Pin* dc)
        : Spi(handle), _handle(handle), _cs(cs), _rw(rw), _dc(dc)
    {
    };

    Result SpiWithPins::_init()
    {
        INIT_BEGIN()
        PTR_INIT_ERROR_CHECK(_cs)
        PTR_INIT_ERROR_CHECK(_rw)
        PTR_INIT_ERROR_CHECK(_dc)
        if (_cs)
        {
            _cs->config.inverse = _pinconfig.csPinHighIsDisable;
        }
        if (_dc)
        {
            _dc->config.inverse = _pinconfig.dcPinHighIsCmd;
        }
        if (_rw)
        {
            _rw->config.inverse = _pinconfig.rwPinHighIsWrite;
        }
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_TX_COMPLETE_CB_ID,
            &wibot::peripheral::SpiWithPins::_on_write_complete_callback);
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_RX_COMPLETE_CB_ID,
            &wibot::peripheral::SpiWithPins::_on_read_complete_callback);
        HAL_SPI_RegisterCallback(&_handle, HAL_SPI_ERROR_CB_ID,
            &wibot::peripheral::Spi::_on_error_callback);
        Peripherals::register_peripheral("spiwithpin", this, &_handle);
        INIT_END()
    };
    void SpiWithPins::_deinit()
    {
        Peripherals::unregister_peripheral("spiwithpin", this);
        PTR_DEINIT(_cs)
        PTR_DEINIT(_rw)
        PTR_DEINIT(_dc)
    };

    SpiWithPinsConfig& SpiWithPins::pinconfig_get()
    {
        return _pinconfig;
    };

    Result SpiWithPins::read(bool isData, void* data, uint32_t size, WaitHandler& waitHandler)
    {
        if (_pinconfig.autoCs || _status.busy)
        {
            cs_set(true);
        }
        rw_set(true);
        dc_set(isData);
        return Spi::read(data, size, waitHandler);
    };
    Result SpiWithPins::write(bool isData, void* data, uint32_t size, WaitHandler& waitHandler)
    {
        if (_pinconfig.autoCs || _status.busy)
        {
            cs_set(true);
        }

        rw_set(false);
        dc_set(isData);
        return Spi::write(data, size, waitHandler);
    };

    Result SpiWithPins::session_begin(WaitHandler& waitHandler)
    {
        cs_set(true);
        if (_status.busy)
        {
            return Result::Busy;
        }

        _status.busy = 1;
        waitHandler.done_set(this);
        return Result::OK;
    };
    Result SpiWithPins::session_end(WaitHandler& waitHandler)
    {
        _status.busy = 0;
        cs_set(false);
        waitHandler.done_set(this);
        return Result::OK;
    };

    void SpiWithPins::_on_read_complete_callback(SPI_HandleTypeDef* instance)
    {
        SpiWithPins* spi = (SpiWithPins*)Peripherals::get_peripheral(instance);
        if (spi->_pinconfig.autoCs || !spi->_status.busy)
        {
            spi->cs_set(false);
        }
        Spi::_on_read_complete_callback(instance);
    };
    void SpiWithPins::_on_write_complete_callback(SPI_HandleTypeDef* instance)
    {
        SpiWithPins* spi = (SpiWithPins*)Peripherals::get_peripheral(instance);
        if (spi->_pinconfig.autoCs || !spi->_status.busy)
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
    }

}; // namespace wibot::peripheral

#endif
