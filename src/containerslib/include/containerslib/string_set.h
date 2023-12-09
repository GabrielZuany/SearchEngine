#ifndef _CONTAINERSLIB_STRING_SET_H_
#define _CONTAINERSLIB_STRING_SET_H_

#include "types.h"
#include "tst.h"

typedef struct StringSet StringSet;

StringSet *stringset_init();

bool stringset_put(StringSet *self, const char *key);
bool stringset_contains(StringSet *self, char *key);
void stringset_delete(StringSet *self, char *key);
bool stringset_empty(StringSet *self);
int stringset_size(StringSet *self);

typedef struct StringSetIterator StringSetIterator;

StringSetIterator *stringset_iterator_init(StringSet* self);
bool stringset_iterator_has_next(StringSetIterator *iterator);
char *stringset_iterator_next(StringSetIterator *self);
void stringset_iterator_finish(StringSetIterator* self);
void stringset_traverse(StringSet *self, void (*visit)(char *));

void stringset_finish(StringSet *self, free_fn free_fn);

#endif
