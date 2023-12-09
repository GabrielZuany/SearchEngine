#ifndef _CONTAINERSLIB_TST_H_
#define _CONTAINERSLIB_TST_H_

typedef struct TST TST;
#define data_type void*
#include "types.h"

TST* TST_init();
data_type TST_put(TST* self, const char* key, data_type val);
data_type TST_search(TST* self, char* key);
void TST_free(TST* self, free_fn free_fn);
int TST_empty(TST* self);
int TST_size(TST* self);

typedef struct TST_iterator TST_iterator;

TST_iterator* TST_iterator_init(TST* self);
bool TST_iterator_has_next(TST_iterator *iterator);
data_type TST_iterator_next(TST_iterator *self, char **out_key);
void TST_iterator_free(TST_iterator* self);
void TST_traverse(TST *tst, void (*visit)(char *, data_type));

#endif