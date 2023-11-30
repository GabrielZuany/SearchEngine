#ifndef _PAGERANKERLIB_PAGE_RANK_H_
#define _PAGERANKERLIB_PAGE_RANK_H_
#define DAMPING_FACTOR 0.85
#define EPSILON 0.000001 // 10^-6

double get_page_rank(double *page_rank,  ForwardList** out_links, ForwardList** in_links, int n_pages, int page_id);

#endif