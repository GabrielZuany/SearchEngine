#ifndef _CONTAINERSLIB_RBTREE_H_
#define _CONTAINERSLIB_RBTREE_H_

#include "types.h"

typedef struct RbTree RbTree;

RbTree *rbtree_init(cmp_fn key_cmp);

void *rbtree_put(RbTree *self, void *key, void *val, void **out_key);
void *rbtree_get(RbTree *self, void *key, void **out_key);
bool rbtree_contains(RbTree *self, void *key);
void *rbtree_delete(RbTree *self, void *key, void **out_key);
bool rbtree_empty(RbTree *self);
size_t rbtree_size(RbTree *self);

void *rbtree_min(RbTree *self);
void *rbtree_max(RbTree *self);
void *rbtree_floor(RbTree *self, void *key);
void *rbtree_ceiling(RbTree *self, void *key);
void *rbtree_delmin(RbTree *self, void **out_key);
void *rbtree_delmax(RbTree *self, void **out_key);
void rbtree_traverse(RbTree *self, void (*visit)(void *,  void *));

typedef struct RbTreeIterator RbTreeIterator;

RbTreeIterator* rbtree_iterator_init(RbTree *self);
bool rbtree_iterator_has_next(RbTreeIterator *iterator);
void *rbtree_iterator_next(RbTreeIterator *iterator, void **out_key);
void rbtree_iterator_free(RbTreeIterator* self);

void rbtree_finish(RbTree *self);

#endif
