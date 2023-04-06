#ifndef __WWBASE_TREE_HPP__
#define __WWBASE_TREE_HPP__

#include "base.hpp"

namespace wibot
{
    struct Tree;
    typedef int (* CompareFunction)(const Tree* node, const void* b);
    struct Tree
    {
        Tree* parent;
        Tree* next;
        Tree* child;
        bool child_append(Tree* node);
        bool child_remove(Tree* node);
        Tree* child_find(const void* item, const CompareFunction compare);
    };

} // namespace wibot

#endif // __WWBASE_TREE_HPP__
