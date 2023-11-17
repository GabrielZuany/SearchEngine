#include <string.h>

#include "containerslib/rbtree.h"

#include "containerslib/string_st.h"

struct StringSt {
    RbTree *rbtree;
};

// Create an empty symbol table.
StringSt *stringst_init() {
    StringSt *self = malloc(sizeof(*self));

    self->rbtree = rbtree_init(strcmp);

    return self;
}

// Put key-value pair into the table: a[key] = val; .
void *stringst_put(StringSt *self, char *key, void *val) {
    char *out_key;
    void *val = rbtree_put(self->rbtree, key, val, &out_key);
    if (out_key != NULL)
        free(out_key);

    return val;
}

// Value paired with key: a[key] .
void *stringst_get(StringSt *self, char *key) {
    return rbtree_get(self->rbtree, key);
}

// Is there a value paired with key?
bool stringst_contains(StringSt *self, char *key) {
    return rbtree_contains(self->rbtree, key);
}

// Remove key (and its value) from table.
void *stringst_delete(StringSt *self, char *key) {
    char *out_key;
    void *val = rbtree_delete(self->rbtree, key, &out_key);
    free(out_key);
    return val;
}

// Is the table empty?
bool stringst_empty(StringSt *self) {
    return rbtree_empty(self->rbtree);
}

// Number of key-value pairs in the table.
int stringst_size(StringSt *self) {
    return rbtree_size(self->rbtree);
}

// Smallest key.
char *stringst_min(StringSt *self) {
    return rbtree_min(self->rbtree);
}

// Largest key.
char *stringst_max(StringSt *self) {
    return rbtree_max(self->rbtree);
}

// Largest key less than or equal to key.
char *stringst_floor(StringSt *self, char *key) {
    return rbtree_floor(self->rbtree, key);
}

// Smallest key greater than to equal to key.
char *stringst_ceiling(StringSt *self, char *key) {
    return rbtree_ceiling(self->rbtree, key);
}

// Delete smallest key.
void *stringst_delmin(StringSt *self) {
    char *key;
    void *val = rbtree_delmin(self->rbtree, &key);
    free(key);

    return val;
}

// Delete largest key.
void *stringst_delmax(StringSt *self) {
    char *key;
    void *val = rbtree_delmax(self->rbtree, &key);
    free(key);

    return val;
}

// Visit all the key-value pairs in the order of their keys.
void stringst_traverse(StringSt *self, void (*visit)(char *,  void *)) {
    rbtree_traverse(self->rbtree, visit);
}

// Clean up the table memory.
void stringst_finish(StringSt *self) {
    rbtree_finish(self->rbtree);
    free(self);
}
