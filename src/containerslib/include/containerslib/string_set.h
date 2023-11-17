#ifndef _CONTAINERSLIB_STRING_SET_H_
#define _CONTAINERSLIB_STRING_SET_H_

#include "containerslib/types.h"

typedef struct StringSet StringSet;

StringSet *stringset_init();

bool stringset_put(StringSet *self, char *key);
bool stringset_contains(StringSet *self, char *key);
void stringset_delete(StringSet *self, char *key);
bool stringset_empty(StringSet *self);
int stringset_size(StringSet *self);

void stringset_traverse(StringSet *self, void (*visit)(char *));

void stringset_finish(StringSet *self);

#endif
