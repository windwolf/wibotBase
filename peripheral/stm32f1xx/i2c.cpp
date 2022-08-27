#include "i2c.hpp"
#include "misc.hpp"
#include "os.hpp"

namespace ww::peripheral
{
#define SCL_SET() _scl.write(PinStatus_Set)
#define SCL_RESET() _scl.write(PinStatus_Reset)
#define SDA_SET() _sda.write(PinStatus_Set)
#define SDA_RESET() _sda.write(PinStatus_Reset)

void I2cMaster::_start()
{
    _scl.mode_set(PinMode_Output);
    _sda.mode_set(PinMode_Output);
    SDA_SET();
    SCL_SET();
    Misc::us_delay(4);
    SDA_RESET();
    Misc::us_delay(4);
    SCL_RESET(); // hold the bus
    Misc::us_delay(4);
};

void I2cMaster::_stop()
{
    SCL_RESET();
    Misc::us_delay(4);
    SDA_RESET();
    Misc::us_delay(4);
    SCL_SET();
    Misc::us_delay(4);
    SDA_SET();
    Misc::us_delay(4);
};

bool I2cMaster::_ack_wait(uint32_t timeout)
{
    bool ack = false;

    SCL_RESET();
    Misc::us_delay(2);
    _sda.mode_set(PinMode_Input);
    Misc::us_delay(2);
    SCL_SET();
    Misc::us_delay(4);
    for (uint8_t i = 20; i > 0; i--)
    {
        PinStatus sta;
        _sda.read(sta);
        if (sta == PinStatus_Reset)
        {
            ack = true;
            break;
        }
        {
            ack = true;
            break;
        }
    }
    _sda.mode_set(PinMode_Output);
    return ack;
};

void I2cMaster::_byte_write(uint8_t byte)
{
    uint8_t i = 8;
    while (i--)
    {
        SCL_RESET();
        Misc::us_delay(4);
        if (byte & 0x80)
        {
            SDA_SET();
        }
        else
        {
            SDA_RESET();
        }
        Misc::us_delay(4);
        byte <<= 1;
        SCL_SET();
        Misc::us_delay(4);
    }
};

I2cMasterConfig &I2cMaster::config_get()
{
    return _config;
};

Result I2cMaster::init()
{
    return Result_OK;
};
Result I2cMaster::deinit()
{
    return Result_OK;
};

Result I2cMaster::read(uint32_t address, void *data, uint32_t size,
                       WaitHandler &waitHandler)
{
    return Result_NotSupport;
};
Result I2cMaster::write(uint32_t address, void *data, uint32_t size,
                        WaitHandler &waitHandler)
{
    Result rst = Result_OK;
    if (rst != Result_OK)
    {
        return rst;
    }
    _start();
    _byte_write((uint8_t)_config.slaveAddress & 0xFE); // Slave address,SA0=0
    if (!_ack_wait(20))
    {
        rst = Result_Timeout;
    }

    if (_config.dataWidth == DATAWIDTH_8)
    {
        _byte_write((uint8_t)address);
        if (!_ack_wait(20))
        {
            rst = Result_Timeout;
        }
    }
    else if (_config.dataWidth == DATAWIDTH_16)
    {
        _byte_write((uint8_t)(address >> 8));
        if (!_ack_wait(20))
        {
            rst = Result_Timeout;
        }
        _byte_write((uint8_t)address);
        if (!_ack_wait(20))
        {
            rst = Result_Timeout;
        }
    }
    else
    {
        rst = Result_NotSupport;
    }

    if (rst == Result_OK)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            _byte_write(((uint8_t *)data)[i]);
            if (!_ack_wait(20))
            {
                rst = Result_Timeout;
                break;
            }
        }
    }

    _stop();

    if (rst == Result_OK)
    {
        waitHandler.done_set(this);
    }
    else
    {
        waitHandler.set_value((void *)rst);
        waitHandler.error_set(this);
    }
    return Result_OK;
};
Result I2cMaster::read(void *data, uint32_t size, WaitHandler &waitHandler)
{
    return Result_NotSupport;
};
Result I2cMaster::write(void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result_OK;
    if (rst != Result_OK)
    {
        return rst;
    }
    _start();
    _byte_write((uint8_t)_config.slaveAddress & 0xFE); // Slave address,SA0=0
    if (!_ack_wait(20))
    {
        rst = Result_Timeout;
    }
    if (rst == Result_OK)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            _byte_write(((uint8_t *)data)[i]);
            if (!_ack_wait(20))
            {
                rst = Result_Timeout;
                break;
            }
        }
    }
    _stop();

    if (rst == Result_OK)
    {
        waitHandler.done_set(this);
    }
    else
    {
        waitHandler.set_value((void *)rst);
        waitHandler.error_set(this);
    }
    return Result_OK;
};

// uint32_t I2CMaster::wait_for_cplt(uint32_t timeout) {
//     auto tick = ww::os::Utils::tick_get();
//     while (_status.isWriteBusy || _status.isReadBusy) {
//         if (ww::os::Utils::tick_diff(tick) > timeout) {
//             return Result_Timeout;
//         }
//     }
// };
} // namespace ww::peripheral