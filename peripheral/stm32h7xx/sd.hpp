#ifndef ___BSP_SD_HPP__
#define ___BSP_SD_HPP__

#include "base.hpp"
#include "wait_handler.hpp"
#include "accessor/block.hpp"
#include "stm32h7xx_hal.h"
namespace ww::peripheral
{
using namespace ww;
using namespace ww::accessor;

typedef HAL_SD_CardInfoTypeDef CardInfo;

union SdConfig {
    struct
    {
        bool useTxDma : 1;
        bool useRxDma : 1;
    };
    uint32_t value;
};

class SdCard : Initializable
{
  public:
    SdCard(SD_HandleTypeDef &handle);
    ~SdCard();
    SdConfig &config_get();
    CardInfo &card_info_get();
    Result read(void *data, uint32_t num, uint32_t count, WaitHandler WaitHandler);
    Result write(void *data, uint32_t num, uint32_t count, WaitHandler WaitHandler);
    Result erase(uint32_t num, uint32_t count, WaitHandler WaitHandler);

    Result status_query();
    Result card_init();

  private:
    SD_HandleTypeDef &_handle;
    union {
        struct
        {
            bool isTxDmaEnabled : 1;
            bool isRxDmaEnabled : 1;
        };
        uint32_t value;
    } _status;
    SdConfig _config;
    CardInfo _cardInfo;
    WaitHandler *_waitHandler;
    Buffer _txBuffer;
    BUffer _rxBuffer;

  protected:
    static void _on_read_complete_callback(SD_HandleTypeDef *instance);
    static void _on_write_complete_callback(SD_HandleTypeDef *instance);
    static void _on_error_callback(SD_HandleTypeDef *instance);

}

class SdCardBlock : public Block
{
  public:
    SdCardBlock(SdCard &sdcard, Buffer buffer);
    Result card_init();

  private:
    SdCard &_instance;
    Buffer _buffer;

  protected:
    virtual Result media_read(void *data, uint32_t num, uint32_t size, WaitHandler &waitHandler) = 0;
    virtual Result media_write(void *data, uint32_t num, uint32_t size, WaitHandler &waitHandler) = 0;
    virtual Result media_erase(uint32_t num, uint32_t size, WaitHandler &waitHandler) = 0;
}

} // namespace ww::peripheral

#endif // ___BSP_SD_HPP__