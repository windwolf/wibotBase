#include "fsm.h"

#include "stdbool.h"
#define LOG_MODULE "fsm"
#include "log.h"

#define EVENT_CHECK(flag, event, mode)                                                                                                                                                              \
    ((((mode)&FSM_EVENT_PRESENTATION_MASK) == FSM_EVENT_PRESENTATION_SET) ? ((((mode)&FSM_EVENT_SELECT_MASK) == FSM_EVENT_SELECT_OR) ? (((flag) & (event)) != 0) : (((flag) & (event)) == (event))) \
                                                                          : ((((mode)&FSM_EVENT_SELECT_MASK) == FSM_EVENT_SELECT_AND) ? (((flag) & (event)) == 0) : (((flag) & (event)) != (event))))

static bool FSM_build(FSM_t *fsm);

static FSM_State_t *FSM_find_state(FSM_t *fsm, uint32_t state_no);

static void FSM_state_do_entry(FSM_t *fsm, FSM_State_t *state, FSM_State_t *from_state);

static void FSM_state_do_exit(FSM_t *fsm, FSM_State_t *state, FSM_State_t *to_state);

static void FSM_state_do_poll(FSM_t *fsm, FSM_State_t *state);

static void FSM_transition_check(FSM_t *fsm, FSM_State_t *state);

static bool FSM_has_child(FSM_t *fsm, FSM_State_t *state);

static bool FSM_parent_of(FSM_State_t *targetState, FSM_State_t *patternState);

void FSM_init(FSM_t *fsm, const char *name)
{
    fsm->name = name;
    fsm->state_count = 0;
    fsm->current_state = NULL;
    fsm->last_update_tick = 0;
    fsm->current_tick = 0;
    fsm->transition_count = 0;
    fsm->user_data = NULL;
}

void FSM_states_register(FSM_t *fsm, FSM_State_t *states, uint32_t count)
{
#ifdef FSM_TRANSITION_PREFILTER
    for (uint8_t i = 0; i < count; i++)
    {
        states[i].transition_count = 0;
    }
#endif
    fsm->states = states;
    fsm->state_count = count;
}

void FSM_transitions_register(FSM_t *fsm, FSM_Transition_t *transitions, uint32_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        FSM_Transition_t *tran = &transitions[i];
        if (tran->config.mode == FSM_TRANSITION_MODE_EVENT)
        {
            if (tran->config.mode_parameters.event.events == FSM_NO_EVENT)
            {
                LOG_E("transition[%u] event is FSM_NO_EVENT", i);
                return;
            }
        }
    }

    fsm->transitions = transitions;
    fsm->transition_count = count;
}

static bool FSM_build(FSM_t *fsm)
{
    for (uint8_t i = 0; i < fsm->state_count; i++)
    {
        FSM_State_t *sta = &(fsm->states[i]);
        if (sta->config.parent_state_no != 0)
        {
            FSM_State_t *pState = FSM_find_state(fsm, sta->config.parent_state_no);
            if (pState == NULL)
            {
                LOG_E("%s %s's parent not exsits.", (fsm->name == NULL) ? "" : fsm->name, (sta->config.name == NULL) ? "" : sta->config.name);
                return false;
            }
            sta->parent = pState;
        }
        else
        {
            sta->parent = NULL;
        }
    }

    for (uint32_t i = 0; i < fsm->transition_count; i++)
    {
        FSM_Transition_t *trans = &(fsm->transitions[i]);
        FSM_State_t *fromState = FSM_find_state(fsm, trans->config.from);
        FSM_State_t *toState = FSM_find_state(fsm, trans->config.to);
        if (fromState == NULL)
        {
            LOG_E("%s transition[%d]'s from not exsits.", (fsm->name == NULL) ? "" : fsm->name, (int)i);
            return false;
        }
        if (toState == NULL)
        {
            LOG_E("%s transition[%d]'s to not exsits.", (fsm->name == NULL) ? "" : fsm->name, (int)i);
            return false;
        }

        if (FSM_has_child(fsm, toState))
        {
            LOG_E("%s transition[%d] 's to state must be leaf node.", (fsm->name == NULL) ? "" : fsm->name, (int)i);
            return false;
        }
#ifdef FSM_TRANSITION_PREFILTER
        fromState->transitions[fromState->transition_count] = trans;
        fromState->transition_count++;
#endif
        trans->to = toState;
    }
    return true;
}

bool FSM_start(FSM_t *fsm, uint32_t state_no, void *user_data, uint32_t initial_tick)
{
    if (!FSM_build(fsm))
    {
        LOG_E("FSM %s build error.", (fsm->name == NULL) ? "" : fsm->name);
        return false;
    }
    fsm->current_tick = initial_tick;
    FSM_State_t *state = FSM_find_state(fsm, state_no);
    if (state == NULL)
    {
        LOG_E("start state: %s.%lu not found.", (fsm->name == NULL) ? "" : fsm->name, state_no);
        return false;
    }
    fsm->user_data = user_data;
    FSM_state_do_entry(fsm, state, NULL);
    fsm->last_update_tick = initial_tick;
    LOG_I("FSM %s started.", (fsm->name == NULL) ? "" : fsm->name);
    return true;
}

void FSM_update(FSM_t *fsm, uint32_t tick)
{
    fsm->current_tick = tick;
    FSM_State_t *state = fsm->current_state;
    if (state == NULL)
    {
        return;
    }
    FSM_state_do_poll(fsm, state);
    FSM_transition_check(fsm, state);
    fsm->last_update_tick = tick;
}

void FSM_update_inc(FSM_t *fsm, uint32_t tick_inc)
{
    FSM_update(fsm, fsm->last_update_tick + tick_inc);
}

static FSM_State_t *FSM_find_state(FSM_t *fsm, uint32_t state_no)
{
    for (uint8_t i = 0; i < fsm->state_count; i++)
    {
        if (fsm->states[i].config.state_no == state_no)
        {
            return &(fsm->states[i]);
        }
    }

    return NULL;
}

static bool FSM_has_child(FSM_t *fsm, FSM_State_t *state)
{
    for (uint8_t i = 0; i < fsm->state_count; i++)
    {
        FSM_State_t *sta = &(fsm->states[i]);
        if (sta->config.parent_state_no == state->config.state_no)
        {
            return true;
        }
    }
    return false;
}

static bool FSM_parent_of(FSM_State_t *targetState, FSM_State_t *patternState)
{
    FSM_State_t *sta = targetState->parent;
    while (sta != NULL)
    {
        if (sta->config.state_no == patternState->config.state_no)
        {
            return true;
        }
        sta = sta->parent;
    }
    return false;
}

// static FSM_State_t *FSM_find_state_by_no(FSM_t *fsm, uint32_t no)
// {
//     for (uint32_t i = 0; i < fsm->state_count; i++)
//     {
//         FSM_State_t *sta = &(fsm->states[i]);
//         if (sta->config->state_no == no)
//         {
//             return sta;
//         }
//     }
//     return NULL;
// }

static void FSM_state_do_entry(FSM_t *fsm, FSM_State_t *state, FSM_State_t *from_state)
{
    fsm->current_state = state;
    FSM_State_t *curSta = state;
    while (curSta != NULL && !FSM_parent_of(from_state, curSta))
    {
        curSta->enter_tick = fsm->current_tick;
        if (curSta->config.entry_action != NULL)
        {
            curSta->config.entry_action(fsm, curSta);
        }
        curSta = curSta->parent;
    }

    LOG_I("%lu %s.%s: entry", fsm->current_tick, (fsm->name == NULL) ? "" : fsm->name, (state->config.name == NULL) ? "" : state->config.name);
}

static void FSM_state_do_exit(FSM_t *fsm, FSM_State_t *state, FSM_State_t *to_state)
{
    FSM_State_t *curSta = state;
    while (curSta != NULL && !FSM_parent_of(to_state, curSta))
    {
        if (curSta->config.exit_action != NULL)
        {
            curSta->config.exit_action(fsm, curSta);
        }
        curSta = curSta->parent;
    }

    LOG_I("%lu %s.%s: exit", fsm->current_tick, (fsm->name == NULL) ? "" : fsm->name, (state->config.name == NULL) ? "" : state->config.name);
}

static void FSM_state_do_poll(FSM_t *fsm, FSM_State_t *state)
{
    FSM_State_t *sta = state;
    while (sta != NULL)
    {
        if ((sta->config.poll_action != NULL) &&
            ((fsm->current_tick - state->last_polling_tick) >= state->config.polling_interval))
        {
            sta->config.poll_action(fsm, state);
            state->last_polling_tick = fsm->current_tick;
            // LOG_I("FSM", "State poll: %s.%s", (fsm->name == NULL) ? "" : fsm->name, (state->config->name == NULL) ? NULL : state->config->name);
        }
        sta = sta->parent;
    }
}

//
/**
 * @brief Check current leaf state's transitions condition, if condition is true, then trigger transition.
 * If all the transition's condition is false, and state has parent state, then check parent state's transitions condition.
 * If all the transition's condition is false, and state has no parent state, do nothing.
 * When transition is triggered, the state will exit and enter the target state. if state or target state has exit/entry action, invoke it.
 * If state or target state has parent state, invoke parent state's exit/entry action.
 *
 * @param fsm
 * @param state
 */
static void FSM_transition_check(FSM_t *fsm, FSM_State_t *state)
{
    uint32_t events = fsm->events;
    uint32_t duration = (fsm->current_tick - state->enter_tick);
    FSM_State_t *curSta = state;
    while (curSta != NULL)
    {
#ifdef FSM_TRANSITION_PREFILTER
        for (uint8_t i = 0; i < curSta->transition_count; i++)
        {
            FSM_Transition_t *transition = curSta->transitions[i];
#else
        for (uint8_t i = 0; i < fsm->transition_count; i++)
        {
            FSM_Transition_t *transition = &(fsm->transitions[i]);
            if (transition->config.from != curSta->config.state_no)
            {
                continue;
            }
#endif
            // skip transit to self.
            if (transition->config.to == state->config.state_no)
            {
                continue;
            }

            if (transition->config.mode == FSM_TRANSITION_MODE_EVENT &&
                EVENT_CHECK(events, transition->config.mode_parameters.event.events, transition->config.mode_parameters.event.mode) &&
                (transition->config.guard == NULL || transition->config.guard(fsm, state)))
            {
                FSM_state_do_exit(fsm, state, transition->to);
                if ((transition->config.mode_parameters.event.mode & FSM_EVENT_ACTION_MASK) == FSM_EVENT_ACTION_CLEAR)
                {
                    FSM_event_reset(fsm, transition->config.mode_parameters.event.events);
                }
                if (transition->config.action != NULL)
                {
                    transition->config.action(fsm, state);
                }
                FSM_state_do_entry(fsm, transition->to, fsm->current_state);
                return;
            }
            else if (transition->config.mode == FSM_TRANSITION_MODE_TIMEOUT &&
                     transition->config.mode_parameters.timeout <= duration &&
                     (transition->config.guard == NULL || transition->config.guard(fsm, state)))
            {
                FSM_state_do_exit(fsm, state, transition->to);
                if (transition->config.action != NULL)
                {
                    transition->config.action(fsm, state);
                }
                FSM_state_do_entry(fsm, transition->to, fsm->current_state);
                return;
            }
        }
        curSta = curSta->parent;
    }
}

void FSM_event_set(FSM_t *fsm, uint32_t events)
{
    fsm->events |= events;
};

void FSM_event_reset(FSM_t *fsm, uint32_t events)
{
    fsm->events &= ~events;
};
