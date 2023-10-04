#include "i2c.hpp"
#include "peripheral.hpp"
#include "misc.hpp"
#include "os.hpp"
#include "pin.hpp"
#include "base.hpp"


namespace wibot::peripheral {

#define SCL_SET()   _scl.write(PinStatus::Set)
#define SCL_RESET() _scl.write(PinStatus::Reset)
#define SDA_SET()   _sda.write(PinStatus::Set)
#define SDA_RESET() _sda.write(PinStatus::Reset)
#define SDA_LOCK() _sda.mode_set(PinMode::Output)
#define SDA_RELEASE() _sda.mode_set(PinMode::Input)


static void _i2c_start(Pin& _scl, Pin& _sda, uint8_t delayUs) {

    SCL_SET();
    Misc::us_delay(delayUs);
    SDA_RELEASE();
    Misc::us_delay(delayUs);
    SDA_LOCK();
    Misc::us_delay(1);
    SDA_SET();
    Misc::us_delay(delayUs);

    SDA_RESET();
    Misc::us_delay(delayUs);
    SCL_RESET();  // hold the bus
    Misc::us_delay(delayUs);
};

static void _i2c_stop(Pin& _scl, Pin& _sda, uint8_t delayUs) {
//    SCL_RESET();
//    Misc::us_delay(_delayUs);
//    SDA_RESET();
//    Misc::us_delay(_delayUs);
    SCL_SET();
    Misc::us_delay(1);
    SDA_LOCK();
    Misc::us_delay(delayUs);
    SDA_SET();
    Misc::us_delay(delayUs);
    SDA_RELEASE();
    Misc::us_delay(delayUs);
};



static bool _i2c_ack_wait(Pin& _scl, Pin& _sda, uint8_t delayUs) {

    SCL_RESET();
    Misc::us_delay(1);
    SDA_RELEASE();
    Misc::us_delay(delayUs);
    SCL_SET();
    Misc::us_delay(1);
    PinStatus sta;
    _sda.read(sta);
    Misc::us_delay(delayUs);
    if (sta == PinStatus::Reset) {
        return true;
    }
    else {
        return false;
    }
};

static void _i2c_byte_write(Pin& _scl, Pin& _sda, uint8_t byte, uint8_t delayUs) {
    uint8_t i = 8;
    SCL_RESET();
    Misc::us_delay(1);
    SDA_LOCK();
    while (i--) {
        SCL_RESET();
        Misc::us_delay(delayUs);
        if (byte & 0x80) {
            SDA_SET();
        } else {
            SDA_RESET();
        }
        Misc::us_delay(delayUs);
        byte <<= 1;
        SCL_SET();
        Misc::us_delay(delayUs);
    }
};

static uint8_t _i2c_byte_read(Pin& _scl, Pin& _sda, uint8_t delayUs) {
    uint8_t i = 8;
    uint8_t byte = 0;
    SCL_RESET();
    Misc::us_delay(1);
    SDA_RELEASE();
    while (i--) {
        PinStatus sta;
        SCL_RESET();
        Misc::us_delay(delayUs);
        SCL_SET();
        Misc::us_delay(delayUs);
        _sda.read(sta);
        byte <<= 1;
        if (sta == PinStatus::Set) {
            byte |= 0x01;
        }
        Misc::us_delay(delayUs);
    }
    return byte;
};

static void _i2c_ack(Pin& _scl, Pin& _sda, uint8_t delayUs) {
    SCL_RESET();
    SDA_LOCK();
    Misc::us_delay(1);
    SDA_RESET();
    Misc::us_delay(delayUs);
    SCL_SET();
    Misc::us_delay(delayUs);
}

SoftI2cMaster::SoftI2cMaster(Pin& scl, Pin& sda) : _scl(scl), _sda(sda){};
SoftI2cMaster::~SoftI2cMaster(){};

Result SoftI2cMaster::_init(){
    _scl.init();
    _sda.init();
    _scl.mode_set(PinMode::Output);
    _delayUs = 10;
    return Result::OK;
};
void   SoftI2cMaster::_deinit(){
    _scl.deinit();
    _sda.deinit();
};

Result SoftI2cMaster::read(uint8_t* data, uint32_t size) {
    Result rst = Result::OK;
    _i2c_start(_scl, _sda, _delayUs);

    rst = _header(true);

    if (rst == Result::OK) {
        rst = _readData(data, size);
    }
    _i2c_stop(_scl, _sda, _delayUs);
    return rst;
};
Result SoftI2cMaster::write(uint8_t* data, uint32_t size) {
    Result rst = Result::OK;
    _i2c_start(_scl, _sda, _delayUs);

        rst = _header(false);

    if (rst == Result::OK) {
        rst = _writeData(data, size);
    }
    _i2c_stop(_scl, _sda, _delayUs);
    return rst;
}

Result SoftI2cMaster::writeRead(uint8_t* writeData, uint32_t writeSize, uint8_t* readData,
                                uint32_t readSize) {
    Result rst = Result::OK;
    _i2c_start(_scl, _sda, _delayUs);

    rst = _header(false);

    if (rst == Result::OK) {
        rst = _writeData(writeData, writeSize);
    }
    if (rst == Result::OK) {
        rst = _header(true);
    }
    if (rst == Result::OK) {
        rst = _readData(readData, readSize);
    }
    _i2c_stop(_scl, _sda, _delayUs);
    return rst;
}

Result SoftI2cMaster::writeWrite(uint8_t* writeData1, uint32_t writeSize1, uint8_t* writeData2,
                                 uint32_t writeSize2) {
    Result rst = Result::OK;
    _i2c_start(_scl, _sda, _delayUs);

    rst = _header(false);

    if (rst == Result::OK) {
        rst = _writeData(writeData1, writeSize1);
    }
    if (rst == Result::OK) {
        rst = _header(true);
    }
    if (rst == Result::OK) {
        rst = _writeData(writeData2, writeSize2);
    }
    _i2c_stop(_scl, _sda, _delayUs);
    return rst;
}

Result SoftI2cMaster::_header(bool isRead) {
    auto head = isRead ?((uint8_t)config.slaveAddress | 0x01) : ((uint8_t)config.slaveAddress & 0xFE);
    _i2c_byte_write(_scl, _sda, head, _delayUs);  // Slave address,SA0=0
    return _i2c_ack_wait(_scl, _sda, _delayUs)?Result::OK:Result::Timeout;
}
Result SoftI2cMaster::_writeData(uint8_t* data, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        _i2c_byte_write(_scl, _sda, ((uint8_t*)data)[i], _delayUs);
        if (!_i2c_ack_wait(_scl, _sda, _delayUs)) {
            return Result::Timeout;
        }
    }
    return Result::OK;
}
Result SoftI2cMaster::_readData(uint8_t* data, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        auto byte = _i2c_byte_read(_scl, _sda, _delayUs);
        _i2c_ack(_scl, _sda, _delayUs);
        data[i] = byte;
    }
}


}  // namespace wibot::peripheral

