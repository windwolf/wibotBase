#include "tree.hpp"

namespace ww {
bool Tree::child_append(Tree *node) {
    Tree *pre = child;
    if (pre == nullptr) {
        child = node;
        node->parent = this;
        return true;
    }
    while (pre->next != nullptr) {
        pre = pre->next;
    }
    if (pre->next == nullptr) {
        pre->next = node;
        node->parent = this;
        return true;
    }
    return false;
};
bool Tree::child_remove(Tree *node) {
    if (node->parent == nullptr) {
        return true;
    }
    Tree *pre = node->parent->child;
    if (pre == node) {
        node->parent->child = nullptr;
        node->parent = nullptr;
        return true;
    }
    while (pre->next != nullptr && pre->next != node) {
        pre = pre->next;
    }
    if (pre->next != nullptr) {
        pre->next = nullptr;
        node->parent = nullptr;
        return true;
    } else {
        return false;
    }
};
Tree *Tree::child_find(const void *item, const CompareFunction compare) {
    Tree *child = this->child;
    while (child != nullptr) {
        if (compare(child, item) == 0) {
            return child;
        }
        child = child->next;
    };
    return nullptr;
};

} // namespace ww