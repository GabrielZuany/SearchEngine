#ifndef _PAGE_INDEXERLIB_INDEX_H_
#define _PAGE_INDEXERLIB_INDEX_H_

typedef struct Index Index;

Index *index_init(StringSt *word_pages_map);

StringSet *index_intersect_pages(Index *index, StringSt *word_pages_subset);

void index_finish(Index *index);

#endif
