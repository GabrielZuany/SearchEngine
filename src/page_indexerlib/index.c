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

StringSt* index_get_page_idpage_map(Index* index){
    return index->page_idpage_map;
}

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

static void __index_intersect_pages_heap_derrefed_free_fn(SetIterator **set)
{
    set_iterator_finish(*set);
}

static int __common_double_cmp(const int *a, const int *b)
{
    return *b - *a;
}

StringSet *index_intersect_pages(Index *index, StringSet *words) {
    StringSet *pages = stringset_init(); // Set<string>

    Heap *heap = heap_init(2, sizeof(size_t), __SIZEOF_POINTER__, (free_fn)__index_intersect_pages_heap_derrefed_free_fn, (cmp_fn)__common_double_cmp);

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
        size_t next = (size_t)set_iterator_next(iter);
        heap_push(heap, &next, &iter);
    }
    stringset_iterator_finish(iterator);

    size_t page_id = 0;
    size_t page_count = 0;
    size_t num_words = heap_len(heap);
    bool lazy_break = false;
    while (heap_len(heap) > 0) {
        size_t curr_page_id;
        SetIterator *curr_iter;
        heap_pop(heap, &curr_page_id, &curr_iter);
        if (page_id != curr_page_id) {
            if(lazy_break) {
                set_iterator_finish(curr_iter);
                break;
            }
            page_id = curr_page_id;
            page_count = 1;
        } else {
            page_count++;
        }

        if (page_count == num_words) {
            stringset_put(pages, __index_get_page_from_pageid(index, page_id));

            page_id = 0;
            page_count = 0;
        }

        if (set_iterator_has_next(curr_iter)) {
            size_t next = (size_t)set_iterator_next(curr_iter);
            heap_push(heap, &next, &curr_iter);
        }
        else {
            set_iterator_finish(curr_iter);
            lazy_break = true;
        }
    }
    heap_free(heap);

    return pages;
}

size_t index_get_num_pages(Index *index) {
    return index->len;
}

void index_finish(Index *index)
{
    stringst_finish(index->word_idspageset_map, (free_fn)set_finish);

    stringst_finish(index->page_idpage_map, NULL);

    for (size_t i = 0; i < index->len; i++)
        free(index->idpage_page_map[i]);

    free(index->idpage_page_map);    

    free(index);
}
