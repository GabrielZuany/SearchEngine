#include <string.h>

#include "containerslib/tst.h"

#include "containerslib/string_st.h"

struct StringSt {
    TST *tst;
};

// Create an empty symbol table.
StringSt *stringst_init() {
    StringSt *self = malloc(sizeof(*self));

    self->tst = TST_init((cmp_fn)strcmp);

    return self;
}

// Put key-value pair into the table: a[key] = val; .
void *stringst_put(StringSt *self, char *key, void *val) {
    char *out_key;
    void *replaced_val = TST_insert(self->tst, key, val);
    if (out_key != NULL)
        free(out_key);

    return replaced_val;
}

// Value paired with key: a[key] .
void *stringst_get(StringSt *self, char *key) {
    return TST_search(self->tst, key);
}

// Is there a value paired with key?
bool stringst_contains(StringSt *self, char *key) {
    return TST_search(self->tst, key) != NULL;
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
    return TST_empty(self->tst);
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

// Visit all the key-value pairs in the order of their keys.
void stringst_traverse(StringSt *self, void (*visit)(char *,  void *)) {
    TST_traverse(self->tst, (void (*)(void *,  void *))visit);
}

// Clean up the table memory.
void stringst_finish(StringSt *self) {
    TST_free(self->tst);
    free(self);
}
