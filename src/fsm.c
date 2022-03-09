#include "../inc/fsm.h"
#include "log.h"

#define EVENT_CHECK(flag, event, mode) (((mode) == FSM_EVENT_MODE_OR) ? (((flag) & (event)) != 0) : (((flag) & (event)) == (event)))

static FSM_State_t *FSM_find_state(FSM_t *fsm, uint32_t state_no);

static void FSM_state_entry(FSM_t *fsm, FSM_State_t *state);

static void FSM_state_exit(FSM_t *fsm, FSM_State_t *state);

static void FSM_state_poll(FSM_t *fsm, FSM_State_t *state);

static void FSM_transition_check(FSM_t *fsm, FSM_State_t *state);

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

void FSM_state_register(FSM_t *fsm, FSM_State_Config_t *config)
{
    if (fsm->state_count >= FSM_MAX_STATES_COUNT)
    {
        return;
    }
    fsm->states[fsm->state_count].config = config;
    fsm->states[fsm->state_count].user_data = NULL;
    fsm->states[fsm->state_count].transition_count = 0;
    fsm->state_count++;
}

void FSM_states_register(FSM_t *fsm, FSM_State_Config_t configs[], uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        FSM_state_register(fsm, &configs[i]);
    }
}

void FSM_transition_register(FSM_t *fsm, FSM_Transition_Config_t *config)
{
    if (config->mode == FSM_TRANSITION_MODE_EVENT)
    {
        if (config->mode_parameters.event.events == FSM_NO_EVENT)
        {
            return;
        }
    }
    else if (config->mode == FSM_TRANSITION_MODE_TIMEOUT)
    {
        // if (config->mode_parameters.timeout == 0)
        // {
        //     return;
        // }
    }
    else if (config->mode == FSM_TRANSITION_MODE_CONDITION && config->mode_parameters.condition == NULL)
    {
        if (config->mode_parameters.condition == NULL)
        {
            return;
        }
    }
    FSM_State_t *fromState = FSM_find_state(fsm, config->from);
    FSM_State_t *toState = FSM_find_state(fsm, config->to);
    if (fromState == NULL || toState == NULL)
    {
        return;
    }
    fsm->transitions[fsm->transition_count].config = config;
    fsm->transitions[fsm->transition_count].to = toState;
    fromState->transitions[fromState->transition_count] = &(fsm->transitions[fsm->transition_count]);
    fromState->transition_count++;
    fsm->transition_count++;
}

void FSM_transitions_register(FSM_t *fsm, FSM_Transition_Config_t configs[], uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        FSM_transition_register(fsm, &configs[i]);
    }
}

void FSM_start(FSM_t *fsm, uint32_t state_no, void *user_data, uint32_t initial_tick)
{
    fsm->current_tick = initial_tick;
    FSM_State_t *state = FSM_find_state(fsm, state_no);
    if (state == NULL)
    {
        return;
    }
    fsm->user_data = user_data;
    FSM_state_entry(fsm, state);
    fsm->last_update_tick = initial_tick;
}

void FSM_update(FSM_t *fsm, uint32_t tick)
{
    fsm->current_tick = tick;
    FSM_State_t *state = fsm->current_state;
    if (state == NULL)
    {
        return;
    }
    FSM_state_poll(fsm, state);
    FSM_transition_check(fsm, state);
    fsm->last_update_tick = tick;
}

void FSM_update_inc(FSM_t *fsm, uint32_t tick_inc)
{
    FSM_update(fsm, fsm->last_update_tick + tick_inc);
}

static FSM_State_t *FSM_find_state(FSM_t *fsm, uint32_t state_no)
{
    for (uint32_t i = 0; i < fsm->state_count; i++)
    {
        if (fsm->states[i].config->state_no == state_no)
        {
            return &(fsm->states[i]);
        }
    }

    return NULL;
}

static void FSM_state_entry(FSM_t *fsm, FSM_State_t *state)
{
    fsm->current_state = state;
    state->enter_tick = fsm->current_tick;
    if (state->config->entry_action != NULL)
    {
        state->config->entry_action(fsm, state);
    }
    LOG_I("FSM", "State entry: %s.%s", fsm->name, state->config->name);
}

static void FSM_state_exit(FSM_t *fsm, FSM_State_t *state)
{
    if (state->config->exit_action != NULL)
    {
        state->config->exit_action(fsm, state);
    }
    state->exit_tick = fsm->current_tick;
    LOG_I("FSM", "State exit: %s.%s", fsm->name, state->config->name);
}

static void FSM_state_poll(FSM_t *fsm, FSM_State_t *state)
{
    if (state->config->poll_action != NULL)
    {
        state->config->poll_action(fsm, state);
        // LOG_I("FSM", "State poll: %s.%s", fsm->name, state->config->name);
    }
}

static void FSM_transition_check(FSM_t *fsm, FSM_State_t *state)
{
    uint32_t events = fsm->events;
    uint32_t duration = (fsm->current_tick - state->enter_tick);
    for (uint32_t i = 0; i < state->transition_count; i++)
    {
        FSM_Transition_t *transition = state->transitions[i];
        if (transition->config->mode == FSM_TRANSITION_MODE_EVENT && EVENT_CHECK(transition->config->mode_parameters.event.events, events, transition->config->mode_parameters.event.mode))
        {
            FSM_state_exit(fsm, state);
            if (transition->config->action != NULL)
            {
                transition->config->action(fsm, state);
            }
            FSM_state_entry(fsm, transition->to);
            return;
        }
        else if (transition->config->mode == FSM_TRANSITION_MODE_TIMEOUT && transition->config->mode_parameters.timeout <= duration)
        {
            FSM_state_exit(fsm, state);
            if (transition->config->action != NULL)
            {
                transition->config->action(fsm, state);
            }
            FSM_state_entry(fsm, transition->to);
            return;
        }
        else if (transition->config->mode == FSM_TRANSITION_MODE_CONDITION && transition->config->mode_parameters.condition(fsm, state))
        {
            FSM_state_exit(fsm, state);
            if (transition->config->action != NULL)
            {
                transition->config->action(fsm, state);
            }
            FSM_state_entry(fsm, transition->to);
            return;
        }
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