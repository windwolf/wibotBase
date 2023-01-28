//
// Created by zhouj on 2023/1/28.
//

#include "arch.hpp"
namespace wibot::arch
{
    bool sync_compare_and_swap(volatile uint32_t* ptr, uint32_t old_value, uint32_t new_value)
    {
        if (*ptr == old_value)
        {
            *ptr = new_value;
            return true;
        }
        else
        {
            return true;
        }

    };
}


