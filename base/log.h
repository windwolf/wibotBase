#ifndef ___LOG_H__
#define ___LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LOG_MODULE
#ifndef LOG_MODULE__
#define LOG_MODULE__ LOG_MODULE
#endif
#else
#define LOG_MODULE__ "LOG"
#endif

#ifdef LOG_LEVEL
#ifndef LOG_LEVEL__
#define LOG_LEVEL__ LOG_LEVEL
#endif
#else
#define LOG_LEVEL__ LOG_LEVEL_DEBUG
#endif

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#define LOG_CTRL_TEXT_BRIGHT_BLACK "\x1B[1;30m"
#define LOG_CTRL_TEXT_BRIGHT_RED "\x1B[1;31m"
#define LOG_CTRL_TEXT_BRIGHT_GREEN "\x1B[1;32m"
#define LOG_CTRL_TEXT_BRIGHT_YELLOW "\x1B[1;33m"
#define LOG_CTRL_TEXT_BRIGHT_BLUE "\x1B[1;34m"
#define LOG_CTRL_TEXT_BRIGHT_MAGENTA "\x1B[1;35m"
#define LOG_CTRL_TEXT_BRIGHT_CYAN "\x1B[1;36m"
#define LOG_CTRL_TEXT_BRIGHT_WHITE "\x1B[1;37m"

#include "stdio.h"
#include "string.h"
#define LOG(MODULE, LEVEL, COLOR, FMT, ...) printf(COLOR "[" LEVEL "] [" MODULE "] " FMT "\r\n", ##__VA_ARGS__)

#if (LOG_LEVEL__ >= LOG_LEVEL_ERROR)
#define LOG_E(FMT, ...) LOG(LOG_MODULE__, "E", LOG_CTRL_TEXT_BRIGHT_RED, FMT, ##__VA_ARGS__)
#define LOG_E_INTERVAL(INTERVAL, FMT, ...) \
    {                                      \
        static uint32_t _log_count_ = 0;   \
        if (_log_count_ == 0)              \
        {                                  \
            LOG_E(FMT, ##__VA_ARGS__);     \
        }                                  \
        _log_count_++;                     \
        if (_log_count_ >= INTERVAL)       \
        {                                  \
            _log_count_ = 0;               \
        }                                  \
    }
#else
#define LOG_E(FMT, ...)
#define LOG_E_INTERVAL(INTERVAL, FMT, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_WARNING)
#define LOG_W(FMT, ...) LOG(LOG_MODULE__, "W", LOG_CTRL_TEXT_BRIGHT_YELLOW, FMT, ##__VA_ARGS__)
#define LOG_W_INTERVAL(INTERVAL, FMT, ...) \
    {                                      \
        static uint32_t _log_count_ = 0;   \
        if (_log_count_ == 0)              \
        {                                  \
            LOG_W(FMT, ##__VA_ARGS__);     \
        }                                  \
        _log_count_++;                     \
        if (_log_count_ >= INTERVAL)       \
        {                                  \
            _log_count_ = 0;               \
        }                                  \
    }
#else
#define LOG_W(FMT, ...)
#define LOG_W_INTERVAL(INTERVAL, FMT, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_INFO)
#define LOG_I(FMT, ...) LOG(LOG_MODULE__, "I", LOG_CTRL_TEXT_BRIGHT_WHITE, FMT, ##__VA_ARGS__)
#define LOG_I_INTERVAL(INTERVAL, FMT, ...) \
    {                                      \
        static uint32_t _log_count_ = 0;   \
        if (_log_count_ == 0)              \
        {                                  \
            LOG_I(FMT, ##__VA_ARGS__);     \
        }                                  \
        _log_count_++;                     \
        if (_log_count_ >= INTERVAL)       \
        {                                  \
            _log_count_ = 0;               \
        }                                  \
    }
#else
#define LOG_I(FMT, ...)
#define LOG_I_INTERVAL(INTERVAL, FMT, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_DEBUG)
#define LOG_D(FMT, ...) LOG(LOG_MODULE__, "D", LOG_CTRL_TEXT_BRIGHT_MAGENTA, FMT, ##__VA_ARGS__)
#define LOG_D_INTERVAL(INTERVAL, FMT, ...) \
    {                                      \
        static uint32_t _log_count_ = 0;   \
        if (_log_count_ == 0)              \
        {                                  \
            LOG_D(FMT, ##__VA_ARGS__);     \
        }                                  \
        _log_count_++;                     \
        if (_log_count_ >= INTERVAL)       \
        {                                  \
            _log_count_ = 0;               \
        }                                  \
    }
#else
#define LOG_D(FMT, ...)
#define LOG_D_INTERVAL(INTERVAL, FMT, ...)
#endif

#ifdef __cplusplus
}
#endif

#endif // ___LOG_H__
