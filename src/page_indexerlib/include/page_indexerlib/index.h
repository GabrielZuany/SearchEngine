#ifndef _PAGE_INDEXERLIB_INDEX_H_
#define _PAGE_INDEXERLIB_INDEX_H_

typedef struct Index Index;

Index *index_init(StringSt *word_pages_map, StringSet *ref_stop_words);

StringSet *index_get_ref_stop_words(Index *index);

StringSet *index_intersect_pages(Index *index, StringSet *words);

void index_finish(Index *index);

#endif
