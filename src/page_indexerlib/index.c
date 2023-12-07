#include "containerslib/string_set.h"
#include "containerslib/string_st.h"
#include "containerslib/heap.h"
#include "containerslib/exceptions.h"

#include "page_indexerlib/index.h"

struct Index
{
    StringSt *word_idspageset_map; // St<string, Set<int>>
    StringSt *page_idpage_map; // St<string, int>
    StringSt *idpage_page_map; // St<int, string>
    StringSet *ref_stop_words; // Set<string>
};

Index *index_init(StringSt *word_idspageset_map, StringSt *page_idpage_map, StringSt *idpage_page_map, StringSet *ref_stop_words)
{
    Index *index = malloc(sizeof(Index));
    if (index == NULL)
        exception_throw_oom("index_init - Failed to allocate memory for index");

    index->word_idspageset_map = word_idspageset_map;
    index->page_idpage_map = page_idpage_map;
    index->idpage_page_map = idpage_page_map;
    index->ref_stop_words = ref_stop_words;

    return index;
}

StringSet *index_get_ref_stop_words(Index *index) {
    return index->ref_stop_words;
}

StringSet *index_intersect_pages(Index *index, StringSet *words) {
    exception_throw_failure("index_intersect_pages - Not implemented");

    StringSet *pages = stringset_init();

    // TODO: implementar
    if (index) {}; // warning suppression hihiih

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
    stringset_finish(value);
}

static void __index_finish_stringst(char *key, void *value)
{
    if (value) {}; // warning suppression hihiih
    free(key);
}

void index_finish(Index *index)
{
    stringst_traverse(index->word_idspageset_map, __index_finish_stringset);
    stringst_finish(index->word_idspageset_map);

    stringst_finish(index->page_idpage_map);

    stringst_traverse(index->idpage_page_map, __index_finish_stringst);
    stringst_finish(index->idpage_page_map);

    free(index);
}
