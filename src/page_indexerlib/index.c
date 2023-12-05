#include "containerslib/string_set.h"
#include "containerslib/string_st.h"

#include "page_indexerlib/index.h"

struct Index
{
    StringSt *word_pages_map;
    StringSet *ref_stop_words;
};

Index *index_init(StringSt *word_pages_map, StringSet *ref_stop_words)
{
    Index *index = malloc(sizeof(Index));
    if (index == NULL)
        exception_throw_memory_failure("index_init - Failed to allocate memory for index");

    index->word_pages_map = word_pages_map;
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

void index_finish(Index *index)
{
    stringst_traverse(index->word_pages_map, __index_finish_stringset);
    stringst_finish(index->word_pages_map);

    free(index);
}
