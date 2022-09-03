#ifndef ___BSP_QSPI_H__
#define ___BSP_QSPI_H__

#include "base.hpp"
#include "peripheral.hpp"
#include "wait_handler.hpp"

#define FSCCAutoPollingTypeDef QSPI_AutoPollingTypeDef
#define FSCC_EVENT_AUTO_POLLING_CPLT 0x08000000

namespace ww::peripheral
{
union QSPIConfig {
    struct
    {
        DataWidth dataWidth : 2;
        bool useTxDma : 1;
        bool useRxDma : 1;
        uint8_t dummyCycles : 4;
        uint8_t txDmaThreshold : 8;
        uint8_t rxDmaThreshold : 8;
        uint8_t mode : 1;
        uint32_t : 7;
    };
    uint32_t value;
};

class QSPI : Initializable
{
  public:
    QSPI(QSPI_CTOR_ARG);
    virtual Result _init();
    virtual void _deinit();
    QSPIConfig &config_get();

    Result read(void *data, uint32_t size, WaitHandler &waitHandler);
    Result write(void *data, uint32_t size, WaitHandler &waitHandler);

  private:
    QSPI_FIELD_DECL
    union {
        struct
        {
            bool isTxDmaEnabled : 1;
            bool isRxDmaEnabled : 1;
        };
        uint32_t value;
    } _status;
    QSPIConfig _config;
    WaitHandler *_readWaitHandler;
    WaitHandler *_writeWaitHandler;
    Buffer _txBuffer;
    Buffer _rxBuffer;

  protected:
    static void _on_read_complete_callback(QSPI_CALLBACK_ARG);
    static void _on_write_complete_callback(QSPI_CALLBACK_ARG);
    static void _on_command_complete_callback(QSPI_CALLBACK_ARG);
    static void _on_status_match_callback(QSPI_CALLBACK_ARG);
    static void _on_error_callback(QSPI_CALLBACK_ARG);
};

} // namespace ww::peripheral
#endif // ___BSP_QSPI_H__
