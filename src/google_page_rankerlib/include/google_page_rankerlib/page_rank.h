#ifndef _PAGERANKERLIB_PAGE_RANK_H_
#define _PAGERANKERLIB_PAGE_RANK_H_

#define DAMPING_FACTOR 0.85
#define EPSLON 0.000001 // 10^-6

typedef struct PageRank PageRank;

PageRank* page_rank_build(size_t num_pages, const char* graph_path);

double page_rank_get_rank(PageRank* self, char* page);

void page_rank_finish(PageRank* self);

#endif