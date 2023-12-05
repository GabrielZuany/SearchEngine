#ifndef _ENGINELIB_SEARCH_H_
#define _ENGINELIB_SEARCH_H_

#include "containerslib/heap.h"

#include "page_indexerlib/index.h"

typedef struct Search Search;

struct Search
{
    char *query;
    Heap *heap_pr_page; // Heap<int, string>
};

size_t enginelib_search(Index *index, /* TODO: PR data,*/ FILE *in, Search *out);

Heap *enginelib_search_get_ranking(Search *search_result);

char *enginelib_search_get_query(Search *search_result);

void enginelib_search_clear(Search *search_result);

#endif
