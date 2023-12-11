#include "enginelib/search.h"

#include "enginelib/engine.h"

void engine_run(Index *index, PageRank *page_rank, FILE *in, FILE *out) {
    Search search_result;
    long long int results;
    while ((results = enginelib_search(index, page_rank, in, &search_result)) != -1)
    {
        Heap *ranking = enginelib_search_get_ranking(&search_result);

        fprintf(out, "search:%s\n", enginelib_search_get_query(&search_result));

        fprintf(out, "pages:");
        double prs[results];
        for (long long int i = 0; i < results; i++)
        {
            Page_data *page;
            (heap_pop(ranking, &page));
            prs[i] = page->page_rank;

            fprintf(out, "%s ", page->page);

            __page_data_free_fn(&page);
        }
        fprintf(out, "\n");

        fprintf(out, "pr:");
        for (long long int i = 0; i < results; i++)
            fprintf(out, "%.16lf ", prs[i]);
        
        fprintf(out, "\n");

        enginelib_search_clear(&search_result);
    }
}
