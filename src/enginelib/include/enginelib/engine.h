#ifndef _ENGINELIB_ENGINE_H_
#define _ENGINELIB_ENGINE_H_

#include <stdio.h>

#include "page_indexerlib/page_indexer.h"

void engine_run(Index *index, /* TODO: PR data,*/ FILE *in, FILE *out);

#endif
