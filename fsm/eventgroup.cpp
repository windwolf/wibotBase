#include "eventgroup.hpp"

namespace ww::fsm
{
bool FSM_EventGroup::check(FSM_EventFlag &eventFlags)
{
    return (_events & eventFlags.mask) == (eventFlags.value & eventFlags.mask);
};
void FSM_EventGroup::set(uint32_t events)
{
    _events |= events;
};
void FSM_EventGroup::reset(uint32_t events)
{
    _events &= ~events;
};
void FSM_EventGroup::update_begin(){

};
void FSM_EventGroup::update_end()
{
    _events &= ~_eventsClearMask;
};
} // namespace ww::fsm