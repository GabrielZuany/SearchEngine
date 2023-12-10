#include <stdio.h>
#include <stdlib.h>

#include "containerslib/utils.h"

#include "page_indexerlib/index.h"
#include "page_indexerlib/page_indexer.h"

#include "google_page_rankerlib/page_rank.h"

#include "enginelib/engine.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <diretório_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input_directory = argv[1];

    char *graph_path = utils_pathcat(input_directory, "graph.txt");
    char *index_path = utils_pathcat(input_directory, "index.txt");
    char *pages_folder_path = utils_pathcat(input_directory, "pages/");
    char *stop_words_path = utils_pathcat(input_directory, "stopwords.txt");

    StringSet *stop_words = pageindexer_read_stop_words(stop_words_path);
    Index *index = pageindexer_create(index_path, pages_folder_path, stop_words);

    PageRank* page_rank = page_rank_init(index);
    page_rank_build_links(page_rank, input_directory, graph_path);

    engine_run(index, page_rank, stdin, stdout);

    free(graph_path);
    free(index_path);
    free(pages_folder_path);
    free(stop_words_path);

    stringset_finish(stop_words, NULL);

    index_finish(index);
    page_rank_finish(page_rank);

    return 0;
}
