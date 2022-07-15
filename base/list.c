#include "list.h"
#include "stdint.h"

bool list_next_append(List *previous, List *node)
{
    // assert

    previous->next = node;
    node->previous = previous;
    return true;
};

bool list_previous_append(List *next, List *node)
{
    next->previous = node;
    node->next = next;
    return true;
};

void list_remove(List *node)
{
    if (node->previous != NULL)
    {
        node->previous->next = node->next;
    }
    if (node->next != NULL)
    {
        node->next->previous = node->previous;
    }
};