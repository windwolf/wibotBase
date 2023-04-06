//
// Created by zhouj on 2023/4/6.
//

#ifndef WWMOTOR_LIBS_WWBASE_BASE_LOGGER_HPP_
#define WWMOTOR_LIBS_WWBASE_BASE_LOGGER_HPP_

namespace wibot {

enum class LogLevel {
    None    = 0,
    Error   = 1,
    Warning = 2,
    Info    = 3,
    Debug   = 4,
};

class Logger {
   private:
    const char* name_;

   public:
    explicit Logger(const char* name) : name_(name) {
    }
    //    void error(const char* fmt, ...);
    //    void warning(const char* fmt, ...);
    //    void info(const char* fmt, ...);
    //    void debug(const char* fmt, ...);

    void errorInterval(int interval, const char* fmt, ...);
    void warningInterval(int interval, const char* fmt, ...);
    void infoInterval(int interval, const char* fmt, ...);
    void debugInterval(int interval, const char* fmt, ...);

    static void setLevelThreshold(LogLevel level);
    static bool filterModule(const char* name);
};

#ifdef LOG_LEVEL
#ifndef LOG_LEVEL__
#define LOG_LEVEL__ LOG_LEVEL
#endif
#else
#define LOG_LEVEL__ LOG_LEVEL_DEBUG
#endif

#define LOG_LEVEL_NONE    0
#define LOG_LEVEL_ERROR   1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_DEBUG   4

#define CREATE_LOGGER(name) static Logger logger_(name);

#if (LOG_LEVEL__ >= LOG_LEVEL_ERROR)
#define LOG_E(FMT, ...)                    logger_.errorInterval(1, FMT, ##__VA_ARGS__)
#define LOG_E_INTERVAL(INTERVAL, FMT, ...) logger_.errorInterval(INTERVAL, FMT, ##__VA_ARGS__)
#else
#define LOG_E(FMT, ...)
#define LOG_E_INTERVAL(INTERVAL, FMT, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_WARNING)
#define LOG_W(FMT, ...)                    logger_.warningInterval(1, FMT, ##__VA_ARGS__)
#define LOG_W_INTERVAL(INTERVAL, FMT, ...) logger_.warningInterval(INTERVAL, FMT, ##__VA_ARGS__)
#else
#define LOG_W(FMT, ...)
#define LOG_W_INTERVAL(INTERVAL, FMT, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_INFO)
#define LOG_I(FMT, ...)                    logger_.infoInterval(1, FMT, ##__VA_ARGS__)
#define LOG_I_INTERVAL(INTERVAL, FMT, ...) logger_.infoInterval(INTERVAL, FMT, ##__VA_ARGS__)
#else
#define LOG_I(FMT, ...)
#define LOG_I_INTERVAL(INTERVAL, FMT, ...)
#endif

#if (LOG_LEVEL__ >= LOG_LEVEL_DEBUG)
#define LOG_D(FMT, ...)                    logger_.debugInterval(1, FMT, ##__VA_ARGS__)
#define LOG_D_INTERVAL(INTERVAL, FMT, ...) logger_.debugInterval(INTERVAL, FMT, ##__VA_ARGS__)
#else
#define LOG_D(FMT, ...)
#define LOG_D_INTERVAL(INTERVAL, FMT, ...)
#endif

}  // namespace wibot

#endif  // WWMOTOR_LIBS_WWBASE_BASE_LOGGER_HPP_
