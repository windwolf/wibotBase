//
// Created by zhouj on 2022/12/5.
//

#ifndef WWMOTOR_LIBS_WWBASE_BASE_UTILS_HPP_
#define WWMOTOR_LIBS_WWBASE_BASE_UTILS_HPP_

#include "base.hpp"

namespace wibot
{

    class StopWatch
    {
     public:
        void start();
        uint64_t tick();

     private :
        uint64_t _start_time;
    };

} // wibot

#endif //WWMOTOR_LIBS_WWBASE_BASE_UTILS_HPP_
