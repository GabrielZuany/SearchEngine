#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containerslib/string_set.h"
#include "containerslib/utils.h"
#include "containerslib/heap.h"
#include "containerslib/exceptions.h"
#include "google_page_rankerlib/page_rank.h"

#include "enginelib/search.h"

long long int enginelib_search(Index *index, PageRank *page_rank, FILE *in, Search *out) {
    char *query = NULL;
    size_t query_length = 0;
    ssize_t read;
    if ((read = getline(&query, &query_length, in)) == -1) {
        free(query);
        return -1;
    }

    if (query[read - 1] == '\n')
        query[read - 1] = '\0';

    utils_inplacestrtolower(query);

    StringSet *words = stringset_init();

    const char delim[] = " ";
    char *saveptr = NULL, *token = NULL;
    for (token = query;; token = NULL) {
        token = strtok_r(token, delim, &saveptr);
        if (token == NULL)
            break;

        stringset_put(words, token);
    }

    StringSet *pages = index_intersect_pages(index, words);

    stringset_finish(words, NULL);

    out->query = query;
    // TODO: extract PR given pages
    out->heap_pr_page = heap_init(MAX_HEAP, 16, sizeof(char *), (free_fn)free);
    StringSetIterator *iterator = stringset_iterator_init(pages);
    while (stringset_iterator_has_next(iterator)) {
        char *page = strdup(stringset_iterator_next(iterator));
        double pr = page_rank_get(page_rank, page);
        heap_push(out->heap_pr_page, &page, pr);
    }
    stringset_iterator_finish(iterator);

    stringset_finish(pages, NULL);

    return (long long int)heap_len(out->heap_pr_page);
}

Heap *enginelib_search_get_ranking(Search *search_result) {
    return search_result->heap_pr_page;
}

char *enginelib_search_get_query(Search *search_result) {
    return search_result->query;
}

void enginelib_search_clear(Search *search_result) {
    heap_free(search_result->heap_pr_page);
    free(search_result->query);
}
