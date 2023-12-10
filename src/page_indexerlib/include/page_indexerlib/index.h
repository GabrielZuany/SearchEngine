#ifndef _PAGE_INDEXERLIB_INDEX_H_
#define _PAGE_INDEXERLIB_INDEX_H_

#include "containerslib/string_set.h"
#include "containerslib/string_st.h"

typedef struct Index Index;

StringSt* index_get_page_idpage_map(Index* index);

Index *index_init(StringSt *word_idspageset_map, StringSt *page_idpage_map, char **idpage_page_map, size_t len, size_t size, StringSet *ref_stop_words);

StringSet *index_get_ref_stop_words(Index *index);

size_t index_get_num_pages(Index *index);

StringSet *index_intersect_pages(Index *index, StringSet *words);

void index_finish(Index *index);

#endif
