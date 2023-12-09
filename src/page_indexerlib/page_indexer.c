#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "containerslib/string_set.h"
#include "containerslib/string_st.h"
#include "containerslib/exceptions.h"
#include "containerslib/utils.h"
#include "containerslib/set.h"

#include "page_indexerlib/index.h"
#include "page_indexerlib/page_indexer.h"

static inline void __pageindexer_index_page_line(char *line, StringSet *stop_words, StringSt *word_idspageset_map, size_t page_id);
static inline void __pageindexer_index_page(char *page_name, size_t page_id, const char *pages_folder_path, StringSet *stop_words, StringSt *word_idspageset_map);

Index *pageindexer_create(const char *index_path, const char *pages_folder_path, StringSet *stop_words) {
    StringSt *word_idspageset_map = stringst_init(); // St<string, Set<int>>
    StringSt *page_idpage_map = stringst_init(); // St<string, int>

    size_t len = 0;
    size_t size = 4;
    char **idpage_page_map = malloc(__SIZEOF_POINTER__ * size);

    // iterate over all lines in index
    FILE *index_file = fopen(index_path, "r");
    if (index_file == NULL)
        exception_throw_failure("pageindexer_create - Failed to open index file");

    char *page_name = NULL;
    size_t page_name_length = 0;
    ssize_t read;
    while ((read = getline(&page_name, &page_name_length, index_file)) != -1) {
        if (page_name[read - 1] == '\n')
            page_name[read - 1] = '\0';

        char *strdup_page_name = strdup(page_name);
        if (len == size) {
            size <<= 1;
            idpage_page_map = realloc(idpage_page_map, __SIZEOF_POINTER__ * size);
        }
        idpage_page_map[len++] = strdup_page_name;
        stringst_put(page_idpage_map, strdup_page_name, (size_t *)len);

        __pageindexer_index_page(strdup_page_name, len, pages_folder_path, stop_words, word_idspageset_map);
    }

    /* free(page_name); */
    fclose(index_file);

    Index *index = index_init(word_idspageset_map, page_idpage_map, idpage_page_map, len, size, stop_words);

    return index;
}

StringSet *pageindexer_read_stop_words(const char *stop_words_path) {
    FILE *stop_words_file = fopen(stop_words_path, "r");
    if (stop_words_file == NULL)
        exception_throw_failure("pageindexer_read_stop_words - Failed to open stop words file");

    StringSet *stop_words = stringset_init();

    char *stop_word = NULL;
    size_t stop_word_length = 0;
    ssize_t read;
    while ((read = getline(&stop_word, &stop_word_length, stop_words_file)) != -1) {
        if (stop_word[read - 1] == '\n')
            stop_word[read - 1] = '\0';

        utils_inplacestrtolower(stop_word);

        stringset_put(stop_words, stop_word);
    }

    /* free(stop_word); */
    fclose(stop_words_file);

    return stop_words;
}

static inline void __pageindexer_index_page(char *page_name, size_t page_id, const char *pages_folder_path, StringSet *stop_words, StringSt *word_idspageset_map) {
    char *page_path = utils_pathcat(pages_folder_path, page_name);

    FILE *page_file = fopen(page_path, "r");
    if (page_file == NULL)
        exception_throw_failure("pageindexer_index_page - Failed to open page file");

    free(page_path);

    char *line = NULL;
    size_t line_length = 0;
    ssize_t read;
    while ((read = getline(&line, &line_length, page_file)) != -1) {
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        utils_inplacestrtolower(line);

        __pageindexer_index_page_line(line, stop_words, word_idspageset_map, page_id);
    }

    /* free(line); */
    fclose(page_file);
}

int __pageindexer_pageidsset_cmp(const size_t *a, const size_t *b) {
    return (size_t)a - (size_t)b;
}

static inline void __pageindexer_index_page_line(char *line, StringSet *stop_words, StringSt *word_idspageset_map, size_t page_id) {
    const char delim[] = " \t\n";

    char *saveptr = NULL, *token = NULL;
    for (token = line;; token = NULL) {
        token = strtok_r(token, delim, &saveptr);
        if (token == NULL)
            break;

        char *word = token;
        if (stringset_contains(stop_words, word))
            continue;

        Set *pages = stringst_get(word_idspageset_map, word);
        if (pages == NULL) {
            pages = set_init((cmp_fn)__pageindexer_pageidsset_cmp);
            stringst_put(word_idspageset_map, word, pages);
        }

        set_put(pages, (size_t *)page_id);
    }
}
