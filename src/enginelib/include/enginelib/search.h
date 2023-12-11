#ifndef _ENGINELIB_SEARCH_H_
#define _ENGINELIB_SEARCH_H_

#include <stdio.h>

#include "containerslib/heap.h"
#include "page_indexerlib/index.h"
#include "google_page_rankerlib/page_rank.h"

typedef struct Search Search;

struct Search
{
    char *query;
    Heap *heap_pr_page; // Heap<int, string>
};

typedef struct{
    char *page;
    double page_rank;
} Page_data;

void __page_data_clean(Page_data *data);

long long int enginelib_search(Index *index, PageRank *page_rank, FILE *in, Search *out);

Heap *enginelib_search_get_ranking(Search *search_result);

char *enginelib_search_get_query(Search *search_result);

void enginelib_search_clear(Search *search_result);

#endif
