#include "i2c.hpp"
#include "misc.hpp"
#include "os.hpp"
#include "pin.hpp"

namespace ww::peripheral
{
#define SCL_SET() _scl.write(PinStatus::Set)
#define SCL_RESET() _scl.write(PinStatus::Reset)
#define SDA_SET() _sda.write(PinStatus::Set)
#define SDA_RESET() _sda.write(PinStatus::Reset)

static void _i2c_start(Pin &_scl, Pin &_sda)
{
    _sda.mode_set(PinMode::Output);
    _scl.mode_set(PinMode::Output);
    SDA_SET();
    SCL_SET();
    Misc::us_delay(4);
    SDA_RESET();
    Misc::us_delay(4);
    SCL_RESET(); // hold the bus
    Misc::us_delay(4);
};

static void _i2c_stop(Pin &_scl, Pin &_sda)
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

static bool _i2c_ack_wait(Pin &_scl, Pin &_sda, uint32_t timeout)
{
    bool ack = false;

    SCL_RESET();
    Misc::us_delay(2);
    _sda.mode_set(PinMode::Input);
    Misc::us_delay(2);
    SCL_SET();
    Misc::us_delay(4);
    for (uint8_t i = 20; i > 0; i--)
    {
        PinStatus sta;
        _sda.read(sta);
        if (sta == PinStatus::Reset)
        {
            ack = true;
            break;
        }
        {
            ack = true;
            break;
        }
    }
    _sda.mode_set(PinMode::Output);
    return ack;
};

static void _i2c_byte_write(Pin &_scl, Pin &_sda, uint8_t byte)
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
I2cMaster::I2cMaster(Pin &scl, Pin &sda) : _scl(scl), _sda(sda){};
I2cMaster::~I2cMaster(){};

Result I2cMaster::_init(){INIT_BEGIN() MEMBER_INIT_ERROR_CHECK(_scl) MEMBER_INIT_ERROR_CHECK(_sda)
                              INIT_END()};
void I2cMaster::_deinit(){MEMBER_DEINIT(_scl) MEMBER_DEINIT(_sda)};

Result I2cMaster::read(uint32_t address, void *data, uint32_t size, WaitHandler &waitHandler)
{
    return Result::NotSupport;
};
Result I2cMaster::write(uint32_t address, void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result::OK;
    if (rst != Result::OK)
    {
        return rst;
    }
    _i2c_start(_scl, _sda);
    _i2c_byte_write(_scl, _sda, (uint8_t)_config.slaveAddress & 0xFE); // Slave address,SA0=0
    if (!_i2c_ack_wait(_scl, _sda, 20))
    {
        rst = Result::Timeout;
    }

    if (_config.dataWidth == DataWidth::Bit8)
    {
        _i2c_byte_write(_scl, _sda, (uint8_t)address);
        if (!_i2c_ack_wait(_scl, _sda, 20))
        {
            rst = Result::Timeout;
        }
    }
    else if (_config.dataWidth == DataWidth::Bit16)
    {
        _i2c_byte_write(_scl, _sda, (uint8_t)(address >> 8));
        if (!_i2c_ack_wait(_scl, _sda, 20))
        {
            rst = Result::Timeout;
        }
        _i2c_byte_write(_scl, _sda, (uint8_t)address);
        if (!_i2c_ack_wait(_scl, _sda, 20))
        {
            rst = Result::Timeout;
        }
    }
    else
    {
        rst = Result::NotSupport;
    }

    if (rst == Result::OK)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            _i2c_byte_write(_scl, _sda, ((uint8_t *)data)[i]);
            if (!_i2c_ack_wait(_scl, _sda, 20))
            {
                rst = Result::Timeout;
                break;
            }
        }
    }

    _i2c_stop(_scl, _sda);

    if (rst == Result::OK)
    {
        waitHandler.done_set(this);
    }
    else
    {
        waitHandler.set_value((void *)rst);
        waitHandler.error_set(this);
    }
    return Result::OK;
};
Result I2cMaster::read(void *data, uint32_t size, WaitHandler &waitHandler)
{
    return Result::NotSupport;
};
Result I2cMaster::write(void *data, uint32_t size, WaitHandler &waitHandler)
{
    Result rst = Result::OK;
    if (rst != Result::OK)
    {
        return rst;
    }
    _i2c_start(_scl, _sda);
    _i2c_byte_write(_scl, _sda, (uint8_t)_config.slaveAddress & 0xFE); // Slave address,SA0=0
    if (!_i2c_ack_wait(_scl, _sda, 20))
    {
        rst = Result::Timeout;
    }
    if (rst == Result::OK)
    {
        for (uint32_t i = 0; i < size; i++)
        {
            _i2c_byte_write(_scl, _sda, ((uint8_t *)data)[i]);
            if (!_i2c_ack_wait(_scl, _sda, 20))
            {
                rst = Result::Timeout;
                break;
            }
        }
    }
    _i2c_stop(_scl, _sda);

    if (rst == Result::OK)
    {
        waitHandler.done_set(this);
    }
    else
    {
        waitHandler.set_value((void *)rst);
        waitHandler.error_set(this);
    }
    return Result::OK;
};

// uint32_t I2CMaster::wait_for_cplt(uint32_t timeout) {
//     auto tick = ww::os::Utils::tick_get();
//     while (_status.isWriteBusy || _status.isReadBusy) {
//         if (ww::os::Utils::tick_diff(tick) > timeout) {
//             return Result::Timeout;
//         }
//     }
// };
} // namespace ww::peripheral