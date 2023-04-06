//
// Created by zhouj on 2023/4/6.
//

#include <cstdarg>
#include "Logger.hpp"
#include <cstdio>
#include <cstring>
namespace wibot {

#define LOG_CTRL_TEXT_BRIGHT_BLACK   "\x1B[1;30m"
#define LOG_CTRL_TEXT_BRIGHT_RED     "\x1B[1;31m"
#define LOG_CTRL_TEXT_BRIGHT_GREEN   "\x1B[1;32m"
#define LOG_CTRL_TEXT_BRIGHT_YELLOW  "\x1B[1;33m"
#define LOG_CTRL_TEXT_BRIGHT_BLUE    "\x1B[1;34m"
#define LOG_CTRL_TEXT_BRIGHT_MAGENTA "\x1B[1;35m"
#define LOG_CTRL_TEXT_BRIGHT_CYAN    "\x1B[1;36m"
#define LOG_CTRL_TEXT_BRIGHT_WHITE   "\x1B[1;37m"

#define LOG(MODULE, LEVEL, COLOR, FMT, ...) \
    printf(COLOR "[" LEVEL "] [" MODULE "] " FMT "\r\n", ##__VA_ARGS__)

void Logger::errorInterval(int interval, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(LOG_CTRL_TEXT_BRIGHT_RED "[E] [%s] ", name_);
    printf(fmt, args);
    printf("\r\n");
    va_end(args);
}
void Logger::warningInterval(int interval, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(LOG_CTRL_TEXT_BRIGHT_YELLOW "[W] [%s] ", name_);
    printf(fmt, args);
    printf("\r\n");
    va_end(args);
}
void Logger::infoInterval(int interval, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(LOG_CTRL_TEXT_BRIGHT_WHITE "[I] [%s] ", name_);
    printf(fmt, args);
    printf("\r\n");
    va_end(args);
}
void Logger::debugInterval(int interval, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf(LOG_CTRL_TEXT_BRIGHT_MAGENTA "[D] [%s] ", name_);
    printf(fmt, args);
    printf("\r\n");
    va_end(args);
}

}  // namespace wibot
