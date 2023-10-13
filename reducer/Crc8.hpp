//
// Created by zhouj on 2023/2/21.
//

#ifndef BASE_CRC8_HPP_
#define BASE_CRC8_HPP_

#include "base.hpp"
#include "validator.hpp"

namespace wibot {

class Crc8Validator : Validator<uint8_t> {
   public:
    Crc8Validator(uint8_t poly, uint8_t init = 0x00, uint8_t xorout = 0x00, bool ref_in = false,
                  bool ref_out = false)
        : poly_(poly), init_(init), xorout_(xorout), ref_in_(ref_in), ref_out_(ref_out){};
    void reset() override;
    void calculate(uint8_t* data, uint32_t length) override;
    bool validate(uint8_t* sum) override;
    uint8_t get();
   private:
    uint8_t poly_;
    uint8_t init_;
    uint8_t xorout_;
    bool    ref_in_;
    bool    ref_out_;
    uint8_t crc_;

   public:
    constexpr static uint8_t CRC8_DVB_S2       = 0xD5;
    constexpr static uint8_t CRC8_AUTOSAR      = 0x2F;
    constexpr static uint8_t CRC8_BLUETOOTH    = 0xA7;
    constexpr static uint8_t CRC8_CCITT        = 0x07;
    constexpr static uint8_t CRC8_DALLAS_MAXIM = 0x31;
    constexpr static uint8_t CRC8_DARC         = 0x39;
    constexpr static uint8_t CRC8_GSM_B        = 0x49;
    constexpr static uint8_t CRC8_SAE_J1850    = 0x1D;
    constexpr static uint8_t CRC8_WCDMA        = 0x9B;
    constexpr static uint8_t CRC8_GSM_A        = 0x37;
};

}  // namespace wibot

#endif  // BASE_CRC8_HPP_
