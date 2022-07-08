#include "tree.h"
#include "stdint.h"

bool tree_child_append(Tree *parent, Tree *node)
{
    Tree *pre = parent->child;
    if (pre == NULL)
    {
        parent->child = node;
        node->parent = parent;
        return true;
    }
    while (pre->next != NULL)
    {
        pre = pre->next;
    }
    if (pre->next == NULL)
    {
        pre->next = node;
        node->parent = parent;
        return true;
    }
    return false;
};

bool tree_child_remove(Tree *node)
{
    if (node->parent == NULL)
    {
        return true;
    }
    Tree *pre = node->parent->child;
    if (pre == node)
    {
        node->parent->child = NULL;
        node->parent = NULL;
        return true;
    }
    while (pre->next != NULL && pre->next != node)
    {
        pre = pre->next;
    }
    if (pre->next != NULL)
    {
        pre->next = NULL;
        node->parent = NULL;
        return true;
    }
    else
    {
        return false;
    }
};

Tree *tree_child_find(const Tree *parent, const void *item, const CompareFunction compare)
{
    Tree *child = parent->child;
    while (child != NULL)
    {
        if (compare(child, item) == 0)
        {
            return child;
        }
        child = child->next;
    };
    return NULL;
};