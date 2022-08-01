#ifndef ___FSM_FSM2_H__
#define ___FSM_FSM2_H__

#include <stddef.h>
#include <stdint.h>
#include "stdbool.h"
#include "eventgroup.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FSM_MAX_STATES_COUNT (256)
#define FSM_MAX_TRANSITIONS_COUNT (256)
#define FSM_MAX_TRANSITIONS_COUNT_PRE_STATE (8)

    struct FSM_State_Config_t;
    struct FSM_State_t;
    struct FSM_Transition_Config_t;
    struct FSM_Transition_t;
    struct FSM_t;

#define FSM_STATE_MODE uint8_t
#define FSM_STATE_MODE_INTERVAL 1U
#define FSM_STATE_MODE_POLL 2U

    typedef void (*FSM_Action)(struct FSM_t *, struct FSM_State_t *);
    typedef bool (*FSM_Guard)(struct FSM_t *, struct FSM_State_t *);

    typedef struct FSM_State_Config_t
    {
        const char *name;
        FSM_Action entry_action;
        FSM_Action exit_action;
        uint32_t polling_interval;
        FSM_Action poll_action;
        /**
         * @brief parent state is used to simplify transition configuration.
         * parent represents the template of the state,
         * all the children states has the transitions and actions of the parent state.
         */
        uint8_t parent_state_no;

        uint8_t state_no; // 0 means not registered
    } FSM_State_Config_t;

    typedef struct FSM_State_t
    {
        struct FSM_State_Config_t config;
        uint32_t enter_tick;
        uint32_t last_polling_tick;
        struct FSM_State_t *parent;
#ifdef FSM_TRANSITION_PREFILTER
        struct FSM_Transition_t *transitions[FSM_MAX_TRANSITIONS_COUNT_PRE_STATE];
        uint8_t transition_count;
#endif
    } FSM_State_t;

#define FSM_TRANSITION_MODE uint8_t
#define FSM_TRANSITION_MODE_TIMEOUT 1U
#define FSM_TRANSITION_MODE_EVENT 2U

    typedef struct FSM_Transition_Config_t
    {
        const char *name;
        FSM_TRANSITION_MODE mode;
        union
        {
            EventFlag_t events;
            uint32_t timeout;
        } mode_parameters;
        uint8_t from;
        uint8_t to;
        FSM_Guard guard;
        FSM_Action action;
    } FSM_Transition_Config_t;

    typedef struct FSM_Transition_t
    {
        struct FSM_Transition_Config_t config;
        struct FSM_State_t *to;
    } FSM_Transition_t;

    typedef struct FSM_t
    {
        struct FSM_State_t *states;
        uint8_t state_count;
        struct FSM_State_t *current_state;
        const char *name;

        EventGroup_t events;

        struct FSM_Transition_t *transitions;
        uint8_t transition_count;
        // uint32_t current_state_enter_tick;
        uint32_t last_update_tick;
        uint32_t current_tick;
        void *user_data;

    } FSM_t;

    /**
     * @brief Zero initializatoin
     *
     * @param fsm
     * @param name
     */
    void FSM_init(FSM_t *fsm, const char *name, uint32_t eventClearMask);

    void FSM_states_register(FSM_t *fsm, FSM_State_t *states, uint32_t count);

    void FSM_transitions_register(FSM_t *fsm, FSM_Transition_t *transitions, uint32_t count);

    bool FSM_start(FSM_t *fsm, uint32_t state_no, void *user_data, uint32_t initial_tick);

    void FSM_event_set(FSM_t *fsm, uint32_t events);

    void FSM_event_reset(FSM_t *fsm, uint32_t events);

    void FSM_update(FSM_t *fsm, uint32_t tick);

    void FSM_update_inc(FSM_t *fsm, uint32_t tick_inc);

#ifdef __cplusplus
}
#endif

#endif // ___FSM_FSM2_H__
