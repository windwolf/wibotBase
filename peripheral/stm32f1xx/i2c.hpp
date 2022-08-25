#ifndef __WWDEVICE_PERIPHERAL_I2C_HPP__
#define __WWDEVICE_PERIPHERAL_I2C_HPP__

#include "peripheral.hpp"
#include "pin.hpp"
#include "stm32f1xx_hal.h"

namespace ww::peripheral
{
union I2cMasterConfig {
    struct
    {
        DataWidth dataWidth : 2;
        uint8_t : 6;
        uint8_t slaveAddress : 8;
        uint32_t : 16;
    };
    uint32_t value;
};

class I2cMaster
{
  public:
    I2cMaster(Pin &scl, Pin &sda) : _scl(scl), _sda(sda){};
    ~I2cMaster(){};
    I2cMasterConfig &config_get();
    Result init();
    Result deinit();
    Result read(uint32_t address, void *data, uint32_t size,
                WaitHandler &waitHandler);
    Result write(uint32_t address, void *data, uint32_t size,
                 WaitHandler &waitHandler);
    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);

  private:
    Pin &_scl;
    Pin &_sda;
    I2cMasterConfig _config;
    union {
        struct
        {
            bool isReadDmaEnabled : 1;
            bool isWriteDmaEnabled : 1;
        };
        uint32_t value;
    } _status;
    void _start();
    void _stop();
    bool _ack_wait(uint32_t timeout);
    void _byte_write(uint8_t byte);
};
}; // namespace ww::peripheral

#endif // __WWDEVICE_PERIPHERAL_I2C_HPP__