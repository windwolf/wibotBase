#include "eventgroup.hpp"

namespace ww::fsm
{
bool EventGroup::check(EventFlag &eventFlags)
{
    return (_events & eventFlags.mask) == (eventFlags.value & eventFlags.mask);
};
void EventGroup::set(uint32_t events)
{
    _events |= events;
};
void EventGroup::reset(uint32_t events)
{
    _events &= ~events;
};
void EventGroup::update_begin(){

};
void EventGroup::update_end()
{
    _events &= ~_eventsClearMask;
};
} // namespace ww::fsm