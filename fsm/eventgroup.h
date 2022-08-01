#ifndef __FSM_EVENTGROUP_H__
#define __FSM_EVENTGROUP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"

    typedef struct EventFlag_t
    {
        uint32_t value;
        uint32_t mask;
    } EventFlag_t;

    typedef struct EventGroup_t
    {
        uint32_t events;
        uint32_t eventsClearMask;
    } EventGroup_t;

    void EventGroup_init(EventGroup_t *eventGroup, uint32_t eventsClearMask);
    bool EventGroup_check(EventGroup_t *eventGroup, EventFlag_t *eventFlags);
    void EventGroup_set(EventGroup_t *eventGroup, uint32_t events);
    void EventGroup_reset(EventGroup_t *eventGroup, uint32_t events);
    void EventGroup_update_begin(EventGroup_t *eventGroup);
    void EventGroup_update_end(EventGroup_t *eventGroup);

#ifdef __cplusplus
}
#endif

#endif // __FSM_EVENTGROUP_H__