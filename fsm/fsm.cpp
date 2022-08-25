#include "fsm.hpp"
#define LOG_MODULE "fsm"
#include "log.h"

namespace ww::fsm
{

Result FSM_State::_init(FSM &fsm)
{
    if (_config.parent_state_no != 0)
    {
        this->_parent = fsm._find_state_by_no(_config.parent_state_no);
        if (this->_parent == nullptr)
        {
            LOG_E("%s %s's parent not exsits.",
                  (fsm._name == NULL) ? "" : fsm._name,
                  (_config.name == NULL) ? "" : _config.name);
            return Result_GeneralError;
        }
    }
    else
    {
        this->_parent = nullptr;
    }
    return Result_OK;
};

void FSM_State::poll(FSM &fsm)
{
    FSM_State *sta = this;
    while (sta != NULL)
    {
        sta->do_poll(fsm);
    }
    sta = sta->_parent;
};

void FSM_State::do_poll(FSM &fsm)
{
    if ((_config.poll_action != NULL) &&
        ((fsm._currentTick - _last_polling_tick) >= _config.polling_interval))
    {
        _config.poll_action(fsm, this);
        _last_polling_tick = fsm._currentTick;
    }
};

bool FSM_State::is_parent_of(FSM_State *state)
{
    while (state != NULL)
    {
        if (state->_config.state_no == _config.state_no)
        {
            return true;
        }
        state = state->_parent;
    }
    return false;
}

void FSM_State::enter(FSM &fsm, FSM_State *fromState)
{
    fsm._currentState = this;
    FSM_State *curSta = this;
    while (curSta != NULL && !curSta->is_parent_of(fromState))
    {
        curSta->do_enter(fsm);
        curSta = curSta->_parent;
    }

    LOG_I("%lu %s.%s: entry", fsm._currentTick,
          (fsm._name == NULL) ? "" : fsm._name,
          (_config.name == NULL) ? "" : _config.name);
};

void FSM_State::do_enter(FSM &fsm)
{
    _enter_tick = fsm._currentTick;
    if (_config.entry_action != NULL)
    {
        _config.entry_action(fsm, this);
    }
};

void FSM_State::exit(FSM &fsm, FSM_State *toState)
{
    FSM_State *curSta = this;
    while (curSta != NULL && !curSta->is_parent_of(toState))
    {
        curSta->do_exit(fsm);
        curSta = curSta->_parent;
    }

    LOG_I("%lu %s.%s: exit", fsm._currentTick,
          (fsm._name == NULL) ? "" : fsm._name,
          (_config.name == NULL) ? "" : _config.name);
};
void FSM_State::do_exit(FSM &fsm)
{
    if (_config.exit_action != NULL)
    {
        _config.exit_action(fsm, this);
    }
};

FSM_Transition_Config &FSM_Transition::config_get()
{
    return _config;
};

Result FSM_Transition::_init(FSM &fsm)
{
    FSM_State *fromState = fsm._find_state_by_no(_config.from);
    FSM_State *toState = fsm._find_state_by_no(_config.to);
    if (fromState == NULL)
    {
        LOG_E("%s transition's from %d not exsits.",
              (fsm._name == NULL) ? "" : fsm._name, _config.from);
        return Result_GeneralError;
    }
    if (toState == NULL)
    {
        LOG_E("%s transition's to %d not exsits.",
              (fsm._name == NULL) ? "" : fsm._name, _config.to);
        return Result_GeneralError;
    }

    for (uint8_t i = 0; i < fsm._stateCount; i++)
    {
        if (fsm._states[i]._config.parent_state_no == _config.to)
        {
            LOG_E("%s transition 's to state[%d] must be leaf node.",
                  (fsm._name == NULL) ? "" : fsm._name, _config.to);
            return Result_GeneralError;
        }
    }
#ifdef FSM_TRANSITION_PREFILTER
    fromState->_transitions[fromState->_transition_count] = this;
    fromState->_transition_count++;
#endif
    _to = toState;
    return Result_OK;
};
bool FSM_Transition::_do_event_check(FSM &fsm, FSM_State *fromState)
{
    // skip transit to self.
    if (_config.to == fromState->_config.state_no)
    {
        return false;
    }

    if (_config.mode == FSM_TRANSITION_MODE_EVENT &&
        fsm._events.check(_config.mode_parameters.events) &&
        (_config.guard == NULL || _config.guard(fsm, fromState)))
    {
        fromState->exit(fsm, _to);

        if (_config.action != NULL)
        {
            _config.action(fsm, fromState);
        }
        _to->enter(fsm, fsm._currentState);
        return true;
    }
    return false;
};

bool FSM_Transition::_do_timeout_check(FSM &fsm, FSM_State *fromState,
                                       uint32_t duration)
{
    // skip transit to self.
    if (_config.to == fromState->_config.state_no)
    {
        return false;
    }
    if (_config.mode == FSM_TRANSITION_MODE_TIMEOUT &&
        _config.mode_parameters.timeout <= duration &&
        (_config.guard == NULL || _config.guard(fsm, fromState)))
    {
        fromState->exit(fsm, _to);
        if (_config.action != NULL)
        {
            _config.action(fsm, fromState);
        }
        _to->enter(fsm, fsm._currentState);
        return true;
    }
    return false;
};

FSM::FSM(const char *name, uint32_t eventClearMask, FSM_State (&states)[],
         uint32_t stateCount, FSM_Transition (&transitions)[],
         uint32_t transitionCount)
    : _name(name), _events(eventClearMask), _states(states),
      _stateCount(stateCount), _transitions(transitions),
      transitionCount(transitionCount){

      };

Result FSM::init()
{
    Result rst;
#ifdef FSM_TRANSITION_PREFILTER
    for (uint8_t i = 0; i < _stateCount; i++)
    {
        _states[i]._transition_count = 0;
    }
#endif

    for (uint8_t i = 0; i < _stateCount; i++)
    {

        rst = _states[i]._init(*this);
        if (rst != Result_OK)
        {
            LOG_E("FSM %s build error.", (_name == NULL) ? "" : _name);
            return rst;
        }
    }

    for (uint32_t i = 0; i < transitionCount; i++)
    {
        rst = _transitions[i]._init(*this);
        if (rst != Result_OK)
        {
            LOG_E("FSM %s build error.", (_name == NULL) ? "" : _name);
            return rst;
        }
    }
    return Result_OK;
};
Result FSM::deinit()
{
    return Result_OK;
};

Result FSM::start(uint32_t stateNo, void *userData, uint32_t initialTick)
{
    _currentTick = initialTick;
    FSM_State *state = _find_state_by_no(stateNo);
    if (state == nullptr)
    {
        LOG_E("start state: %s.%lu not found.", (_name == NULL) ? "" : _name,
              stateNo);
        return Result_InvalidParameter;
    }
    _userData = userData;
    state->enter(*this, nullptr);
    _lastUpdateTick = initialTick;
    LOG_I("FSM %s started.", (_name == NULL) ? "" : _name);
    return Result_OK;
};

//#undef FSM_TRANSITION_PREFILTER
void FSM::_transition_check(FSM_State &state)
{
    uint32_t duration = (_currentTick - state._enter_tick);
    FSM_State *curSta = &state;
    while (curSta != NULL)
    {
#ifdef FSM_TRANSITION_PREFILTER
        for (uint8_t i = 0; i < curSta->_transition_count; i++)
        {
            FSM_Transition *transition = curSta->_transitions[i];
#else
        for (uint8_t i = 0; i < transitionCount; i++)
        {
            FSM_Transition_t *transition = &(_transitions[i]);
            if (transition->config.from != curSta->config.state_no)
            {
                continue;
            }
#endif
            if (transition->_do_event_check(*this, curSta))
            {
                return;
            }
        }
        curSta = curSta->_parent;
    }

    curSta = &state;
    while (curSta != NULL)
    {
#ifdef FSM_TRANSITION_PREFILTER
        for (uint8_t i = 0; i < curSta->_transition_count; i++)
        {
            FSM_Transition *transition = curSta->_transitions[i];
#else
        for (uint8_t i = 0; i < transitionCount; i++)
        {
            FSM_Transition *transition = &(transitions[i]);
            if (transition->config.from != curSta->config.state_no)
            {
                continue;
            }
#endif
            if (transition->_do_timeout_check(*this, curSta, duration))
            {
                return;
            }
        }
        curSta = curSta->_parent;
    }
};

void FSM::event_set(uint32_t events)
{
    this->_events.set(events);
};

void FSM::event_reset(uint32_t events)
{
    this->_events.reset(events);
};

void FSM::update(uint32_t tick)
{
    _currentTick = tick;
    FSM_State *state = _currentState;
    if (state == nullptr)
    {
        return;
    }
    _events.update_begin();
    state->poll(*this);
    _transition_check(*state);
    _events.update_end();

    _lastUpdateTick = tick;
};

void FSM::update_inc(uint32_t tickInc)
{
    update(_lastUpdateTick + tickInc);
};
} // namespace ww::fsm