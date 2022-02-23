#ifndef ___SHARED_H__
#define ___SHARED_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define EVENTS_CLEAR_FLAGS(eg) (tx_event_flags_set(&eg, 0, TX_AND))
#define EVENTS_SET_FLAGS(eg, flags) (tx_event_flags_set(&eg, flags, TX_OR))
#define EVENTS_RESET_FLAGS(eg, flags) (tx_event_flags_set(&eg, ~flags, TX_AND))

#define ALIGN32 __attribute__((aligned(32)))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define min(a, b) (((a) <= (b)) ? (a) : (b))
#define max(a, b) (((a) >= (b)) ? (a) : (b))

#define OP_RESULT uint32_t

#define OP_RESULT_OK 0x00000000
#define OP_RESULT_BUSY 0x00000080
#define OP_RESULT_NOT_SUPPORT 0x00000040
#define OP_RESULT_PARAMETER_ERROR 0x00000020
#define OP_RESULT_NO_MATCH 0x00000010
#define OP_RESULT_GENERAL_ERROR 0x00000001

#define OP_RESULT_USER_DEFINE_START 0x00010000

    typedef void (*EventHandler)(void *sender, void *host, void *event);

    typedef struct Event
    {
        struct Event *next;
        void *host;
        EventHandler handler;

    } Event;

    typedef struct Events
    {
        struct Event *event;
    } Events;

    void event_handler_add(Events *events, Event *event);

    // inline char *strntrim(char *str, size_t size, char delimit)
    // {
    //     size_t i = 0;
    //     while (*str != 0x00 && i < size)
    //     {
    //         if (*str == delimit)
    //         {
    //             str++;
    //             i++;
    //             continue;
    //         }
    //         else
    //         {
    //             return str;
    //         }
    //     }

    //     return NULL;
    // }

    char *strtrim(char *str, char delimit);

    uint32_t fast_log2(uint32_t _val);

#ifdef __cplusplus
}
#endif

#endif // ___SHARED_H__