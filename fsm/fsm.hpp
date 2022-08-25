#ifndef __WWBASE_FSM_HPP__
#define __WWBASE_FSM_HPP__

#include "base.hpp"
#include "eventgroup.hpp"
#include "stdint.h"

namespace ww::fsm
{
#define FSM_MAX_STATES_COUNT (256)
#define FSM_MAX_TRANSITIONS_COUNT (256)
#define FSM_MAX_TRANSITIONS_COUNT_PRE_STATE (8)

struct FSM_State_Config;
class FSM_State;
struct FSM_Transition_Config;
class FSM_Transition;
class FSM;

enum FSM_STATE_MODE : uint8_t
{
    FSM_STATE_MODE_INTERVAL = 1U,
    FSM_STATE_MODE_POLL = 2U,
};

enum FSM_TRANSITION_MODE : uint8_t
{
    FSM_TRANSITION_MODE_TIMEOUT = 1U,
    FSM_TRANSITION_MODE_EVENT = 2U,
};

typedef void (*FSM_Action)(FSM &, FSM_State *);
typedef bool (*FSM_Guard)(FSM &, FSM_State *);

struct FSM_State_Config
{
    const char *name;
    FSM_Action entry_action;
    FSM_Action exit_action;
    uint32_t polling_interval;
    FSM_Action poll_action;
    /**
     * @brief parent state is used to simplify transition configuration.
     * parent represents the template of the state,
     * all the children states has the transitions and actions of the parent
     * state.
     */
    uint8_t parent_state_no;

    uint8_t state_no; // 0 means not registered
};

class FSM_State
{

  public:
    friend class FSM;
    friend class FSM_Transition;
    FSM_State(){};
    FSM_State_Config &config_get();

  private:
    FSM_State_Config _config;
    uint32_t _enter_tick;
    uint32_t _last_polling_tick;
    FSM_State *_parent;
#ifdef FSM_TRANSITION_PREFILTER
    struct FSM_Transition *_transitions[FSM_MAX_TRANSITIONS_COUNT_PRE_STATE];
    uint8_t _transition_count;
#endif

    Result _init(FSM &fsm);
    void poll(FSM &fsm);
    void do_poll(FSM &fsm);
    void enter(FSM &fsm, FSM_State *fromState);
    void do_enter(FSM &fsm);
    void exit(FSM &fsm, FSM_State *toState);
    void do_exit(FSM &fsm);
    bool is_parent_of(FSM_State *state);

    bool transit_check(FSM &fsm);
};

struct FSM_Transition_Config
{
    const char *name;
    FSM_TRANSITION_MODE mode;
    union {
        EventFlag events;
        uint32_t timeout;
    } mode_parameters;
    uint8_t from;
    uint8_t to;
    FSM_Guard guard;
    FSM_Action action;
};

class FSM_Transition
{
  public:
    friend class FSM;
    friend class FSM_State;
    FSM_Transition(){};
    FSM_Transition_Config &config_get();

  private:
    FSM_Transition_Config _config;
    FSM_State *_to;
    Result _init(FSM &fsm);
    bool _do_event_check(FSM &fsm, FSM_State *fromState);
    bool _do_timeout_check(FSM &fsm, FSM_State *fromState, uint32_t duration);
};

class FSM
{
  public:
    friend class FSM_State;
    friend class FSM_Transition;
    FSM(const char *name, uint32_t eventClearMask, FSM_State (&states)[],
        uint32_t stateCount, FSM_Transition (&transitions)[],
        uint32_t transitionCount);

    Result init();
    Result deinit();

    Result start(uint32_t stateNo, void *userData, uint32_t initialTick);

    void event_set(uint32_t events);

    void event_reset(uint32_t events);

    void update(uint32_t tick);

    void update_inc(uint32_t tickInc);

  private:
    const char *_name;
    EventGroup _events;
    FSM_State (&_states)[];
    uint8_t _stateCount;
    FSM_Transition (&_transitions)[];
    uint8_t transitionCount;

    FSM_State *_currentState;
    // uint32_t current_state_enter_tick;
    uint32_t _lastUpdateTick;
    uint32_t _currentTick;
    void *_userData;

    FSM_State *_find_state_by_no(uint8_t stateNo);
    void _transition_check(FSM_State &state);
};

} // namespace ww::fsm

#endif // __WWBASE_FSM_HPP__