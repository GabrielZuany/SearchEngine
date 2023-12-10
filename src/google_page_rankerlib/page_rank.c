#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// #include "include/google_page_rankerlib/page_rank.h"
#include <math.h>
#include <stdbool.h>
// gcc -o teste_page_rank src/containerslib/node.c src/containerslib/forward_list.c src/containerslib/utils.c src/google_page_rankerlib/page_rank.c src/main.c src/containerslib/string_st.c src/containerslib/tst.c  src/containerslib/exceptions.c -g

// no launch.json
// "cwd": "/home/zuany/LinuxWorkEnvironmnet/SearchEngine/",
// "program": "/home/zuany/LinuxWorkEnvironmnet/SearchEngine/teste_page_rank",

#include "containerslib/exceptions.h"
#include "containerslib/string_st.h"
#include "containerslib/string_set.h"
#include "containerslib/forward_list.h"

#include "google_page_rankerlib/page_rank.h"

struct PageRank
{
    StringSt* page_rank_map; // St<string, double>
};

static PageRank* page_rank_init(StringSt* page_rank_map){
    PageRank* page_rank = malloc(sizeof *page_rank);

    page_rank->page_rank_map = page_rank_map;

    return page_rank;
}

static void __page_rank_linker(char **pages, StringSt *out_links_qty, StringSt *in_links, const char* graph_path){
    FILE* graph_file = fopen(graph_path, "r");
    if (graph_file == NULL)
        exception_throw_failure("page_rank_build - failed to open graph file");


    StringSet *pages_set = stringset_init();

    char *vertex_adj = NULL;
    size_t vertex_adj_length = 0;
    ssize_t read;
    while ((read = getline(&vertex_adj, &vertex_adj_length, graph_file)) != -1) {
        if (vertex_adj[read - 1] == '\n')
            vertex_adj[read - 1] = '\0';

        if (read < 2)
            continue;

        const char delim[] = " ";
        char *saveptr = NULL, *token = vertex_adj;
        token = strtok_r(token, delim, &saveptr);
        // now token is the origin page name

        char *origin_page_name = strdup(token);
        stringset_put(pages_set, token);

        token = strtok_r(NULL, delim, &saveptr);
        // now token is the number of out links
        
        size_t num_out_links = atoi(token);
        stringst_put(out_links_qty, origin_page_name, (size_t *)num_out_links);

        for (size_t i = 0; i < num_out_links; i++) {
            token = strtok_r(NULL, delim, &saveptr);
            // now token is the out link

            stringset_put(pages_set, token);

            ForwardList *curr_page_in_links = stringst_get(in_links, token);
            if (curr_page_in_links == NULL) {
                curr_page_in_links = forward_list_construct();
                stringst_put(in_links, token, curr_page_in_links);
            }

            forward_list_push_front(curr_page_in_links, origin_page_name);
        }
    }
    free(vertex_adj);
    fclose(graph_file);

    size_t curr = 0;
    StringSetIterator *iterator = stringset_iterator_init(pages_set);
    while (stringset_iterator_has_next(iterator)) {
        char *key = stringset_iterator_next(iterator);
        
        pages[curr++] = key;
    }
    stringset_iterator_finish(iterator);

    stringset_finish(pages_set);
}

static StringSt *__page_rank_ranker(size_t num_pages, char **pages, StringSt *out_links_qty, StringSt *in_links) {
    double *page_rank_k0 = malloc(num_pages * sizeof(double));
    double *page_rank_k1 = malloc(num_pages * sizeof(double));

    for (size_t i = 0; i < num_pages; i++) {
        page_rank_k0[i] = 1.0 / (double)num_pages;
        page_rank_k1[i] = 1.0 / (double)num_pages;
    }

    double sum_delta;
    do {
        sum_delta = 0.0;

        for (size_t i = 0; i < num_pages; i++) {
            char *page = pages[i];

            page_rank_k1[i] = (1 - DAMPING_FACTOR) / (double)num_pages;

            double in_sum = 0.0;
            ForwardList *curr_page_in_links = stringst_get(in_links, page);
            if (curr_page_in_links != NULL) {
                Node *curr_node = forward_list_get_head_node(curr_page_in_links);
                while (curr_node != NULL) {
                    char *in_link = node_get_value(curr_node);
                    size_t curr_in_link_out_links_qty = (size_t)stringst_get(out_links_qty, in_link);
                    in_sum += page_rank_k0[i] / (double)curr_in_link_out_links_qty;
                    curr_node = forward_list_goto_next(curr_node);
                }
            }

            page_rank_k1[i] += DAMPING_FACTOR * in_sum;

            size_t curr_page_out_links_num = (size_t)stringst_get(out_links_qty, page);
            if (curr_page_out_links_num == 0)
                page_rank_k1[i] += DAMPING_FACTOR * page_rank_k0[i];

            sum_delta += fabs(page_rank_k1[i] - page_rank_k0[i]);
        }

        for (size_t i = 0; i < num_pages; i++)
            page_rank_k0[i] = page_rank_k1[i];
    } while (sum_delta > EPSLON);

    free(page_rank_k0);

    StringSt *page_rank_k1_map = stringst_init();

    for (size_t i = 0; i < num_pages; i++) {
        char *page = pages[i];

        double *page_rank = malloc(sizeof(double));
        *page_rank = page_rank_k1[i];
        stringst_put(page_rank_k1_map, page, page_rank);
    }

    return page_rank_k1_map;
}

static void __page_rank_free_links(ForwardList *fwl) {
    Node *curr_node = forward_list_get_head_node(fwl);
    while (curr_node != NULL) {
        free(node_get_value(curr_node));
        curr_node = forward_list_goto_next(curr_node);
    }
    forward_list_destroy(fwl);
}

PageRank* page_rank_build(size_t num_pages, const char* graph_path) {
    StringSt* out_links_qty = stringst_init(); // St<string, size_t>
    StringSt* in_links = stringst_init(); // St<string, ForwardList<string>>

    char **pages = malloc(num_pages * __SIZEOF_POINTER__);
    __page_rank_linker(pages, out_links_qty, in_links, graph_path);

    StringSt *page_rank_map = __page_rank_ranker(num_pages, pages, out_links_qty, in_links);

    stringst_finish(out_links_qty, NULL);
    stringst_finish(in_links, (free_fn)__page_rank_free_links);
    for (size_t i = 0; i < num_pages; i++)
        free(pages[i]);
    
    PageRank* page_rank = page_rank_init(page_rank_map);

    return page_rank;
}

double page_rank_get_rank(PageRank* self, char* page){
    double* page_rank = stringst_get(self->page_rank_map, page);
    if(page_rank == NULL)
        exception_throw_failure("page_rank_get_rank - page not found");

    return *page_rank;
}

void page_rank_finish(PageRank* self){
    stringst_finish(self->page_rank_map, free);

    free(self);
}
