#include "containerslib/string_set.h"
#include "containerslib/string_st.h"

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
        exception_throw_memory_failure("index_init - Failed to allocate memory for index");

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
    StringSet *pages = stringset_init();

    // make the supla clever heap-enabled parallel intersection
    

    return pages;
}

static void __index_finish_stringset(char *key, void *value)
{
    stringset_finish(value);
}

static void __index_finish_stringst(char *key, void *value)
{
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
