#ifndef __WWBASE_FSM_HPP__
#define __WWBASE_FSM_HPP__

#include "base.hpp"
#include "eventgroup.hpp"
#include "stdint.h"

namespace wibot::fsm
{
#define FSM_MAX_STATES_COUNT (256)
#define FSM_MAX_TRANSITIONS_COUNT (256)
#define FSM_MAX_TRANSITIONS_COUNT_PRE_STATE (8)

struct FSM_State_Config;
class FSM_State;
struct FSM_Transition_Config;
class FSM_Transition;
class FSM;

enum class FsmStateMode : uint8_t
{
    Interval = 1U,
    Poll = 2U,
};

enum class FsmTransitionMode : uint8_t
{
    Timeout = 1U,
    Event = 2U,
};

typedef void (*FSM_Action)(FSM &, FSM_State *);
typedef bool (*FSM_Guard)(FSM &, FSM_State *);

struct FSM_State_Config
{
    uint8_t stateNo; // 0 means not registered
    const char *name;
    uint8_t parentStateNo;
    uint32_t pollingInterval;
    FSM_Action entryAction;
    FSM_Action exitAction;
    FSM_Action pollAction;
    /**
     * @brief parent state is used to simplify transition configuration.
     * parent represents the template of the state,
     * all the children states has the transitions and actions of the parent
     * state.
     */
};

class FSM_State
{

  public:
    friend class FSM;
    friend class FSM_Transition;
    FSM_State(const FSM_State_Config &config) : _config(config){};
    FSM_State_Config &config_get();
    uint32_t enterTick;
    uint32_t lastPollingTick;

  private:
    FSM_State_Config _config;

    FSM_State *_parent;
#ifdef FSM_TRANSITION_PREFILTER
    struct FSM_Transition *_transitions[FSM_MAX_TRANSITIONS_COUNT_PRE_STATE];
    uint8_t _transitionCount;
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
    uint8_t from;
    uint8_t to;
    FsmTransitionMode mode;
    union {
        FSM_EventFlag events;
        uint32_t timeout;
    };

    FSM_Guard guard;
    FSM_Action action;
};

class FSM_Transition
{
  public:
    friend class FSM;
    friend class FSM_State;
    FSM_Transition(const FSM_Transition_Config &config) : _config(config){};
    FSM_Transition_Config &config_get();

  private:
    FSM_Transition_Config _config;
    FSM_State *_to;
    Result _init(FSM &fsm);
    bool _do_event_check(FSM &fsm, FSM_State *fromState);
    bool _do_timeout_check(FSM &fsm, FSM_State *fromState, uint32_t duration);
};

class FSM : public Initializable
{
  public:
    friend class FSM_State;
    friend class FSM_Transition;
    FSM(const char *name, uint32_t eventClearMask, FSM_State (&states)[], uint32_t stateCount,
        FSM_Transition (&transitions)[], uint32_t transitionCount);
    Result _init() override;
    void _deinit() override;

    Result start(uint32_t stateNo, void *userData, uint32_t initialTick);

    void event_set(uint32_t events);

    void event_reset(uint32_t events);

    void update(uint32_t tick);

    void update_inc(uint32_t tickInc);

  public:
    void *userData;
    uint32_t lastUpdateTick;
    uint32_t currentTick;

  private:
    const char *_name;
    FSM_EventGroup _events;
    FSM_State (&_states)[];
    uint8_t _stateCount;
    FSM_Transition (&_transitions)[];
    uint8_t _transitionCount;

    FSM_State *_currentState;

    FSM_State *_find_state_by_no(uint8_t stateNo);
    void _transition_check(FSM_State &state);
};

} // namespace wibot::fsm

#endif // __WWBASE_FSM_HPP__
