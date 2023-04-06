//
// Created by zhouj on 2023/1/28.
//

#ifndef AQ_WF_2022_LIBS_WWBASE_ARCH_ARCH_HPP_
#define AQ_WF_2022_LIBS_WWBASE_ARCH_ARCH_HPP_

#include "stdint.h"

namespace wibot::arch {
bool     sync_compare_and_swap(volatile uint32_t* ptr, uint32_t old_value, uint32_t new_value);
uint32_t getUint32(uint8_t* data, bool littleEndian);
void     setUint32(uint8_t* data, uint32_t value, bool littleEndian);
uint16_t getUint16(uint8_t* data, bool littleEndian);
void     setUint16(uint8_t* data, uint16_t value, bool littleEndian);
}  // namespace wibot::arch

#endif  // AQ_WF_2022_LIBS_WWBASE_ARCH_ARCH_HPP_
