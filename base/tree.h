#ifndef ___tree_H__
#define ___tree_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "shared.h"
#include "stdbool.h"

    typedef struct Tree
    {
        struct Tree *parent;
        struct Tree *next;
        struct Tree *child;
    } Tree;
    typedef int (*CompareFunction)(const Tree *node, const void *b);
    bool tree_child_append(Tree *parent, Tree *node);
    bool tree_child_remove(Tree *node);
    Tree *tree_child_find(const Tree *parent, const void *item, const CompareFunction compare);

#ifdef __cplusplus
}
#endif

#endif // ___tree_H__
