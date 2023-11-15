#ifndef _CONTAINERSLIB_RBTREE_H_
#define _CONTAINERSLIB_RBTREE_H_

#include "containerslib/types.h"

typedef struct RbTree RbTree;

RbTree *rbtree_init(cmp_fn key_cmp);

void rbtree_put(RbTree *self, void *key, void *val);
void *rbtree_get(RbTree *self, void *key);
bool rbtree_contains(RbTree *self, void *key);
void *rbtree_delete(RbTree *self, void **ref_key);
bool rbtree_empty(RbTree *self);
size_t rbtree_size(RbTree *self);

void *rbtree_min(RbTree *self);
void *rbtree_max(RbTree *self);
void *rbtree_floor(RbTree *self, void *key);
void *rbtree_ceiling(RbTree *self, void *key);
void *rbtree_delmin(RbTree *self, void **out_key);
void *rbtree_delmax(RbTree *self, void **out_key);
void rbtree_traverse(RbTree *self, void (*visit)(void *,  void *));

void rbtree_finish(RbTree *self);

#endif
