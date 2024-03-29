#ifndef __WWDEVICE_PERIPHERAL_I2C_HPP__
#define __WWDEVICE_PERIPHERAL_I2C_HPP__

#include "buffer.hpp"
#include "peripheral.hpp"
#include "pin.hpp"

namespace wibot::peripheral {

I2C_PER_DECL

union I2cMasterConfig {
    struct {
        // uint8_t txDmaThreshold: 8;
        // uint8_t rxDmaThreshold: 8;
        /**
         * @brief 7bit, must be shifted left 1 bit
         */
        uint8_t   slaveAddress : 8;
        DataWidth dataWidth    : 2;
        // bool useTxDma: 1;
        // bool useRxDma: 1;
        uint32_t               : 4;
    };
    uint32_t value;
};

class I2cMaster : public Initializable, public Configurable<I2cMasterConfig> {
   public:
    I2cMaster(I2C_CTOR_ARG);
    ~I2cMaster();

    Result read(uint32_t address, void* data, uint32_t size, WaitHandler& waitHandler);
    Result write(uint32_t address, void* data, uint32_t size, WaitHandler& waitHandler);
    Result read(void* data, uint32_t size, WaitHandler& waitHandler);
    Result write(void* data, uint32_t size, WaitHandler& waitHandler);

   protected:
    Result _init() override;
    void   _deinit() override;

   private:
    I2C_FIELD_DECL

    struct {
    } _status;
    WaitHandler* _waitHandler;
    Buffer8      _txBuffer;
    Buffer8      _rxBuffer;
    static void  _on_read_complete_callback(I2C_CALLBACK_ARG);
    static void  _on_write_complete_callback(I2C_CALLBACK_ARG);
    static void  _on_error_callback(I2C_CALLBACK_ARG);
};

class SoftI2cMaster : public Initializable, public Configurable<I2cMasterConfig> {
   public:
    SoftI2cMaster(Pin& sda, Pin& scl);
    ~SoftI2cMaster();

    Result read(uint8_t* data, uint32_t size);
    Result write(uint8_t* data, uint32_t size);
    Result writeRead(uint8_t* writeData, uint32_t writeSize, uint8_t* readData, uint32_t readSize);
    Result writeWrite(uint8_t* writeData1, uint32_t writeSize1, uint8_t* writeData2,
                      uint32_t writeSize2);

   protected:
    Result _init() override;
    void   _deinit() override;

   private:
    Pin&    _sda;
    Pin&    _scl;
    uint8_t _delayUs;
    Result  _header(bool isRead);
    Result  _readData(uint8_t* data, uint32_t size);
    Result  _writeData(uint8_t* data, uint32_t size);
};

};  // namespace wibot::peripheral

#endif  // __WWDEVICE_PERIPHERAL_I2C_HPP__
