#ifndef __WWBASE_FSM_EVENTGROUP_HPP__
#define __WWBASE_FSM_EVENTGROUP_HPP__

#include "stdint.h"
namespace ww::fsm
{

struct FSM_EventFlag
{
    uint32_t mask;
    uint32_t value;
};

class FSM_EventGroup
{
  public:
    FSM_EventGroup(uint32_t eventsClearMask)
        : _eventsClearMask(eventsClearMask){};
    ;
    bool check(FSM_EventFlag &eventFlags);
    void set(uint32_t events);
    void reset(uint32_t events);
    void update_begin();
    void update_end();

  private:
    uint32_t _events;
    uint32_t _eventsClearMask;
};

} // namespace ww::fsm

#endif // __WWBASE_FSM_EVENTGROUP_HPP__