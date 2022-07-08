#ifndef ___list_H__
#define ___list_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "shared.h"
#include "stdbool.h"

    typedef struct List
    {
        struct List *previous;
        struct List *next;
    } List;

    bool list_next_append(List *previous, List *node);
    bool list_previous_append(List *next, List *node);
    void list_remove(List *node);

#ifdef __cplusplus
}
#endif

#endif // ___list_H__
