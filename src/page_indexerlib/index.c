#include <stdio.h>
#include "containerslib/string_set.h"
#include "containerslib/string_st.h"
#include "containerslib/heap.h"
#include "containerslib/exceptions.h"
#include "containerslib/set.h"

#include "page_indexerlib/index.h"

struct Index
{
    StringSt *word_idspageset_map; // St<string, Set<size_t>>
    StringSt *page_idpage_map; // St<string, size_t> page_name -> page_id

    char **idpage_page_map; // [page_id - 1] -> page
    size_t len;
    size_t size;

    StringSet *ref_stop_words; // Set<string>
};

Index *index_init(StringSt *word_idspageset_map, StringSt *page_idpage_map, char **idpage_page_map, size_t len, size_t size, StringSet *ref_stop_words)
{
    Index *index = malloc(sizeof(Index));
    if (index == NULL)
        exception_throw_oom("index_init - Failed to allocate memory for index");

    index->word_idspageset_map = word_idspageset_map;
    index->page_idpage_map = page_idpage_map;

    index->idpage_page_map = idpage_page_map;
    index->len = len;
    index->size = size;

    index->ref_stop_words = ref_stop_words;

    return index;
}

StringSet *index_get_ref_stop_words(Index *index) {
    return index->ref_stop_words;
}

static char *__index_get_page_from_pageid(Index *index, size_t page_id)
{
    return index->idpage_page_map[page_id - 1];
}

static size_t __index_get_pageid_from_pagename(Index *index, char *page_name)
{
    size_t *page_id = stringst_get(index->page_idpage_map, page_name);

    return (size_t)page_id;
}

StringSet *index_intersect_pages(Index *index, StringSet *words) {
    StringSet *pages = stringset_init(); // Set<string>

    Heap *heap = heap_init(MIN_HEAP, 2, __SIZEOF_POINTER__, (free_fn)set_iterator_finish);

    StringSetIterator *iterator = stringset_iterator_init(words);
    while (stringset_iterator_has_next(iterator)) {
        char *word = stringset_iterator_next(iterator);

        if (stringset_contains(index->ref_stop_words, word))
            continue;

        Set *idspageset = stringst_get(index->word_idspageset_map, word);
        if (idspageset == NULL) {
            stringset_iterator_finish(iterator);
            return pages;
        }

        SetIterator *iter = set_iterator_init(idspageset);
        if (set_iterator_has_next(iter))
            heap_push(heap, &iter, (size_t)set_iterator_next(iter));
    }
    stringset_iterator_finish(iterator);

    size_t page_id = 0;
    size_t page_count = 0;
    size_t num_words = heap_len(heap);
    while (heap_len(heap) > 0) {
        SetIterator *curr_iter;
        size_t curr_page_id = heap_pop(heap, &curr_iter);
        if (page_id != curr_page_id) {
            page_id = curr_page_id;
            page_count = 1;
        } else {
            page_count++;
        }

        if (page_count == num_words) {
            stringset_put(pages, __index_get_page_from_pageid(index, page_id));
            page_count = 0;
            page_id = 0;
        }

        if (set_iterator_has_next(curr_iter))
            heap_push(heap, &curr_iter, (size_t)set_iterator_next(curr_iter));
        else
            break;
    }
    heap_free(heap);

    return pages;
}

static void __index_finish_stringset(char *key, void *value)
{
    if (key) {}; // warning suppression hihiih
    set_finish(value);
}

size_t index_get_num_pages(Index *index) {
    return index->len;
}

void index_finish(Index *index)
{
    stringst_traverse(index->word_idspageset_map, __index_finish_stringset);
    stringst_finish(index->word_idspageset_map);

    stringst_finish(index->page_idpage_map);

    for (size_t i = 0; i < index->len; i++)
        free(index->idpage_page_map[i]);

    free(index->idpage_page_map);    

    free(index);
}
