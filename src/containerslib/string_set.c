#include <string.h>

#include "containerslib/rbtree.h"

#include "containerslib/string_set.h"

struct StringSet {
    RbTree *rbtree;
};

StringSet *stringset_init() {
    StringSet *self = malloc(sizeof(*self));

    self->rbtree = rbtree_init(strcmp);

    return self;
}

bool stringset_put(StringSet *self, char *key) {
    char *out_key;
    rbtree_put(self->rbtree, key, self, &out_key);
    if (out_key != NULL)
        free(out_key);

    return out_key != NULL;
}

bool stringset_contains(StringSet *self, char *key) {
    return rbtree_contains(self->rbtree, key);
}

void stringset_delete(StringSet *self, char *key) {
    char *out_key;
    rbtree_delete(self->rbtree, key, &out_key);
    if (out_key != NULL)
        free(out_key);
}

// Is the table empty?
bool stringset_empty(StringSet *self) {
    return rbtree_empty(self->rbtree);
}

// Number of key-value pairs in the table.
int stringset_size(StringSet *self) {
    return rbtree_size(self->rbtree);
}

// Visit all the key-value pairs in the order of their keys.
void stringset_traverse(StringSet *self, void (*visit)(char *)) {
    rbtree_traverse(self->rbtree, visit);
}

// Clean up the table memory.
void stringset_finish(StringSet *self) {
    rbtree_finish(self->rbtree);
    free(self);
}
