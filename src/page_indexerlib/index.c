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
    exception_throw_failure("index_intersect_pages - Not implemented");

    StringSet *pages = stringset_init();

    // TODO: implementar
    if (index) {}; // warning suppression hihiih
    if (__index_get_page_from_pageid(index, 0)) {}; // warning suppression hihiih
    if (__index_get_pageid_from_pagename(index, NULL)) {}; // warning suppression hihiih

    // giovanni estou usando uma funcao do fun with bits esta orgulhoso?
    Heap *heap = heap_init(MIN_HEAP, __builtin_clz(stringset_size(words)), sizeof(int), NULL);
    //                     ^^^^^^^^
    //                    stolen https://stackoverflow.com/a/3272516

    //TST_iterator *iterator = 

    if (heap) {}; // warning suppression hihiih

    return pages;
}

static void __index_finish_stringset(char *key, void *value)
{
    if (key) {}; // warning suppression hihiih
    set_finish(value);
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
