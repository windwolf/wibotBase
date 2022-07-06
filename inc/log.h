
#ifndef ___LOG_H__
#define ___LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef DEBUG
#include "stdio.h"
#include "SEGGER_RTT.h"
#define LOG(LEVEL, COLOR, MODULE, FMT, ...) SEGGER_RTT_printf(0, COLOR "[" LEVEL "] [" MODULE "] " FMT "\r\n", ##__VA_ARGS__)

#else

#define LOG(LEVEL, COLOR, MODULE, MSG, ...)

#endif

#define LOG_I(MODULE, FMT, ...) LOG("I", RTT_CTRL_TEXT_BRIGHT_GREEN, MODULE, FMT, ##__VA_ARGS__)

#define LOG_I_INTERVAL(MODULE, INTERVAL, FMT, ...) \
    {                                              \
        static uint32_t _log_count_ = 0;           \
        if (_log_count_ == 0)                      \
        {                                          \
            LOG_I(MODULE, FMT, ##__VA_ARGS__);     \
        }                                          \
        _log_count_++;                             \
        if (_log_count_ >= INTERVAL)               \
        {                                          \
            _log_count_ = 0;                       \
        }                                          \
    }

#define LOG_E(MODULE, FMT, ...) LOG("E", RTT_CTRL_TEXT_BRIGHT_RED, MODULE, FMT, ##__VA_ARGS__)

#define LOG_E_INTERVAL(MODULE, INTERVAL, FMT, ...) \
    {                                              \
        static uint32_t _log_count_ = 0;           \
        if (_log_count_ == 0)                      \
        {                                          \
            LOG_E(MODULE, FMT, ##__VA_ARGS__);     \
        }                                          \
        _log_count_++;                             \
        if (_log_count_ >= INTERVAL)               \
        {                                          \
            _log_count_ = 0;                       \
        }                                          \
    }

#ifdef __cplusplus
}
#endif

#endif // ___LOG_H__
