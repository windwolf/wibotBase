//
// Created by zhouj on 2023/1/28.
//

#include "arch.hpp"

#include "peripheral.hpp"
namespace wibot::arch {
bool sync_compare_and_swap(volatile uint32_t* ptr, uint32_t old_value, uint32_t new_value) {
    //        uint32_t oldval, res;
    //        __asm__ __volatile__("@ sync_compare_and_swap\n"
    //                             "ldrex  %1, [%2]\n"
    //                             "mov    %0, #0\n"
    //                             "teq    %1, %3\n"
    //                             "strexeq %0, %4, [%2]\n"
    //            : "=&r" (res), "=&r" (oldval)
    //            : "r" (ptr), "Ir" (old_value), "r" (new_value)
    //            : "cc");
    //        return res;
    auto prim = __get_PRIMASK();
    __disable_irq();
    if (*ptr == old_value) {
        *ptr = new_value;
        if (!prim) {
            __enable_irq();
        }
        return true;
    } else {
        if (!prim) {
            __enable_irq();
        }
        return false;
    }
};

uint32_t getUint32(uint8_t* data, bool littleEndian) {
    if (littleEndian) {
        return *static_cast<uint32_t*>(static_cast<void*>(data));
    } else {
        return (data[0] << 24) + (data[1] << 16) + (data[2] << 8) + data[3];
    }
};

void setUint32(uint8_t* data, uint32_t value, bool littleEndian) {
    if (littleEndian) {
        *static_cast<uint32_t*>(static_cast<void*>(data)) = value;
    } else {
        data[0] = (value >> 24) & 0xFF;
        data[1] = (value >> 16) & 0xFF;
        data[2] = (value >> 8) & 0xFF;
        data[3] = value & 0xFF;
    }
};

uint16_t getUint16(uint8_t* data, bool littleEndian) {
    if (littleEndian) {
        return *static_cast<uint16_t*>(static_cast<void*>(data));
    } else {
        return (data[0] << 8) + data[1];
    }
};
void setUint16(uint8_t* data, uint16_t value, bool littleEndian) {
    if (littleEndian) {
        *static_cast<uint16_t*>(static_cast<void*>(data)) = value;
    } else {
        data[0] = (value >> 8) & 0xFF;
        data[1] = value & 0xFF;
    }
}
}  // namespace wibot::arch
