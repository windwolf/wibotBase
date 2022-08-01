#include "eventgroup.h"

void EventGroup_init(EventGroup_t *eventGroup, uint32_t eventsClearMask)
{
    eventGroup->eventsClearMask = eventsClearMask;
};

bool EventGroup_check(EventGroup_t *eventGroup, EventFlag_t *eventFlags)
{
    return (eventGroup->events & eventFlags->mask) == (eventFlags->value & eventFlags->mask);
};
void EventGroup_set(EventGroup_t *eventGroup, uint32_t events)
{
    eventGroup->events |= events;
};
void EventGroup_reset(EventGroup_t *eventGroup, uint32_t events)
{
    eventGroup->events &= ~events;
};
void EventGroup_update_begin(EventGroup_t *eventGroup){

};
void EventGroup_update_end(EventGroup_t *eventGroup)
{
    eventGroup->events &= ~eventGroup->eventsClearMask;
};