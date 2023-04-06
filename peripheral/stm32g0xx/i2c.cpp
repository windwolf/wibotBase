#include "i2c.hpp"

#include "misc.hpp"
#include "os.hpp"

#ifdef HAL_I2C_MODULE_ENABLED

namespace wibot::peripheral {

I2cMaster::I2cMaster(I2C_HandleTypeDef& handle) : _handle(handle){};

I2cMaster::~I2cMaster(){};

Result I2cMaster::_init() {
    HAL_I2C_RegisterCallback(&_handle, HAL_I2C_MEM_TX_COMPLETE_CB_ID,
                             &I2cMaster::_on_write_complete_callback);
    HAL_I2C_RegisterCallback(&_handle, HAL_I2C_MEM_RX_COMPLETE_CB_ID,
                             &I2cMaster::_on_read_complete_callback);
    HAL_I2C_RegisterCallback(&_handle, HAL_I2C_ERROR_CB_ID, &I2cMaster::_on_error_callback);
    Peripherals::register_peripheral("i2c", this, &_handle);
    return Result::OK;
};
void I2cMaster::_deinit() {
    Peripherals::unregister_peripheral("i2c", this);
};

Result I2cMaster::read(uint32_t address, void* data, uint32_t size, WaitHandler& waitHandler) {
    if (config.dataWidth > DataWidth::Bit16) {
        return Result::NotSupport;
    }
    if (this->_waitHandler != nullptr) {
        return Result::Busy;
    }
    this->_waitHandler = &waitHandler;
#if PERIPHERAL_I2C_READ_DMA_ENABLED
    _status.isWriteDmaEnabled = true;
    // SCB_CleanDCache_by_Addr((uint32_t *)data, size * (width - 1));
    return (Result)HAL_I2C_Mem_Read_DMA(
        &_handle, config.slaveAddress, address,
        config.dataWidth == DataWidth::Bit8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
        (uint8_t*)data, size * (to_underlying(config.dataWidth) + 1));
#endif
#if PERIPHERAL_I2C_READ_IT_ENABLED
    _status.isWriteDmaEnabled = false;
    return (Result)HAL_I2C_Mem_Read_IT(
        &_handle, config.slaveAddress, address,
        config.dataWidth == DataWidth::Bit8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
        (uint8_t*)data, size * (to_underlying(config.dataWidth) + 1));
#endif
};
Result I2cMaster::write(uint32_t address, void* data, uint32_t size, WaitHandler& waitHandler) {
    if (config.dataWidth > DataWidth::Bit16) {
        return Result::NotSupport;
    }
    if (this->_waitHandler != nullptr) {
        return Result::Busy;
    }
    this->_waitHandler = &waitHandler;
#if PERIPHERAL_I2C_WRITE_DMA_ENABLED
    _status.isWriteDmaEnabled = true;
    // SCB_CleanDCache_by_Addr((uint32_t *)data, size * (width - 1));
    return (Result)HAL_I2C_Mem_Write_DMA(
        &_handle, config.slaveAddress, address,
        config.dataWidth == DataWidth::Bit8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
        (uint8_t*)data, size * (to_underlying(config.dataWidth) + 1));
#endif
#if PERIPHERAL_I2C_WRITE_IT_ENABLED
    _status.isWriteDmaEnabled = false;
    return (Result)HAL_I2C_Mem_Write_IT(
        &_handle, config.slaveAddress, address,
        config.dataWidth == DataWidth::Bit8 ? I2C_MEMADD_SIZE_8BIT : I2C_MEMADD_SIZE_16BIT,
        (uint8_t*)data, size * (to_underlying(config.dataWidth) + 1));
#endif
};
Result I2cMaster::read(void* data, uint32_t size, WaitHandler& waitHandler) {
    if (config.dataWidth > DataWidth::Bit16) {
        return Result::NotSupport;
    }
    if (this->_waitHandler != nullptr) {
        return Result::Busy;
    }
    this->_waitHandler = &waitHandler;
#if PERIPHERAL_I2C_READ_DMA_ENABLED
    _status.isReadDmaEnabled = true;
    return (Result)HAL_I2C_Master_Receive_DMA(&_handle, config.slaveAddress, (uint8_t*)data,
                                              size * (to_underlying(config.dataWidth) + 1));
#endif
#if PERIPHERAL_I2C_READ_IT_ENABLED
    _status.isReadDmaEnabled = false;
    return (Result)HAL_I2C_Master_Receive_IT(&_handle, config.slaveAddress, (uint8_t*)data,
                                             size * (to_underlying(config.dataWidth) + 1));
#endif
};
Result I2cMaster::write(void* data, uint32_t size, WaitHandler& waitHandler) {
    if (config.dataWidth > DataWidth::Bit16) {
        return Result::NotSupport;
    }
    if (this->_waitHandler != nullptr) {
        return Result::Busy;
    }
    this->_waitHandler = &waitHandler;
#if PERIPHERAL_I2C_WRITE_DMA_ENABLED
    _status.isWriteDmaEnabled = true;
    // SCB_CleanDCache_by_Addr((uint32_t *)data, size * (width - 1));
    return (Result)HAL_I2C_Master_Transmit_DMA(&_handle, config.slaveAddress, (uint8_t*)data,
                                               size * (to_underlying(config.dataWidth) + 1));
#endif
#if PERIPHERAL_I2C_WRITE_IT_ENABLED
    _status.isWriteDmaEnabled = false;
    return (Result)HAL_I2C_Master_Transmit_IT(&_handle, config.slaveAddress, (uint8_t*)data,
                                              size * (to_underlying(config.dataWidth) + 1));
#endif
};

void I2cMaster::_on_read_complete_callback(I2C_HandleTypeDef* instance) {
    I2cMaster* perip = (I2cMaster*)Peripherals::get_peripheral(instance);
    auto       wh    = perip->_waitHandler;
    if (wh != nullptr) {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};
void I2cMaster::_on_write_complete_callback(I2C_HandleTypeDef* instance) {
    I2cMaster* perip = (I2cMaster*)Peripherals::get_peripheral(instance);
    auto       wh    = perip->_waitHandler;
    if (wh != nullptr) {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};
void I2cMaster::_on_error_callback(I2C_HandleTypeDef* instance) {
    I2cMaster* perip = (I2cMaster*)Peripherals::get_peripheral(instance);
    auto       wh    = perip->_waitHandler;
    if (wh != nullptr) {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
    wh = perip->_waitHandler;
    if (wh != nullptr) {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};

}  // namespace wibot::peripheral

#endif
