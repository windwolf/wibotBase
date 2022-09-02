// #ifndef ___BSP_QSPI_H__
// #define ___BSP_QSPI_H__


// #include "base.hpp"
// #include "pin.hpp"
// #include "spi.hpp"
// #include "wait_handler.hpp"
// #include "stm32h7xx_hal.h"

// #define FSCCAutoPollingTypeDef QSPI_AutoPollingTypeDef
// #define FSCC_EVENT_AUTO_POLLING_CPLT 0x08000000

//     struct CommandQspi;
//     typedef void (*CommandQspiEventHandleFuncType)(struct CommandQspi *instance);
//     typedef struct CommandQspi
//     {
//         Command base;
//         void *instance;
//         uint32_t dmaThreshold;

//         void *parent;
//         CommandQspiEventHandleFuncType onStatusPollingResult;

//         Buffer _rxBuffer;
//         struct
//         {
//             uint8_t isDmaTx : 1;
//             uint8_t isDmaRx : 1;
//         } _status;
//     } CommandQspi;

//     void _command_qspi_register(CommandQspi *command, void *parent,
//                                 CommandErrorHandleFuncType onError,
//                                 CommandQspiEventHandleFuncType onStatusPollingResult);

//     OP_RESULT command_qspi_create(CommandQspi *command,
//                                   QSPI_HandleTypeDef *instance,
//                                   uint32_t dmaThreshold);

//     OP_RESULT command_qspi_autopolling(CommandQspi *command, CommandFrame *pollingCommandStep, FSCCAutoPollingTypeDef *autoPolling);



// #endif // ___BSP_QSPI_H__
