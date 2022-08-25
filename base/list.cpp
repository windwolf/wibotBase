#include "list.hpp"

namespace ww {
void List::next_append(List *node) {
    this->next = node;
    node->previous = this;
};
void List::previous_append(List *node) {
    this->previous = node;
    node->next = this;
};
void List::remove() {
    if (this->previous != nullptr) {
        this->previous->next = this->next;
    }
    if (this->next != nullptr) {
        this->next->previous = this->previous;
    }
};
} // namespace ww