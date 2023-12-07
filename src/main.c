#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "containerslib/forward_list.h"
#include "containerslib/utils.h"

#include "page_indexerlib/page_indexer.h"

#include "google_page_rankerlib/page_rank.h"

#include "enginelib/engine.h"
#include "enginelib/search.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <diretório_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input_directory = argv[1];

    char* graph_path = utils_pathcat(input_directory, "graph.txt");
    char *index_path = utils_pathcat(input_directory, "index.txt");
    char *pages_folder_path = utils_pathcat(input_directory, "pages/");
    char *stop_words_path = utils_pathcat(input_directory, "stop_words.txt");

    StringSet *stop_words = pageindexer_read_stop_words(stop_words_path);
    Index *index = pageindexer_create(index_path, pages_folder_path, stop_words);

    int n_pages = 0;
    PageRank* page_rank = page_rank_init(n_pages);
    page_rank_build_links(page_rank, graph_path);
    double page_rank_value = page_rank_get(page_rank, 0);

    // TODO remove
    if (page_rank_value) {}; // warning suppression hihiih

    engine_run(index, /* TODO: PR data,*/ stdin, stdout);

    free(graph_path);
    free(index_path);
    free(pages_folder_path);
    free(stop_words_path);

    stringset_finish(stop_words);

    index_finish(index);

    return 0;
}
