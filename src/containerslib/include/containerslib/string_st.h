#ifndef _CONTAINERSLIB_STRING_stringST_H_
#define _CONTAINERSLIB_STRING_stringST_H_

// #include "containerslib/types.h"
#include "types.h"

typedef struct StringSt StringSt;

// Create an empty symbol table.
StringSt *stringst_init();

// Put key-value pair into the table: a[key] = val; .
void *stringst_put(StringSt *self, char *key, void *val);
// Value paired with key: a[key] .
void *stringst_get(StringSt *self, char *key);
// Is there a value paired with key?
bool stringst_contains(StringSt *self, char *key);
// Remove key (and its value) from table.
void *stringst_delete(StringSt *self, char *key);
// Is the table empty?
bool stringst_empty(StringSt *self);
// Number of key-value pairs in the table.
int stringst_size(StringSt *self);

// Smallest key.
char *stringst_min(StringSt *self);
// Largest key.
char *stringst_max(StringSt *self);
// Largest key less than or equal to key.
char *stringst_floor(StringSt *self, char *key);
// Smallest key greater than to equal to key.
char *stringst_ceiling(StringSt *self, char *key);
// Delete smallest key.
void *stringst_delmin(StringSt *self, char **out_key);
// Delete largest key.
void *stringst_delmax(StringSt *self, char **out_key);

typedef struct StringStIterator StringStIterator;

StringStIterator *stringst_iterator_init(StringSt* self);
bool stringst_iterator_has_next(StringStIterator *iterator);
void *stringst_iterator_next(StringStIterator *self, char **out_key);
void stringst_iterator_finish(StringStIterator* self);
// Visit all the key-value pairs in the order of their keys.
void stringst_traverse(StringSt *self, void (*visit)(char *,  void *));

// Clean up the table memory.
void stringst_finish(StringSt *self, free_fn free_fn);

#endif
