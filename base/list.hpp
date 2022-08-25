#ifndef __WWBASE_LIST_HPP__
#define __WWBASE_LIST_HPP__

#include "base.hpp"

namespace ww {
struct List;
struct List {
    void *data;
    List *previous;
    List *next;
    void next_append(List *node);
    void previous_append(List *node);
    void remove();
};

} // namespace ww

#endif //__WWBASE_LIST_HPP__