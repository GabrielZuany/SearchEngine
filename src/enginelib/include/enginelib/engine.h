#ifndef _ENGINELIB_ENGINE_H_
#define _ENGINELIB_ENGINE_H_

#include <stdio.h>

#include "google_page_rankerlib/page_rank.h"
#include "page_indexerlib/page_indexer.h"

void engine_run(Index *index, PageRank *page_rank, FILE *in, FILE *out);

#endif
