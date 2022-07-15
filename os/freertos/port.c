#include "os/os.h"
#include "bsp_shared.h"
#include "cmsis_os2.h"

void ww_os_thread_sleep(uint32_t ms)
{
    osDelay(ms);
}

bool ww_os_mutex_create(DRIVER_MUTEX *mutex, const char *name)
{
    osMutexAttr_t attr = {
        .cb_mem = mutex,
        .cb_size = sizeof(DRIVER_MUTEX),
        .name = name,
    };
    return osMutexNew(&attr) != NULL;
}

bool ww_os_mutex_get(DRIVER_MUTEX *mutex)
{
    return osMutexAcquire(mutex, osWaitForever) == osOK;
}

bool ww_os_mutex_put(DRIVER_MUTEX *mutex)
{
    return osMutexRelease(mutex) == osOK;
}

bool ww_os_events_create(DRIVER_EVENTS *events, const char *name)
{
    osEventFlagsAttr_t attr = {
        .cb_mem = events,
        .cb_size = sizeof(DRIVER_EVENTS),
        .name = name,
    };
    return osEventFlagsNew(&attr) != NULL;
}

bool ww_os_events_get(DRIVER_EVENTS *events, uint32_t flags, DRIVER_EVENTS_OPTION option, uint32_t timeout)
{
    return osEventFlagsWait(events, flags,
                            (option == DRIVER_EVENTS_OPTION_AND) ? osFlagsWaitAll : 0,
                            timeout) == osOK;
}

bool ww_os_events_set(DRIVER_EVENTS *events, uint32_t flags)
{
    return osEventFlagsSet(events, flags) == osOK;
};

bool ww_os_events_reset(DRIVER_EVENTS *events, uint32_t flags)
{
    return osEventFlagsClear(events, flags) == osOK;
};

void ww_os_delay(uint32_t ms)
{
    osDelay(ms);
};

uint32_t ww_os_tick_get(void)
{
    return osKernelGetTickCount();
}