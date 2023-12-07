#include "containerslib/rbtree.h"

#include "containerslib/set.h"

struct Set
{
    RbTree *rbtree;
};

// Create an empty symbol table.
Set *set_init(cmp_fn key_cmp) {
    Set *self = malloc(sizeof(*self));

    self->rbtree = rbtree_init(key_cmp);

    return self;
}

// Put key-value pair into the table: a[key] = val; .
void *set_put(Set *self, void *key) {
    void *out_key;
    rbtree_put(self->rbtree, key, self, &out_key);
    return out_key;
}

// Is there a value paired with key?
bool set_contains(Set *self, void *key) {
    return rbtree_contains(self->rbtree, key);
}

// Is the table empty?
bool set_empty(Set *self) {
    return rbtree_empty(self->rbtree);
}

// Number of key-value pairs in the table.
int set_size(Set *self) {
    return rbtree_size(self->rbtree);
}

struct SetIterator {
    RbTreeIterator *iterator;
};

SetIterator* set_iterator_init(Set *self) {
    SetIterator *iterator = malloc(sizeof(*iterator));
    iterator->iterator = rbtree_iterator_init(self->rbtree);
    return iterator;
}

bool set_iterator_has_next(SetIterator *self) {
    return rbtree_iterator_has_next(self->iterator);
}

void *set_iterator_next(SetIterator *self) {
    void *out_key;
    rbtree_iterator_next(self->iterator, &out_key);
    return out_key;
}

void set_iterator_finish(SetIterator* self) {
    rbtree_iterator_free(self->iterator);
    free(self);
}

// Clean up the table memory.
void set_finish(Set *self) {
    rbtree_finish(self->rbtree);
    free(self);
}
