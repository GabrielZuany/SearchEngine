#ifndef _PAGE_INDEXERLIB_PAGE_INDEXER_H_
#define _PAGE_INDEXERLIB_PAGE_INDEXER_H_

#include "containerslib/string_set.h"

#include "page_indexerlib/index.h"

Index *pageindexer_create(const char *index_path, const char *pages_folder_path, StringSet *stop_words);

StringSet *pageindexer_read_stop_words(const char *stop_words_path);

#endif
