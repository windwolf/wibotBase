#ifndef ___FSM_H__
#define ___FSM_H__

#include <stddef.h>
#include <stdint.h>

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

    typedef enum
    {
        FSM_EVENT_MODE_OR,
        FSM_EVENT_MODE_AND
    } FSM_Event_Mode;

    typedef struct FSM_State_Config_t
    {
        uint32_t state_no;
        char *name;
        // FSM_State_Mode mode;
        //  union
        //  {
        //      uint32_t interval;
        //  } mode_parameters;
        void (*entry_action)(struct FSM_t *, struct FSM_State_t *);
        void (*poll_action)(struct FSM_t *, struct FSM_State_t *);
        void (*exit_action)(struct FSM_t *, struct FSM_State_t *);
    } FSM_State_Config_t;

    typedef struct FSM_State_t
    {
        struct FSM_State_Config_t *config;
        void *user_data;
        struct FSM_Transition_t *transitions[FSM_MAX_TRANSITIONS_COUNT_PRE_STATE];
        uint32_t transition_count;
    } FSM_State_t;

    typedef enum
    {
        FSM_TRANSITION_MODE_TIMEOUT,
        FSM_TRANSITION_MODE_EVENT,
        FSM_TRANSITION_MODE_CONDITION,
        // FSM_TRANSITION_MODE_EVENT_OR_TIMEOUT,
    } FSM_Transition_Mode;

    typedef struct FSM_Transition_Config_t
    {
        char *name;
        FSM_Transition_Mode mode;
        union
        {
            struct
            {
                uint32_t events;
                FSM_Event_Mode mode;
            } event;
            uint32_t timeout;
            uint32_t (*condition)(struct FSM_t *, struct FSM_State_t *);
        } mode_parameters;
        uint32_t from;
        uint32_t to;
        uint32_t (*action)(struct FSM_t *, struct FSM_State_t *);
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

        uint32_t events;

        struct FSM_Transition_t transitions[FSM_MAX_TRANSITIONS_COUNT];
        uint32_t transition_count;
        uint32_t current_state_enter_tick;
        uint32_t last_update_tick;
        uint32_t current_tick;
        void *user_data;
    } FSM_t;

    void FSM_init(FSM_t *fsm);

    void FSM_state_register(FSM_t *fsm, FSM_State_Config_t *config);

    void FSM_states_register(FSM_t *fsm, FSM_State_Config_t configs[], uint32_t count);

    void FSM_transition_register(FSM_t *fsm, FSM_Transition_Config_t *config);

    void FSM_transitions_register(FSM_t *fsm, FSM_Transition_Config_t configs[], uint32_t count);

    void FSM_start(FSM_t *fsm, uint32_t state_no, void *user_data, uint32_t initial_tick);

    void FSM_event_set(FSM_t *fsm, uint32_t events);

    void FSM_event_reset(FSM_t *fsm, uint32_t events);

    void FSM_update(FSM_t *fsm, uint32_t tick);

    void FSM_update_inc(FSM_t *fsm, uint32_t tick_inc);

#ifdef __cplusplus
}
#endif

#endif // ___FSM_H__