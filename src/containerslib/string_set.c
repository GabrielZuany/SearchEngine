#include <string.h>
#include <stdio.h>

#include "containerslib/string_set.h"
#include "containerslib/tst.h"
#include "containerslib/exceptions.h"
#include "containerslib/rbtree.h"
#include "containerslib/types.h"

struct StringSet {
    RbTree *tst;
};

StringSet *stringset_init() {
    StringSet *self = malloc(sizeof(*self));

    self->tst = rbtree_init((cmp_fn)strcmp);

    return self;
}

bool stringset_put(StringSet *self, const char *key) {
    void *replaced_val = rbtree_put(self->tst, (void *)key, self, &replaced_val);
    return replaced_val != NULL;
}

bool stringset_contains(StringSet *self, char *key) {
    return rbtree_contains(self->tst, key);
}

void stringset_delete(StringSet *self, char *key) {
    if (self) {}; // warning suppression hihiih
    if (key) {}; // warning suppression hihiih
    exception_throw_failure("stringset_delete - Not implemented");
}

// Is the table empty?
bool stringset_empty(StringSet *self) {
    return rbtree_empty(self->tst);
}

// Number of key-value pairs in the table.
int stringset_size(StringSet *self) {
    return rbtree_size(self->tst);
}

struct StringSetIterator {
    RbTreeIterator *iterator;
};

StringSetIterator* stringset_iterator_init(StringSet* self) {
    StringSetIterator *iterator = malloc(sizeof(*iterator));
    iterator->iterator = rbtree_iterator_init(self->tst);
    return iterator;
}

bool stringset_iterator_has_next(StringSetIterator *self) {
    return rbtree_iterator_has_next(self->iterator);
}

char *stringset_iterator_next(StringSetIterator *self) {
    char *out_key;
    rbtree_iterator_next(self->iterator, (void *)&out_key);
    return out_key;
}

void stringset_iterator_finish(StringSetIterator* self) {
    rbtree_iterator_free(self->iterator);
    free(self);
}

// Visit all the key-value pairs in the order of their keys.
void stringset_traverse(StringSet *self, void (*visit)(char *)) {
    if (self) {}; // warning suppression hihiih
    if (visit) {}; // warning suppression hihiih
    exception_throw_failure("stringset_traverse - Not implemented");
}

// Clean up the table memory.
void stringset_finish(StringSet *self) {
    rbtree_finish(self->tst);
    free(self);
}
