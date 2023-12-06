#ifndef _CONTAINERSLIB_TST_H_
#define _CONTAINERSLIB_TST_H_

typedef struct TST TST;
#define data_type void*
#include "containerslib/types.h"

TST* TST_init();
bool TST_insert(TST* self, char* key, data_type val);
data_type TST_search(TST* self, char* key);
void TST_free(TST* self, free_fn free_val);
int TST_empty(TST* self);
int TST_size(TST* self);

typedef struct TSTIterator TSTIterator;

// TSTIterator* TST_iterator_init(TST* self);
// key_value TST_iterator_next(TSTIterator* self);
// void TST_iterator_free(TSTIterator* self);
// void TST_traverse(TST *tst, void (*visit)(char *, data_type));

#endif