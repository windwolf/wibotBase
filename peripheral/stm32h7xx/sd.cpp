#include "sd.hpp"

namespace wibot::peripheral
{

void SdCard::_on_write_complete_callback(SD_HandleTypeDef *instance)
{
    SdCard *perip = (SdCard *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_waitHandler;
    if (wh != nullptr)
    {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};

void SdCard::_on_read_complete_callback(SD_HandleTypeDef *instance)
{
    SdCard *perip = (SdCard *)Peripherals::peripheral_get_by_instance(instance);
    auto wh = perip->_waitHandler;
    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data, perip->_rxBuffer.size);
    }
    if (wh != nullptr)
    {
        perip->_waitHandler = nullptr;
        wh->done_set(perip);
    }
};

void SdCard::_on_error_callback(SD_HandleTypeDef *instance)
{
    SdCard *perip = (SdCard *)Peripherals::peripheral_get_by_instance(instance);

    if (perip->_status.isRxDmaEnabled)
    {
        SCB_InvalidateDCache_by_Addr(perip->_rxBuffer.data, perip->_rxBuffer.size);
    }
    auto wh = perip->_waitHandler;
    if (wh != nullptr)
    {
        perip->_waitHandler = nullptr;
        wh->set_value((void *)instance->ErrorCode);
        wh->error_set(perip);
    }
};

SdCard::SdCard(SD_HandleTypeDef &handle) : _handle(handle){};

Result SdCard::_init()
{
    HAL_SD_RegisterCallback(instance, HAL_SD_RX_CPLT_CB_ID, &_on_read_complete_callback);
    HAL_SD_RegisterCallback(instance, HAL_SD_TX_CPLT_CB_ID, &_on_write_complete_callback);
    HAL_SD_RegisterCallback(instance, HAL_SD_ERROR_CB_ID, &_on_error_callback);
    Peripherals::peripheral_register("sd", this, &_handle);
    return Result::OK;
};
void SdCard::_deinit()
{
    Peripherals::peripheral_unregister("sd", this);
};

SdConfig &SdCard::config_get()
{
    return _config;
};
CardInfo &SdCard::card_info_get()
{
    return _cardInfo;
};

Result SdCard::card_init()
{
    Result rst = Result::OK;
    rst = HAL_SD_InitCard(&_handle);
    if (rst != Result::OK)
    {
        rst;
    }
    rst = HAL_SD_GetCardInfo(&_handle, &_cardInfo);
    return rst;
};
Result SdCard::read(void *data, uint32_t num, uint32_t count, WaitHandler waitHandler)
{
    Result rst = Result::OK;
    if (_waitHandler != nullptr)
    {
        return Result::Busy;
    }
    _waitHandler = &waitHandler;
    _rxBuffer.data = data;
    _rxBuffer.size = _cardInfo.blockSize * count;
    if (_config.useRxDma)
    {
        return (Result)HAL_SD_ReadBlocks_DMA(&_handle, (uint8_t *)data, num, count);
    }
    else
    {
        return (Result)HAL_SD_ReadBlocks_IT(&_handle, (uint8_t *)data, num, count);
    }
};
Result SdCard::write(void *data, uint32_t num, uint32_t count, WaitHandler waitHandler)
{
    Result rst = Result::OK;
    if (_waitHandler != nullptr)
    {
        return Result::Busy;
    }
    _waitHandler = &waitHandler;
    if (_config.useTxDma)
    {
        SCB_CleanDCache_by_Addr((uint32_t *)data, _cardInfo.blockSize * count);
        return (Result)HAL_SD_WriteBlocks_DMA(&_handle, data, num, count);
    }
    else
    {
        return (Result)HAL_SD_WriteBlocks_IT(&_handle, data, num, count);
    }
};
Result SdCard::erase(uint32_t num, uint32_t count, WaitHandler waitHandler)
{
    Result rst = Result::OK;
    if (_waitHandler != nullptr)
    {
        return Result::Busy;
    }
    _waitHandler = &waitHandler;
    return (Result)HAL_SD_Erase(&_handle, num, num + count);
};

Result SdCard::status_query()
{
    return ((HAL_SD_GetCardState(&_handle) == HAL_SD_CARD_TRANSFER) ? Result::OK : Result::Busy);
};

SdCardBlock::SdCardBlock(SdCard &sdcard, Buffer8 buffer)
    : Block(buffer, ), _sdcard(sdcard),
      _buffer(buffer){BASE_INIT_ERROR_CHECK() MEMBER_INIT_ERROR_CHECK(_sdcard)};

Result SdCardBlock::card_init()
{
    initErrorCode = _sdcard.card_init();
    if (initErrorCode != Result::OK)
    {
        return initErrorCode;
    }
    auto cardInfo = _sdcard.card_info_get();
    config_set(BlockConfig{
        .readBlockSize = cardInfo.blockSize,
        .writeBlockSize = cardInfo.blockSize,
        .eraseBlockSize = cardInfo.blockSize,
        .readMode = BlockMode::Block,
        .writeMode = BlockMode::Block,
        .eraseMode = BlockMode::Block,
        .needEraseBeforeWrite = false,
    });
};

Result SdCardBlock::media_read(void *data, uint32_t num, uint32_t size, WaitHandler &waitHandler)
{
    _instance.read(data, num, size, WaitHandler);
};
Result SdCardBlock::media_write(void *data, uint32_t num, uint32_t size, WaitHandler &waitHandler)
{
    _instance.write(data, num, size, WaitHandler);
};
Result SdCardBlock::media_erase(uint32_t num, uint32_t size, WaitHandler &waitHandler)
{
    _instance.wrase(data, num, size, WaitHandler);
};

} // namespace wibot::peripheral
