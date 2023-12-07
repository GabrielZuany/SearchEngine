#ifndef _CONTAINERSLIB_SET_H_
#define _CONTAINERSLIB_SET_H_

#include "containerslib/types.h"

typedef struct Set Set;

// Create an empty symbol table.
Set *set_init(cmp_fn key_cmp);

// Put key-value pair into the table: a[key] = val; .
void *set_put(Set *self, void *key);
// Is there a value paired with key?
bool set_contains(Set *self, void *key);
// Is the table empty?
bool set_empty(Set *self);
// Number of key-value pairs in the table.
int set_size(Set *self);

typedef struct SetIterator SetIterator;

SetIterator *set_iterator_init(Set* self);
bool set_iterator_has_next(SetIterator *iterator);
void *set_iterator_next(SetIterator *self);
void set_iterator_finish(SetIterator* self);

// Clean up the table memory.
void set_finish(Set *self);

#endif
