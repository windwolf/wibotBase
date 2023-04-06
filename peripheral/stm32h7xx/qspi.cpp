#include "qspi.hpp"

#include "peripheral.hpp"
#include "stm32h7xx_ll_dma.h"

#ifdef HAL_QSPI_MODULE_ENABLED
namespace wibot::peripheral {
QSPI::QSPI(QSPI_HandleTypeDef &handle) : _handle(handle) {
}

Result QSPI::_init() {
    HAL_QSPI_RegisterCallback(&_handle, HAL_QSPI_TX_CPLT_CB_ID, &QSPI::_on_write_complete_callback);
    HAL_QSPI_RegisterCallback(&_handle, HAL_QSPI_RX_CPLT_CB_ID, &QSPI::_on_read_complete_callback);
    HAL_QSPI_RegisterCallback(&_handle, HAL_QSPI_CMD_CPLT_CB_ID,
                              &QSPI::_on_command_complete_callback);
    HAL_QSPI_RegisterCallback(&_handle, HAL_QSPI_STATUS_MATCH_CB_ID,
                              &QSPI::_on_status_match_callback);
    HAL_QSPI_RegisterCallback(&_handle, HAL_QSPI_ERROR_CB_ID, &QSPI::_on_error_callback);
    Peripherals::register_peripheral("qspi", this, &_handle);
    return Result::OK;
};
void QSPI::_deinit() {
    Peripherals::unregister_peripheral("qspi", this);
};

static void _command_qspi_cmd_tranlate(CommandFrame *cmd, QSPI_CommandTypeDef *cmdhandler) {
    cmdhandler->Instruction     = cmd->commandId;
    cmdhandler->InstructionMode = cmd->commandMode << QUADSPI_CCR_IMODE_Pos;

    cmdhandler->Address     = cmd->address;
    cmdhandler->AddressSize = cmd->addressBits << QUADSPI_CCR_ADSIZE_Pos;
    cmdhandler->AddressMode = cmd->addressMode << QUADSPI_CCR_ADMODE_Pos;

    cmdhandler->AlternateBytes     = cmd->altData;
    cmdhandler->AlternateBytesSize = cmd->altDataBits
                                     << QSPI_ALTERNATE_BYTES_8_BITS;  // QUADSPI_CCR_ABSIZE_Pos
    cmdhandler->AlternateByteMode = cmd->altDataMode << QUADSPI_CCR_ABMODE_Pos;

    cmdhandler->DummyCycles = cmd->dummyCycles;

    cmdhandler->DataMode = cmd->dataMode << QUADSPI_CCR_DMODE_Pos;
    cmdhandler->NbData   = cmd->dataSize;

    cmdhandler->DdrMode          = cmd->isDdr ? QSPI_DDR_MODE_ENABLE : QSPI_DDR_MODE_DISABLE;
    cmdhandler->DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
    cmdhandler->SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;
};

static OP_RESULT _command_qspi_cmd_begin(CommandQspi *command) {
    QSPI_CommandTypeDef cmdHandler;
    CommandFrame       *cmd = command->base._curFrame;
    _command_qspi_cmd_tranlate(cmd, &cmdHandler);

    if (cmd->dataMode == COMMAND_FRAME_MODE_SKIP) {
        return HAL_QSPI_Command_IT((QSPI_HandleTypeDef *)command->instance, &cmdHandler);
    } else {
        OP_RESULT rst =
            HAL_QSPI_Command((QSPI_HandleTypeDef *)command->instance, &cmdHandler, HAL_MAX_DELAY);
        if (rst != HAL_OK) {
            return rst;
        }

        if (cmd->isWrite) {
            if (cmd->dataSize > command->dmaThreshold) {
                command->_status.isDmaTx = 1;
                SCB_CleanDCache_by_Addr((uint32_t *)cmd->data, cmd->dataSize << cmd->dataBits);
                return HAL_QSPI_Transmit_DMA(command->instance, cmd->data);
            } else {
                command->_status.isDmaTx = 0;
                return HAL_QSPI_Transmit_IT(command->instance, cmd->data);
            }
        } else {
            if (cmd->dataSize > command->dmaThreshold) {
                command->_status.isDmaRx = 1;
                command->_rxBuffer.data  = cmd->data;
                command->_rxBuffer.size  = cmd->dataSize << cmd->dataBits;
                return HAL_QSPI_Receive_DMA(command->instance, cmd->data);
            } else {
                command->_status.isDmaRx = 0;
                return HAL_QSPI_Receive_IT(command->instance, cmd->data);
            }
        }
    }
};

static void _command_qspi_cmd_data_end(CommandQspi *command) {
    command->_rxBuffer.data = 0;
    command->_rxBuffer.size = 0;
    command_end(&(command->base));
};

static OP_RESULT _command_qspi_send(Command *cc, CommandFrame *commandStep) {
    CommandQspi *command = (CommandQspi *)cc;
    if (command_is_busy(cc)) {
        return OP_RESULT_BUSY;
    }
    command_start(&(command->base));
    command->base._curFrame = commandStep;
    return _command_qspi_cmd_begin(command);
};

static void _qspi_cmd_cplt(QSPI_HandleTypeDef *instance) {
    // 这个中断进来, 说明后续没有数据;
    OP_RESULT rst;

    _command_qspi_cmd_data_end(_cc);
};

static void _qspi_tx_cplt(QSPI_HandleTypeDef *instance) {
    OP_RESULT rst;
    _command_qspi_cmd_data_end(_cc);
};

static void _qspi_rx_cplt(QSPI_HandleTypeDef *instance) {
    OP_RESULT rst;

    if (_cc->_status.isDmaRx) {
        SCB_InvalidateDCache_by_Addr(_cc->_rxBuffer.data, _cc->_rxBuffer.size);
    }
    _command_qspi_cmd_data_end(_cc);
};

static void _qspi_error(QSPI_HandleTypeDef *instance) {
    if (command_is_busy(&(_cc->base))) {
        command_end(&(_cc->base));
    }
    if (_cc->base.onError) {
        _cc->base.onError(&(_cc->base), instance->ErrorCode);
    }
};

static void _command_qspi_result_match(QSPI_HandleTypeDef *instance) {
    ww_os_events_set(&(_cc->base.events), FSCC_EVENT_AUTO_POLLING_CPLT);
    if (_cc->onStatusPollingResult) {
        _cc->onStatusPollingResult(_cc);
    }
};

OP_RESULT command_qspi_auto_polling_wait(CommandQspi *command) {
    ULONG actualFlags;
    if (ww_os_events_get(&command->base.events, FSCC_EVENT_AUTO_POLLING_CPLT,
                         DRIVER_EVENTS_OPTION_OR, DRIVER_TIMEOUT_FOREVER)) {
        return OP_RESULT_OK;
    } else {
        return OP_RESULT_GENERAL_ERROR;
    }
};

OP_RESULT command_qspi_autopolling(CommandQspi *command, CommandFrame *pollingCommand,
                                   FSCCAutoPollingTypeDef *cfg) {
    QSPI_CommandTypeDef cmdHandler;
    _command_qspi_cmd_tranlate(pollingCommand, &cmdHandler);
    if (!ww_os_events_reset(&command->base.events, FSCC_EVENT_AUTO_POLLING_CPLT)) {
        return OP_RESULT_GENERAL_ERROR;
    }
    return HAL_QSPI_AutoPolling((QSPI_HandleTypeDef *)command->instance, &cmdHandler, cfg,
                                HAL_MAX_DELAY);
};

OP_RESULT command_qspi_create(CommandQspi *command, QSPI_HandleTypeDef *instance,
                              uint32_t dmaThreshold) {
    command_create((Command *)command, &_command_qspi_send);
    command->instance        = instance;
    command->dmaThreshold    = dmaThreshold;
    command->_rxBuffer.data  = NULL;
    command->_rxBuffer.size  = 0;
    command->_status.isDmaRx = 0;
    command->_status.isDmaTx = 0;
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_TX_CPLT_CB_ID, &_qspi_tx_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_RX_CPLT_CB_ID, &_qspi_rx_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_CMD_CPLT_CB_ID, &_qspi_cmd_cplt);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_STATUS_MATCH_CB_ID, &_command_qspi_result_match);
    HAL_QSPI_RegisterCallback(instance, HAL_QSPI_ERROR_CB_ID, &_qspi_error);
    _cc = command;
    return OP_RESULT_OK;
};

void _command_qspi_register(CommandQspi *command, void *parent, CommandErrorHandleFuncType onError,
                            CommandQspiEventHandleFuncType onStatusPollingResult) {
    command->parent                = parent;
    command->base.onError          = onError;
    command->onStatusPollingResult = onStatusPollingResult;
};
}  // namespace wibot::peripheral
#endif  // HAL_QSPI_MODULE_ENABLED
