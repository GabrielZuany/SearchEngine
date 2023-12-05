#ifndef _CONTAINERSLIB_TST_H_
#define _CONTAINERSLIB_TST_H_

typedef struct TST TST;
#define data_type void*

TST* TST_init();
TST* TST_insert(TST* self, char* key, data_type val);
data_type TST_search(TST* self, char* key);
void TST_free(TST* self);

#endif