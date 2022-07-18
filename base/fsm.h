#ifndef ___FSM_H__
#define ___FSM_H__

#include <stddef.h>
#include <stdint.h>
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FSM_MAX_STATES_COUNT (8)
#define FSM_MAX_TRANSITIONS_COUNT (8)
#define FSM_MAX_TRANSITIONS_COUNT_PRE_STATE (4)

#define FSM_NO_EVENT (0x00000000)
#define FSM_ALL_EVENT (0xFFFFFFFF)

    struct FSM_State_Config_t;
    struct FSM_State_t;
    struct FSM_Transition_Config_t;
    struct FSM_Transition_t;
    struct FSM_t;

    typedef enum
    {
        FSM_STATE_MODE_INTERVAL,
        FSM_STATE_MODE_POLL
    } FSM_State_Mode;

#define FSM_EVENT_MODE uint8_t
#define FSM_EVENT_SELECT_MASK 0x01
#define FSM_EVENT_SELECT_OR 0x00
#define FSM_EVENT_SELECT_AND 0x01
#define FSM_EVENT_ACTION_MASK 0x02
#define FSM_EVENT_ACTION_KEEP 0x00
#define FSM_EVENT_ACTION_CLEAR 0x02
#define FSM_EVENT_PRESENTATION_MASK 0x04
#define FSM_EVENT_PRESENTATION_SET 0x00
#define FSM_EVENT_PRESENTATION_RESET 0x04

    typedef void (*FSM_Action)(struct FSM_t *, struct FSM_State_t *);
    typedef bool (*FSM_Guard)(struct FSM_t *, struct FSM_State_t *);

    typedef struct FSM_State_Config_t
    {
        uint32_t state_no; // 0 means not registered
        const char *name;
        // FSM_State_Mode mode;
        //  union
        //  {
        //      uint32_t interval;
        //  } mode_parameters;
        FSM_Action entry_action;
        FSM_Action exit_action;
        uint32_t polling_interval;
        FSM_Action poll_action;
        uint32_t parent_state_no;
    } FSM_State_Config_t;

    typedef struct FSM_State_t
    {
        struct FSM_State_Config_t *config;
        void *user_data;
        struct FSM_Transition_t *transitions[FSM_MAX_TRANSITIONS_COUNT_PRE_STATE];
        uint32_t transition_count;

        uint32_t enter_tick;
        // uint32_t exit_tick;
        uint32_t last_polling_tick;
        struct FSM_State_t *parent;
    } FSM_State_t;

    typedef enum
    {
        FSM_TRANSITION_MODE_TIMEOUT,
        FSM_TRANSITION_MODE_EVENT,
        // FSM_TRANSITION_MODE_CONDITION,
        // FSM_TRANSITION_MODE_EVENT_OR_TIMEOUT,
    } FSM_Transition_Mode;

    typedef struct FSM_Transition_Config_t
    {
        const char *name;
        FSM_Transition_Mode mode;
        union
        {
            struct
            {
                uint32_t events;
                FSM_EVENT_MODE mode;
            } event;
            uint32_t timeout;

        } mode_parameters;
        uint32_t from;
        uint32_t to;
        FSM_Guard guard;
        FSM_Action action;
    } FSM_Transition_Config_t;

    typedef struct FSM_Transition_t
    {
        struct FSM_Transition_Config_t *config;
        struct FSM_State_t *to;
    } FSM_Transition_t;

    typedef struct FSM_t
    {
        struct FSM_State_t states[FSM_MAX_STATES_COUNT];
        uint32_t state_count;
        struct FSM_State_t *current_state;
        const char *name;

        uint32_t events;

        struct FSM_Transition_t transitions[FSM_MAX_TRANSITIONS_COUNT];
        uint32_t transition_count;
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
    void FSM_init(FSM_t *fsm, const char *name);

    void FSM_state_register(FSM_t *fsm, FSM_State_Config_t *config);

    void FSM_states_register(FSM_t *fsm, FSM_State_Config_t configs[], uint32_t count);

    void FSM_transition_register(FSM_t *fsm, FSM_Transition_Config_t *config);

    void FSM_transitions_register(FSM_t *fsm, FSM_Transition_Config_t configs[], uint32_t count);

    bool FSM_start(FSM_t *fsm, uint32_t state_no, void *user_data, uint32_t initial_tick);

    void FSM_event_set(FSM_t *fsm, uint32_t events);

    void FSM_event_reset(FSM_t *fsm, uint32_t events);

    void FSM_update(FSM_t *fsm, uint32_t tick);

    void FSM_update_inc(FSM_t *fsm, uint32_t tick_inc);

#ifdef __cplusplus
}
#endif

#endif // ___FSM_H__
