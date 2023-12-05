#ifndef _PAGERANKERLIB_PAGE_RANK_H_
#define _PAGERANKERLIB_PAGE_RANK_H_

#define DAMPING_FACTOR 0.85
#define EPSILON 0.000001 // 10^-6

#include "../../../containerslib/include/containerslib/forward_list.h"

double get_page_rank(double *page_rank,  ForwardList** out_links, ForwardList** in_links, int n_pages, int page_id);
ForwardList** google_page_ranker_read_links(char* graph_path);
ForwardList* get_out_links_from_page(ForwardList** out_links, char* filename);

#endif