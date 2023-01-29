//
// Created by zhouj on 2023/1/28.
//

#include "arch.hpp"
namespace wibot::arch
{
    bool sync_compare_and_swap(volatile uint32_t* ptr, uint32_t old_value, uint32_t new_value)
    {
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
        if (*ptr == old_value)
        {
            *ptr = new_value;
            return true;
        }
        else
        {
            return false;
        }

    };
}


