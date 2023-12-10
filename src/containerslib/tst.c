#include <stdlib.h>
#include <string.h>
#include "containerslib/exceptions.h"
#include "containerslib/tst.h"
#include "containerslib/types.h"
#include "containerslib/forward_list.h"

typedef struct node node;

struct node {
    data_type val;
    unsigned char c;
    struct node *l, *m, *r;
};

struct TST_iterator_node_string {
    node *node;
    char *string;
};

struct TST_iterator_node_string *TST_iterator_node_string_init(node *node, char *string) {
    struct TST_iterator_node_string *node_string = malloc(sizeof(*node_string));
    if (node_string == NULL)
        exception_throw_oom("TST_iterator_node_string_init - Failed to allocate memory for node_string");

    node_string->node = node;
    node_string->string = string;

    return node_string;
}

struct TST {
    node* root;
    int size;
    ForwardList *iter;
};

static node* create_node() {
    node* n = malloc(sizeof(*n));
    n->val = NULL;
    n->c = '\0';
    n->l = NULL;
    n->m = NULL;
    n->r = NULL;
    return n;
}

TST* TST_init() {
    TST* t = malloc(sizeof(*t));
    t->root = NULL;
    t->size = 0;
    t->iter = forward_list_construct();
    return t;
}

// imagino que quebre se a string for vazia, favor nao ousar
static node* rec_put(TST *tst, node* t, const char* key, data_type val, data_type *out_val, const char * const key_orig) {
    const unsigned char c = *key;
    if (t == NULL) { t = create_node(); t->c = c; }
    if      (c < t->c) { t->l = rec_put(tst, t->l, key, val, out_val, key_orig); }
    else if (c > t->c) { t->r = rec_put(tst, t->r, key, val, out_val, key_orig); }
    else if (key[1] != '\0') {
        t->m = rec_put(tst, t->m, ++key, val, out_val, key_orig);
    }
    else {
        if (t->val == NULL)
            forward_list_push_front(tst->iter, TST_iterator_node_string_init(t, strdup(key_orig)));

        *out_val = t->val;
        t->val = val;
    }
    return t;
}

data_type TST_put(TST* t, const char* key , data_type val) {
    data_type out_val;
    node *n = rec_put(t, t->root, key, val, &out_val, key);

    if (t->root == NULL) {
        t->root = n;
    }

    t->size++;

    return out_val;
}

// favor nao buscar strings vazias
static node* rec_search(node* t, char* key) {
    if (t == NULL) { return NULL; }
    unsigned char c = *key;
    if      (c < t->c) { return rec_search(t->l, key); }
    else if (c > t->c) { return rec_search(t->r, key); }
    else if (key[1] != '\0') {
        return rec_search(t->m, ++key);
    } else { return t; }
}

data_type TST_search(TST* t, char* key) {
    node *n = rec_search(t->root, key);
    if (n == NULL)  { return NULL; } // se for modificar o tipo de retorno, favor mudar aqui
    else            { return n->val; }
}

int TST_empty(TST* t) {
    return t->root == NULL;
}

int TST_size(TST* t) {
    return t->size;
}

static void rec_free(node* t, free_fn free_fn) {
    if (t == NULL) { return; }
    rec_free(t->l, free_fn);
    rec_free(t->m, free_fn);
    rec_free(t->r, free_fn);
    if (t->val != NULL && free_fn != NULL) { free_fn(t->val); }
    free(t);
}

void TST_free(TST* t, free_fn free_fn) {
    rec_free(t->root, free_fn);

    Node *curr_node = forward_list_get_head_node(t->iter);
    while (curr_node != NULL) {
        struct TST_iterator_node_string *ite = node_get_value(curr_node);
        free(ite->string);
        free(ite);

        curr_node = forward_list_goto_next(curr_node);
    }
    forward_list_destroy(t->iter);

    free(t);
}

// TST ITERATOR

// Definindo a estrutura do iterador
struct TST_iterator{
    Node *curr;
};

bool TST_iterator_has_next(TST_iterator *iterator) {
    return iterator->curr != NULL;
}

data_type TST_iterator_next(TST_iterator *iterator, char **out_key) {
    struct TST_iterator_node_string *last = node_get_value(iterator->curr);
    *out_key = last->string;
    iterator->curr = node_get_next(iterator->curr);
    return last->node->val;
}

// inicializar o iterador
TST_iterator* TST_iterator_init(TST *tst) {
    TST_iterator *iterator = malloc(sizeof(*iterator));

    iterator->curr = forward_list_get_head_node(tst->iter);

    return iterator;
}

// Função auxiliar para destruir o iterador
void TST_iterator_free(TST_iterator *iterator) {
    free(iterator);
}

void TST_traverse(TST *tst, void (*visit)(char *, data_type)) {
    TST_iterator *iterator = TST_iterator_init(tst);
    while (TST_iterator_has_next(iterator)) {
        char *key;
        data_type val = TST_iterator_next(iterator, &key);
        visit(key, val);
    }
    TST_iterator_free(iterator);
}

