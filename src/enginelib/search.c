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

static Page_data* __page_data_construct(char *page, double page_rank) {
    Page_data *page_data = malloc(sizeof(Page_data));
    page_data->page = page;
    page_data->page_rank = page_rank;
    return page_data;
}

static int __page_data_cmp(const Page_data **a, const Page_data **b) {
    // Os documentos devem ser listados em ordem decrescente de PageRank.
    // Empates no PageRank devem ser quebrados pela ordem lexicográfica, crescente, do nome dos documentos.

    const Page_data *page_data_a = *a;
    const Page_data *page_data_b = *b;

    double page_rank_a = page_data_a->page_rank;
    double page_rank_b = page_data_b->page_rank;

    if (page_rank_a < page_rank_b)
        return -1;
    else if (page_rank_a > page_rank_b)
        return 1;
    else
        return strcmp(page_data_a->page, page_data_b->page);
}

void __page_data_free_fn(Page_data **data) {
    Page_data *page_data = *data;
    free(page_data->page);
    free(page_data);
}

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

    out->query = strdup(query);

    StringSet *words = stringset_init();

    const char delim[] = " ";
    char *saveptr = NULL, *token = NULL;
    for (token = query;; token = NULL) {
        token = strtok_r(token, delim, &saveptr);
        if (token == NULL)
            break;

        stringset_put(words, token);
    }
    free(query);

    StringSet *pages = index_intersect_pages(index, words);

    stringset_finish(words, NULL);

    // TODO: extract PR given pages
    out->heap_pr_page = heap_init(2, __SIZEOF_POINTER__, (free_fn)__page_data_free_fn, (cmp_fn)__page_data_cmp);
    StringSetIterator *iterator = stringset_iterator_init(pages);
    while (stringset_iterator_has_next(iterator)) {
        char *page = strdup(stringset_iterator_next(iterator));
        double pr = page_rank_get(page_rank, page);
        Page_data *page_data = __page_data_construct(page, pr);
        heap_push(out->heap_pr_page, &page_data, (size_t)page_data);
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
