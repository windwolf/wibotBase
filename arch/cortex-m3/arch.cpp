//
// Created by zhouj on 2023/1/28.
//

#include "arch.hpp"
namespace wibot::arch
{
    bool sync_compare_and_swap(volatile uint32_t* ptr, uint32_t old_value, uint32_t new_value)
    {
        return __sync_bool_compare_and_swap(ptr, old_value, new_value);
    };
}
