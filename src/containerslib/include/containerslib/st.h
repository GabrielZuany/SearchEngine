#ifndef _CONTAINERSLIB_ST_H_
#define _CONTAINERSLIB_ST_H_

#include "containerslib/types.h"

typedef struct St St;

// Create an empty symbol table.
St *st_init(cmp_fn key_cmp);

// Put key-value pair into the table: a[key] = val; .
void st_put(St *self, void *key, void *val);
// Value paired with key: a[key] .
void *st_get(St *self, void *key);
// Is there a value paired with key?
bool st_contains(St *self, void *key);
// Remove key (and its value) from table.
void st_delete(St *self, void **ref_key);
// Is the table empty?
bool st_empty(St *self);
// Number of key-value pairs in the table.
int st_size(St *self);

// Smallest key.
void *st_min(St *self);
// Largest key.
void *st_max(St *self);
// Largest key less than or equal to key.
void *st_floor(St *self, void *key);
// Smallest key greater than to equal to key.
void *st_ceiling(St *self, void *key);
// Delete smallest key.
void st_delmin(St *self, void **out_key);
// Delete largest key.
void st_delmax(St *self, void **out_key);
// Visit all the key-value pairs in the order of their keys.
void st_traverse(St *self, void (*visit)(void *,  void *));

// Clean up the table memory.
void st_finish(St *self);

#endif
