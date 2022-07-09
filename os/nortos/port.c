#include "os/os.h"
#include "bsp_shared.h"

void ww_os_thread_sleep(uint32_t ms)
{
    volatile uint32_t start = ww_os_tick_get();
    if (ww_os_tick_get() - start < ms)
    {
    }
    return;
}

bool ww_os_mutex_create(DRIVER_MUTEX *mutex, const char *name)
{
    *mutex = 0;
    return true;
}

bool ww_os_mutex_get(DRIVER_MUTEX *mutex)
{
    while (!*mutex)
    {
    }
    *mutex = 1;
    return true;
}

bool ww_os_mutex_put(DRIVER_MUTEX *mutex)
{
    *mutex = 0;
    return true;
}

bool ww_os_events_create(DRIVER_EVENTS *events, const char *name)
{
    *events = 0;
    return true;
}

bool ww_os_events_get(DRIVER_EVENTS *events, uint32_t flags, DRIVER_EVENTS_OPTION option, uint32_t timeout)
{

    if (option == DRIVER_EVENTS_OPTION_AND)
    {
        if (timeout == DRIVER_TIMEOUT_NOWAIT)
        {
            return ((*events) & flags) == flags;
        }
        else
        {
            uint32_t start = ww_os_tick_get();
            while (((*events) & flags) != flags)
            {
                if (ww_os_tick_get() - start > timeout)
                {
                    return false;
                }
            };
            return true;
        }
    }
    else
    {
        if (timeout == DRIVER_TIMEOUT_NOWAIT)
        {
            return ((*events) & flags) != 0;
        }
        else
        {
            uint32_t start = ww_os_tick_get();
            while (((*events) & flags) == 0)
            {
                if (ww_os_tick_get() - start > timeout)
                {
                    return false;
                }
            };
            return true;
        }
    }
}

bool ww_os_events_set(DRIVER_EVENTS *events, uint32_t flags)
{
    *events |= flags;
    return true;
};

bool ww_os_events_reset(DRIVER_EVENTS *events, uint32_t flags)
{
    *events &= ~flags;
    return true;
};

void ww_os_delay(uint32_t ms)
{
    bsp_delay_ms(ms);
};
uint32_t ww_os_tick_get(void)
{
    return bsp_get_tick_count();
}