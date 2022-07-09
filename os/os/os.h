#ifndef ___OS_H__
#define ___OS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "port.h"
#include <stdbool.h>
#include <stdint.h>

#define DRIVER_TIMEOUT_FOREVER (uint32_t)(0xFFFFFFFFUL)
#define DRIVER_TIMEOUT_NOWAIT (uint32_t)(0x00000000UL)

    void ww_os_thread_sleep(uint32_t ms);

    bool ww_os_mutex_create(DRIVER_MUTEX *mutex, const char *name);

    bool ww_os_mutex_get(DRIVER_MUTEX *mutex);

    bool ww_os_mutex_put(DRIVER_MUTEX *mutex);

    typedef enum DRIVER_EVENTS_OPTION
    {
        DRIVER_EVENTS_OPTION_AND = 2,
        DRIVER_EVENTS_OPTION_OR = 0

    } DRIVER_EVENTS_OPTION;

    bool ww_os_events_create(DRIVER_EVENTS *events, const char *name);

    bool ww_os_events_get(DRIVER_EVENTS *events, uint32_t flags, DRIVER_EVENTS_OPTION option, uint32_t timeout);

    bool ww_os_events_set(DRIVER_EVENTS *events, uint32_t flags);

    bool ww_os_events_reset(DRIVER_EVENTS *events, uint32_t flags);

    void ww_os_delay(uint32_t ms);

    uint32_t ww_os_tick_get(void);

#ifdef __cplusplus
}
#endif

#endif // ___OS_H__