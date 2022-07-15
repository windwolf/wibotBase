#include "safe_printf.h"
#include "os/os.h"

static DRIVER_MUTEX _printfMutex;

void safe_printf_init()
{
    ww_os_mutex_create(&_printfMutex, "_printfMutex");
}

inline void _begin_safe_printf()
{
    ww_os_mutex_get(&_printfMutex);
}

inline void _end_safe_printf()
{
    ww_os_mutex_put(&_printfMutex);
}