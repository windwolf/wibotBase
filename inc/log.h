
#ifndef ___LOG_H__
#define ___LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

#ifdef DEBUG

#define LOG(LEVEL, MODULE, MSG, ...) SEGGER_RTT_printf(0, "[" LEVEL "][" MODULE "] " MSG "\r\n", ##__VA_ARGS__)

#else

#define LOG(LEVEL, MODULE, MSG, ...)

#endif

#define LOG_I(MODULE, MSG, ...) LOG("I", MODULE, MSG, ##__VA_ARGS__)

#define LOG_I_INTERVAL(MODULE, INTERVAL, DUMSG, ...) \
    {                                                \
        static uint32_t _log_count_ = 0;             \
        if (_log_count_ == 0)                        \
        {                                            \
            LOG_I(MODULE, DUMSG, ##__VA_ARGS__);     \
        }                                            \
        _log_count_++;                               \
        if (_log_count_ >= INTERVAL)                 \
        {                                            \
            _log_count_ = 0;                         \
        }                                            \
    }

#ifdef __cplusplus
}
#endif

#endif // ___LOG_H__
