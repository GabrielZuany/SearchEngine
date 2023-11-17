#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containerslib/string_set.h"
#include "containerslib/string_st.h"
#include "containerslib/exceptions.h"

#include "page_indexerlib/page_indexer.h"

Index *pageindexer_create(const char *index_path, const char *pages_folder_path, StringSet *stop_words) {
    StringSt *word_page_map = stringst_init(); // StringSt<string, DisjointSet>
    StringSt *page_id_map = stringst_init(); // StringSt<string, int>

    // TODO
    return NULL;
}

StringSet *pageindexer_read_stop_words(const char *stop_words_path) {
    FILE *stop_words_file = fopen(stop_words_path, "r");
    if (stop_words_file == NULL)
        exception_throw_failure("pageindexer_read_stop_words - Failed to open stop words file");

    StringSet *stop_words = stringset_create();

    char *stop_word = NULL;
    size_t stop_word_length = 0;
    while (getline(&stop_word, &stop_word_length, stop_words_file) != -1) {
        if (stop_word[stop_word_length - 1] == '\n')
            stop_word[stop_word_length - 1] = '\0';

        stringset_add(stop_words, stop_word);
    }

    free(stop_word);
    fclose(stop_words_file);

    return stop_words;
}
