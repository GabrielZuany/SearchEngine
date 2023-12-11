#ifndef _CONTAINERSLIB_HEAP_H_
#define _CONTAINERSLIB_HEAP_H_

#include <stdlib.h>
#include <stdbool.h>

#include "containerslib/types.h"

typedef struct Heap Heap;

Heap *heap_init(size_t initialPow2Capacity, size_t ksmemb, size_t vsmemb, free_fn vfreer, cmp_fn kcmp);

void heap_push(Heap *self, void *key, void *value);

void heap_pop(Heap *self, void *out_key, void *out_value);

void heap_peek(Heap *self, void *out__key, void *out_value);

size_t heap_len(Heap *self);

void heap_free(Heap *self);

#endif
