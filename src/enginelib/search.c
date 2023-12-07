#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containerslib/string_set.h"
#include "containerslib/utils.h"
#include "containerslib/heap.h"

#include "enginelib/search.h"

long long int enginelib_search(Index *index, /* TODO: PR data,*/ FILE *in, Search *out) {
    char *query = NULL;
    size_t query_length = 0;
    ssize_t read;
    if ((read = getline(&query, &query_length, in)) == -1)
        return -1;

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

    out->query = query;
    // TODO: extract PR given pages
    //out->heap_pr_page = heap_init();

    // extract PR given pages
    if (pages) {}; //TODO

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
