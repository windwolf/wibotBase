#include "i2c.hpp"
#include "misc.hpp"
#include "os.hpp"

#ifdef HAL_I2C_MODULE_ENABLED

namespace ww::peripheral
{

I2cMasterConfig &I2cMaster::config_get()
{
    return _config;
};

Result I2cMaster::init()
{
    HAL_I2C_RegisterCallback(&_handle, HAL_I2C_MEM_TX_COMPLETE_CB_ID,
                             &I2cMaster::_on_write_complete_callback);
    HAL_I2C_RegisterCallback(&_handle, HAL_I2C_MEM_RX_COMPLETE_CB_ID,
                             &I2cMaster::_on_read_complete_callback);
    HAL_I2C_RegisterCallback(&_handle, HAL_I2C_ERROR_CB_ID,
                             &I2cMaster::_on_error_callback);
    Peripherals::peripheral_register("i2c", this, &_handle);
    return Result_OK;
};
Result I2cMaster::deinit()
{
    Peripherals::peripheral_unregister("i2c", this);
    return Result_OK;
};

Result I2cMaster::read(uint32_t address, void *data, uint32_t size,
                       WaitHandler &waitHandler)
{
    this->_rxWaitHandler = &waitHandler;
    if (_config.dataWidth > DATAWIDTH_16)
    {
        return Result_NotSupport;
    }
    if (this->_waitHandler != nullptr)
    {
        return Result_Busy;
    }
    this->_waitHandler = &waitHandler;
    if (_config.useTxDma && (size > _config.txDmaThreshold))
    {
        _status.isWriteDmaEnabled = 1;
        _rxBuffer.data = data;
        _rxBuffer.size = size * (_config.dataWidth - 1);
        return (Result)HAL_I2C_Mem_Read_DMA(
            &_handle, _config.slaveAddress, address,
            _config.dataWidth == DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT
                                             : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size * (_config.dataWidth + 1));
    }
    else
    {
        _status.isWriteDmaEnabled = 0;
        return (Result)HAL_I2C_Mem_Read_IT(
            &_handle, _config.slaveAddress, address,
            _config.dataWidth == DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT
                                             : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size * (_config.dataWidth + 1));
    }
};
Result I2cMaster::write(uint32_t address, void *data, uint32_t size,
                        WaitHandler &waitHandler)
{
    if (_config.dataWidth > DATAWIDTH_16)
    {
        return Result_NotSupport;
    }
    if (this->_waitHandler != nullptr)
    {
        return Result_Busy;
    }
    this->_waitHandler = &waitHandler;
    if (_config.useTxDma && (size > _config.txDmaThreshold))
    {
        _status.isWriteDmaEnabled = 1;
        _txBuffer.data = data;
        _txBuffer.size = size * (_config.dataWidth - 1);
        SCB_CleanDCache_by_Addr((uint32_t *)data,
                                size * (_config.dataWidth - 1));
        return (Result)HAL_I2C_Mem_Write_DMA(
            &_handle, _config.slaveAddress, address,
            _config.dataWidth == DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT
                                             : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size * (_config.dataWidth + 1));
    }
    else
    {
        _status.isWriteDmaEnabled = 0;
        return (Result)HAL_I2C_Mem_Write_IT(
            &_handle, _config.slaveAddress, address,
            _config.dataWidth == DATAWIDTH_8 ? I2C_MEMADD_SIZE_8BIT
                                             : I2C_MEMADD_SIZE_16BIT,
            (uint8_t *)data, size * (_config.dataWidth + 1));
    }
};
Result I2cMaster::read(void *data, uint32_t size, WaitHandler &waitHandler)
{
    if (_config.dataWidth > DATAWIDTH_16)
    {
        return Result_NotSupport;
    }
    if (this->_waitHandler != nullptr)
    {
        return Result_Busy;
    }
    this->_waitHandler = &waitHandler;
    if (_config.useRxDma && (size > _config.rxDmaThreshold))
    {
        _status.isReadDmaEnabled = 1;
        _rxBuffer.data = data;
        _rxBuffer.size = size * (_config.dataWidth - 1);
        return (Result)HAL_I2C_Master_Receive_DMA(
            &_handle, _config.slaveAddress, (uint8_t *)data,
            size * (_config.dataWidth + 1));
    }
    else
    {
        _status.isReadDmaEnabled = 0;
        return (Result)HAL_I2C_Master_Receive_IT(
            &_handle, _config.slaveAddress, (uint8_t *)data,
            size * (_config.dataWidth + 1));
    }
};
Result I2cMaster::write(void *data, uint32_t size, WaitHandler &waitHandler)
{
    if (_config.dataWidth > DATAWIDTH_16)
    {
        return Result_NotSupport;
    }
    if (this->_waitHandler != nullptr)
    {
        return Result_Busy;
    }
    this->_waitHandler = &waitHandler;
    if (_config.useTxDma && (size > _config.txDmaThreshold))
    {
        _status.isWriteDmaEnabled = 1;
        _txBuffer.data = data;
        _txBuffer.size = size * (_config.dataWidth - 1);
        SCB_CleanDCache_by_Addr((uint32_t *)data,
                                size * (_config.dataWidth - 1));
        return (Result)HAL_I2C_Master_Transmit_DMA(
            &_handle, _config.slaveAddress, (uint8_t *)data,
            size * (_config.dataWidth + 1));
    }
    else
    {
        _status.isWriteDmaEnabled = 0;
        return (Result)HAL_I2C_Master_Transmit_IT(
            &_handle, _config.slaveAddress, (uint8_t *)data,
            size * (_config.dataWidth + 1));
    }
};

void I2cMaster::_on_read_complete_callback(I2C_HandleTypeDef *instance)
{
    I2cMaster *i2c =
        (I2cMaster *)Peripherals::peripheral_get_by_instance(instance);
    if (i2c->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(spi->_rxBuffer.data, spi->_rxBuffer.size);
    }
    auto wh = i2c->_rxWaitHandler;
    if (wh != nullptr)
    {
        i2c->_rxWaitHandler = nullptr;
        wh->done_set();
    }
};
void I2cMaster::_on_read_complete_callback(I2C_HandleTypeDef *instance)
{
    I2cMaster *perip =
        (I2cMaster *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_waitHandler;
    if (wh != nullptr)
    {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};
void I2cMaster::_on_write_complete_callback(I2C_HandleTypeDef *instance)
{
    I2cMaster *perip =
        (I2cMaster *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_waitHandler;
    if (wh != nullptr)
    {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};
void I2cMaster::_on_error_callback(I2C_HandleTypeDef *instance)
{
    I2cMaster *perip =
        (I2cMaster *)Peripherals::peripheral_get_by_instance(instance);
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

} // namespace ww::peripheral

#endif