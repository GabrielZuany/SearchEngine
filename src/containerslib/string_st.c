#include <string.h>

#include "containerslib/rbtree.h"
#include "containerslib/tst.h"
#include "containerslib/string_st.h"
#include "containerslib/exceptions.h"

struct StringSt {
    RbTree *tst;
};

// Create an empty symbol table.
StringSt *stringst_init() {
    StringSt *self = malloc(sizeof(*self));

    self->tst = rbtree_init((cmp_fn)strcmp);

    return self;
}

// Put key-value pair into the table: a[key] = val; .
void *stringst_put(StringSt *self, char *key, void *val) {
    void *replaced_val = rbtree_put(self->tst, key, val, &replaced_val);
    return replaced_val;
}

// Value paired with key: a[key] .
void *stringst_get(StringSt *self, char *key) {
    void *out_key = rbtree_get(self->tst, key, &out_key);
    return out_key;
}

// Is there a value paired with key?
bool stringst_contains(StringSt *self, char *key) {
    return rbtree_contains(self->tst, key);
}

// Remove key (and its value) from table.
/* void *stringst_delete(StringSt *self, char *key) { */
/*     char *out_key; */
/*     void *val = rbtree_delete(self->tst, key, (void **)&out_key); */
/*     free(out_key); */
/*     return val; */
/* } */

// Is the table empty?
bool stringst_empty(StringSt *self) {
    return rbtree_empty(self->tst);
}

// Number of key-value pairs in the table.
/* int stringst_size(StringSt *self) { */
/*     return rbtree_size(self->tst); */
/* } */

// Smallest key.
/* char *stringst_min(StringSt *self) { */
/*     return rbtree_min(self->tst); */
/* } */

// Largest key.
/* char *stringst_max(StringSt *self) { */
/*     return rbtree_max(self->tst); */
/* } */

// Largest key less than or equal to key.
/* char *stringst_floor(StringSt *self, char *key) { */
/*     return rbtree_floor(self->tst, key); */
/* } */

// Smallest key greater than to equal to key.
/* char *stringst_ceiling(StringSt *self, char *key) { */
/*     return rbtree_ceiling(self->tst, key); */
/* } */

// Delete smallest key.
/* void *stringst_delmin(StringSt *self, char **out_key) { */
/*     void *val = rbtree_delmin(self->tst, (void **)out_key); */

/*     return val; */
/* } */

// Delete largest key.
/* void *stringst_delmax(StringSt *self, char **out_key) { */
/*     void *val = rbtree_delmax(self->tst, (void **)out_key); */
/*  */
/*     return val; */
/* } */

struct StringStIterator {
    RbTreeIterator *iterator;
};

StringStIterator *stringst_iterator_init(StringSt* self) {
    StringStIterator *iterator = malloc(sizeof(*iterator));
    iterator->iterator = rbtree_iterator_init(self->tst);
    return iterator;
}

bool stringst_iterator_has_next(StringStIterator *iterator) {
    return rbtree_iterator_has_next(iterator->iterator);
}

void *stringst_iterator_next(StringStIterator *self, char **out_key) {
    return rbtree_iterator_next(self->iterator, (void *)out_key);
}

void stringst_iterator_finish(StringStIterator* self) {
    rbtree_iterator_free(self->iterator);
    free(self);
}

// Visit all the key-value pairs in the order of their keys.
void stringst_traverse(StringSt *self, void (*visit)(char *,  void *)) {
    /* rbtree_traverse(self->tst, visit); */
}

// Clean up the table memory.
void stringst_finish(StringSt *self) {
    rbtree_finish(self->tst);
    free(self);
}
