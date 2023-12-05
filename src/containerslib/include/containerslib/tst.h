#ifndef _CONTAINERSLIB_TST_H_
#define _CONTAINERSLIB_TST_H_

typedef struct TST TST;
#define data_type void*

typedef struct key_value {
    char* key;
    data_type val;
} key_value;

TST* TST_init();
key_value TST_insert(TST* self, char* key, data_type val);
data_type TST_search(TST* self, char* key);
void TST_free(TST* self);
int TST_empty(TST* self);
int TST_size(TST* self);

typedef struct TSTIterator TSTIterator;

TSTIterator* TST_iterator_init(TST* self);
key_value TST_iterator_next(TSTIterator* self);
void TST_iterator_free(TSTIterator* self);
void TST_traverse(TST *tst, void (*visit)(char *, data_type));

#endif