#ifndef _PAGERANKERLIB_PAGE_RANK_H_
#define _PAGERANKERLIB_PAGE_RANK_H_

#define DAMPING_FACTOR 0.85
#define EPSLON 0.000001 // 10^-6

#include "containerslib/forward_list.h"
#include "containerslib/string_st.h"
#include "page_indexerlib/index.h"

typedef struct PageRank PageRank;

PageRank* page_rank_init(Index* index);
void page_rank_finish(PageRank* self);
PageRank* page_rank_build_links(PageRank* self, char* folderpath);
double page_rank_get(PageRank* self, char* page);

#endif