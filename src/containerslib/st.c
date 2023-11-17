#include "containerslib/rbtree.h"

#include "containerslib/st.h"

struct St {
    RbTree *rbtree;
};

// Create an empty symbol table.
St *st_init(cmp_fn key_cmp) {
    St *self = malloc(sizeof(*self));

    self->rbtree = rbtree_init(key_cmp);

    return self;
}

// Put key-value pair into the table: a[key] = val; .
void *st_put(St *self, void *key, void *val, void **out_key) {
    return rbtree_put(self->rbtree, key, val, out_key);
}

// Value paired with key: a[key] .
void *st_get(St *self, void *key) {
    void *_;
    return rbtree_get(self->rbtree, key, &_);
}

// Is there a value paired with key?
bool st_contains(St *self, void *key) {
    return rbtree_contains(self->rbtree, key);
}

// Remove key (and its value) from table.
void *st_delete(St *self, void *key, void **out_key) {
    return rbtree_delete(self->rbtree, key, out_key);
}

// Is the table empty?
bool st_empty(St *self) {
    return rbtree_empty(self->rbtree);
}

// Number of key-value pairs in the table.
int st_size(St *self) {
    return rbtree_size(self->rbtree);
}

// Smallest key.
void *st_min(St *self) {
    return rbtree_min(self->rbtree);
}

// Largest key.
void *st_max(St *self) {
    return rbtree_max(self->rbtree);
}

// Largest key less than or equal to key.
void *st_floor(St *self, void *key) {
    return rbtree_floor(self->rbtree, key);
}

// Smallest key greater than to equal to key.
void *st_ceiling(St *self, void *key) {
    return rbtree_ceiling(self->rbtree, key);
}

// Delete smallest key.
void st_delmin(St *self, void **out_key) {
    rbtree_delmin(self->rbtree, out_key);
}

// Delete largest key.
void st_delmax(St *self, void **out_key) {
    rbtree_delmax(self->rbtree, out_key);
}

// Visit all the key-value pairs in the order of their keys.
void st_traverse(St *self, void (*visit)(void *,  void *)) {
    rbtree_traverse(self->rbtree, visit);
}

// Clean up the table memory.
void st_finish(St *self) {
    rbtree_finish(self->rbtree);
    free(self);
}
