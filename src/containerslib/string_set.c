#include <string.h>

#include "include/containerslib/string_set.h"
#include "include/containerslib/tst.h"

struct StringSet {
    TST *tst;
};

StringSet *stringset_init() {
    StringSet *self = malloc(sizeof(*self));

    self->tst = TST_init();

    return self;
}

bool stringset_put(StringSet *self, char *key, void *val) {
    return TST_insert(self->tst, key, val).val == NULL;
}

void* stringset_get(StringSet *self, char *key) {
    // return the val if the key is in the table, NULL otherwise
    return TST_search(self->tst, key);
}  

bool stringset_contains(StringSet *self, char *key) {
    return TST_search(self->tst, key) != NULL;
}

void stringset_delete(StringSet *self, char *key) {
    // TODO: não tem delete na TST ainda
}

// Is the table empty?
bool stringset_empty(StringSet *self) {
    return TST_empty(self->tst);
}

// Number of key-value pairs in the table.
int stringset_size(StringSet *self) {
    return TST_size(self->tst);
}

// Visit all the key-value pairs in the order of their keys.
void stringset_traverse(StringSet *self, void (*visit)(char *)) {
    TST_traverse(self->tst, visit);
}

// Clean up the table memory.
void stringset_finish(StringSet *self) {
    TST_free(self->tst);
    free(self);
}
