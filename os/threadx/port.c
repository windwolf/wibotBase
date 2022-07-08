#include "os/os.h"
#include "tx_api.h"

void driver_thread_sleep(uint32_t ms)
{
    tx_thread_sleep(ms);
}

bool driver_mutex_create(DRIVER_MUTEX *mutex, const char *name)
{
    return tx_mutex_create(mutex, name, 0) == TX_SUCCESS;
}

bool driver_mutex_get(DRIVER_MUTEX *mutex)
{
    return tx_mutex_get(mutex, TX_WAIT_FOREVER) == TX_SUCCESS;
}

bool driver_mutex_put(DRIVER_MUTEX *mutex)
{
    return tx_mutex_put(mutex) == TX_SUCCESS;
}

bool driver_events_create(DRIVER_EVENTS *events, const char *name)
{
    return tx_event_flags_create(events, name) == TX_SUCCESS;
};

bool driver_events_get(DRIVER_EVENTS *events, uint32_t flags, DRIVER_EVENTS_OPTION option, uint32_t timeout)
{
    ULONG actualFlags;
    return tx_event_flags_get(events, flags, option, &actualFlags, timeout) == TX_SUCCESS;
};

bool driver_events_set(DRIVER_EVENTS *events, uint32_t flags)
{
    return tx_event_flags_set(events, flags, TX_OR) == TX_SUCCESS;
};

bool driver_events_reset(DRIVER_EVENTS *events, uint32_t flags)
{
    return tx_event_flags_set(events, flags, TX_AND) == TX_SUCCESS;
};

void driver_delay_ms(uint32_t ms)
{
    tx_thread_sleep(ms);
};
void driver_delay_us(uint32_t us)
{
    tx_thread_sleep(0);
};