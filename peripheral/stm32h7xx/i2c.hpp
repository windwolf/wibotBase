#ifndef __WWDEVICE_PERIPHERAL_I2C_HPP__
#define __WWDEVICE_PERIPHERAL_I2C_HPP__

#include "peripheral.hpp"
#include "pin.hpp"
#include "stm32h7xx_hal.h"

#ifndef HAL_I2C_MODULE_ENABLED
#define I2C_HandleTypeDef uint32_t
#endif // HAL_I2C_MODULE_ENABLED

namespace ww::peripheral
{
union I2cMasterConfig {
    struct
    {
        uint8_t txDmaThreshold : 8;
        uint8_t rxDmaThreshold : 8;
        uint8_t slaveAddress : 8;
        DataWidth dataWidth : 2;
        bool useTxDma : 1;
        bool useRxDma : 1;
        uint32_t : 4;
    };
    uint32_t value;
};

class I2cMaster : public Initializable
{
  public:
    I2cMaster(I2C_HandleTypeDef &handle);
    ~I2cMaster();
    I2cMasterConfig &config_get();
    Result read(uint32_t address, void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(uint32_t address, void *data, uint32_t size, WaitHandler &waitHandler);
    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);

  private:
    I2C_HandleTypeDef &_handle;
    I2cMasterConfig _config;
    union {
        struct
        {
            bool isReadDmaEnabled : 1;
            bool isWriteDmaEnabled : 1;
        };
        uint32_t value;
    } _status;
    WaitHandler *_waitHandler;
    Buffer _txBuffer;
    BUffer _rxBuffer;
    static void _on_read_complete_callback(I2C_HandleTypeDef *instance);
    static void _on_write_complete_callback(I2C_HandleTypeDef *instance);
    static void _on_error_callback(I2C_HandleTypeDef *instance);
};

}; // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_I2C_HPP__