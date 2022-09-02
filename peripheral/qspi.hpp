#ifndef ___BSP_QSPI_H__
#define ___BSP_QSPI_H__

#include "base.hpp"
#include "pin.hpp"
#include "spi.hpp"
#include "wait_handler.hpp"
#include "stm32h7xx_hal.h"

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
    ~QSPI();
    SpiConfig &config_get();

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
}
} // namespace ww::peripheral

struct CommandQspi;
typedef void (*CommandQspiEventHandleFuncType)(struct CommandQspi *instance);
typedef struct CommandQspi
{
    Command base;
    void *instance;
    uint32_t dmaThreshold;

    void *parent;
    CommandQspiEventHandleFuncType onStatusPollingResult;

    Buffer _rxBuffer;
    struct
    {
        uint8_t isDmaTx : 1;
        uint8_t isDmaRx : 1;
    } _status;
} CommandQspi;

void _command_qspi_register(CommandQspi *command, void *parent, CommandErrorHandleFuncType onError,
                            CommandQspiEventHandleFuncType onStatusPollingResult);

OP_RESULT command_qspi_create(CommandQspi *command, QSPI_HandleTypeDef *instance,
                              uint32_t dmaThreshold);

OP_RESULT command_qspi_autopolling(CommandQspi *command, CommandFrame *pollingCommandStep,
                                   FSCCAutoPollingTypeDef *autoPolling);

#endif // ___BSP_QSPI_H__
