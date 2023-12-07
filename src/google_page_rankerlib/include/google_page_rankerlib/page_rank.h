#ifndef _PAGERANKERLIB_PAGE_RANK_H_
#define _PAGERANKERLIB_PAGE_RANK_H_

#define DAMPING_FACTOR 0.85
#define EPSILON 0.000001 // 10^-6

// #include "../../../containerslib/include/containerslib/forward_list.h"
// #include "../../../containerslib/include/containerslib/string_st.h"
#include "containerslib/forward_list.h"
#include "containerslib/string_st.h"


typedef struct PageRank PageRank;

PageRank* page_rank_init(int n_pages);
void page_rank_finish(PageRank* self);
PageRank* page_rank_build_links(PageRank* self, char* graph_path);
ForwardList* get_out_links_from_page(PageRank* self, char* filename);
ForwardList* get_in_links_from_page(PageRank* self, char* filename);
double page_rank_get(PageRank* self, char* page);
double* page_rank_get_all(PageRank* self);

#endif