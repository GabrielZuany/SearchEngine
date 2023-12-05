#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "containerslib/utils.h"

#include "page_indexerlib/page_indexer.h"

#include "containerslib/forward_list.h"
#include "google_page_rankerlib/page_rank.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <diretório_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input_directory = argv[1];

    char *stop_words_path = utils_pathcat(input_directory, "stop_words.txt");
    char *index_path = utils_pathcat(input_directory, "index.txt");
    char *pages_folder_path = utils_pathcat(input_directory, "pages/");

    StringSet *stop_words = pageindexer_read_stop_words(stop_words_path);
    Index *index = pageindexer_create(index_path, pages_folder_path, stop_words);

    char* graph_path = utils_pathcat(input_directory, "graph.txt");
    ForwardList** out_links = google_page_ranker_read_out_links(graph_path);

    free(stop_words_path);
    free(index_path);
    free(pages_folder_path);

    return 0;
}
