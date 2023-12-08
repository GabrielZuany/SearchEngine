#include <string.h>
#include <stdio.h>

#include "containerslib/string_set.h"
#include "containerslib/tst.h"
#include "containerslib/exceptions.h"

struct StringSet {
    TST *tst;
};

StringSet *stringset_init() {
    StringSet *self = malloc(sizeof(*self));

    self->tst = TST_init();

    return self;
}

bool stringset_put(StringSet *self, const char *key) {
    return TST_put(self->tst, key, self) != NULL;
}

bool stringset_contains(StringSet *self, char *key) {
    return TST_search(self->tst, key) != NULL;
}

void stringset_delete(StringSet *self, char *key) {
    if (self) {}; // warning suppression hihiih
    if (key) {}; // warning suppression hihiih
    exception_throw_failure("stringset_delete - Not implemented");
}

// Is the table empty?
bool stringset_empty(StringSet *self) {
    return TST_empty(self->tst);
}

// Number of key-value pairs in the table.
int stringset_size(StringSet *self) {
    return TST_size(self->tst);
}

struct StringSetIterator {
    TST_iterator *iterator;
};

StringSetIterator* stringset_iterator_init(StringSet* self) {
    StringSetIterator *iterator = malloc(sizeof(*iterator));
    iterator->iterator = TST_iterator_init(self->tst);
    return iterator;
}

bool stringset_iterator_has_next(StringSetIterator *self) {
    return TST_iterator_has_next(self->iterator);
}

char *stringset_iterator_next(StringSetIterator *self) {
    char *out_key;
    TST_iterator_next(self->iterator, &out_key);
    return out_key;
}

void stringset_iterator_finish(StringSetIterator* self) {
    TST_iterator_free(self->iterator);
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
    TST_free(self->tst);
    free(self);
}
