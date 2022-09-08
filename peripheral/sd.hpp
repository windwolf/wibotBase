#ifndef ___BSP_SD_HPP__
#define ___BSP_SD_HPP__

#include "base.hpp"
#include "peripheral.hpp"
#include "accessor/block.hpp"
namespace ww::peripheral
{
using namespace ww;
using namespace ww::accessor;

SD_PER_DECL

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
    SdCard(SD_CTOR_ARG);
    Result _init() override;
    void _deinit() override;
    SdConfig &config_get();
    CardInfo &card_info_get();
    Result read(void *data, uint32_t num, uint32_t count, WaitHandler WaitHandler);
    Result write(void *data, uint32_t num, uint32_t count, WaitHandler WaitHandler);
    Result erase(uint32_t num, uint32_t count, WaitHandler WaitHandler);

    Result status_query();
    Result card_init();

  private:
    SD_FIELD_DECL
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
    static void _on_read_complete_callback(SD_CALLBACK_ARG);
    static void _on_write_complete_callback(SD_CALLBACK_ARG);
    static void _on_error_callback(SD_CALLBACK_ARG);

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
    Result media_read(void *data, uint32_t num, uint32_t size,
                      WaitHandler &waitHandler) override;
    Result media_write(void *data, uint32_t num, uint32_t size,
                       WaitHandler &waitHandler) override;
    Result media_erase(uint32_t num, uint32_t size, WaitHandler &waitHandler) override;
}

} // namespace ww::peripheral

#endif // ___BSP_SD_HPP__