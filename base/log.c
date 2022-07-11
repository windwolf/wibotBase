#include "log.h"
#include "string.h"
#include "stdarg.h"

#pragma GCC diagnostic ignored "-Wformat="
inline void logprint(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
    va_end(args);
}