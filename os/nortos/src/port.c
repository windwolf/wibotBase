#include "os.h"
#include "bsp_shared.h"

void driver_thread_sleep(uint32_t ms)
{
    // FIXME: implement
}

bool driver_mutex_create(DRIVER_MUTEX *mutex, const char *name)
{
    *mutex = 0;
    return true;
}

bool driver_mutex_get(DRIVER_MUTEX *mutex)
{
    while (!*mutex)
    {
    }
    *mutex = 1;
    return true;
}

bool driver_mutex_put(DRIVER_MUTEX *mutex)
{
    *mutex = 0;
    return true;
}

bool driver_events_create(DRIVER_EVENTS *events, const char *name)
{
    *events = 0;
    return true;
}

bool driver_events_get(DRIVER_EVENTS *events, uint32_t flags, DRIVER_EVENTS_OPTION option, uint32_t timeout)
{
    if (option == DRIVER_EVENTS_OPTION_AND)
    {
        if (timeout == DRIVER_TIMEOUT_NOWAIT)
        {
            return ((*events) & flags) == flags;
        }
        else
        {
            while (((*events) & flags) != flags)
            {
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
            while (((*events) & flags) == 0)
            {
            };
            return true;
        }
    }
}

bool driver_events_set(DRIVER_EVENTS *events, uint32_t flags)
{
    *events |= flags;
    return true;
};

bool driver_events_reset(DRIVER_EVENTS *events, uint32_t flags)
{
    *events &= ~flags;
    return true;
};

void driver_delay_ms(uint32_t ms)
{
    bsp_delay_ms(ms);
};
void driver_delay_us(uint32_t us)
{
    bsp_delay_us(0);
};