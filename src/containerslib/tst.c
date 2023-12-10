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

struct TST {
    node* root;
    int size;
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
    return t;
}

// imagino que quebre se a string for vazia, favor nao ousar
static node* rec_put(TST *tst, node* t, const char* key, data_type val, data_type *out_val) {
    const unsigned char c = *key;
    if (t == NULL) { t = create_node(); t->c = c;}
    if      (c < t->c) { t->l = rec_put(tst, t->l, key, val, out_val); }
    else if (c > t->c) { t->r = rec_put(tst, t->r, key, val, out_val); }
    else if (key[1] != '\0') {
        t->m = rec_put(tst, t->m, ++key, val, out_val);
    }
    else {
        *out_val = t->val;
        t->val = val;
    }
    return t;
}

data_type TST_put(TST* t, const char* key , data_type val) {
    data_type out_val;
    node *n = rec_put(t, t->root, key, val, &out_val);

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
    free(t);
}

// TST ITERATOR

// Definindo a estrutura do iterador
struct TST_iterator{
    TST *tst;
    ForwardList *stack; // List<Node>

    char *buffer;
    size_t buf_index;
    size_t buf_capacity;
};

static void __TST_iterator_buffer_append(TST_iterator *iterator, char c) {
    if (iterator->buf_index == iterator->buf_capacity) {
        iterator->buf_capacity *= 2;
        iterator->buffer = realloc(iterator->buffer, iterator->buf_capacity * sizeof(char));
    }

    iterator->buffer[iterator->buf_index++] = c;
    iterator->buffer[iterator->buf_index] = '\0';
}

static void __TST_iterator_buffer_pop(TST_iterator *iterator) {
    iterator->buffer[--iterator->buf_index] = '\0';
}

static void __TST_iterator_find_leftest(TST_iterator *iterator, node *n) {
    if (n == NULL) return;

    forward_list_push_front(iterator->stack, n);

    node *curr = n;
    while (curr->l != NULL) {
        if (curr->r != NULL)
            forward_list_push_front(iterator->stack, curr->r);

        if (curr->m != NULL)
            forward_list_push_front(iterator->stack, curr->m);

        forward_list_push_front(iterator->stack, curr->l);

        curr = curr->l;
    }

    if (curr->r != NULL)
        forward_list_push_front(iterator->stack, curr->r);

    __TST_iterator_buffer_append(iterator, curr->c);
    // como essa TST nao tem delete, seguramente há um nó no meio, ou curr e um no terminal
    __TST_iterator_find_leftest(iterator, curr->m);
}

bool TST_iterator_has_next(TST_iterator *iterator) {
    return forward_list_size(iterator->stack) > 0;
}

data_type TST_iterator_next(TST_iterator *iterator, char **out_key) {
    node *last = forward_list_pop_front(iterator->stack);
    *out_key = strdup(iterator->buffer);
    __TST_iterator_buffer_pop(iterator);

    while (forward_list_size(iterator->stack) > 0) {
        node *nhead = forward_list_get_head_value(iterator->stack);
        __TST_iterator_buffer_pop(iterator);

        if (nhead->m != last && (nhead->l == last || nhead->r == last)) { // de um filho lateral para o pai
            __TST_iterator_buffer_append(iterator, nhead->c);
        }
        else if (nhead->m != last && nhead->l != last && nhead->r != last) { // de um filho lateral para o irmao
            forward_list_pop_front(iterator->stack);

            node *nnhead = forward_list_get_head_value(iterator->stack);
            node *nnnhead = forward_list_get(iterator->stack, 1);

            if (nhead == nnhead->m)                                // se for o irmao do meio
                __TST_iterator_buffer_append(iterator, nnhead->c);
            else if (nhead == nnnhead->m)                          // se for o irmao do meio, mas o proximo era o irmao da direita
                __TST_iterator_buffer_append(iterator, nnnhead->c);
            else if (last == nnhead->m)                            // se for o irmao da direita e o anterior era o irmao do meio
                __TST_iterator_buffer_pop(iterator);

            __TST_iterator_find_leftest(iterator, nhead);
            break;
        }

        if (nhead->val != NULL) {
            if (nhead->m == last || (nhead->l != last && nhead->r != last)) // de morgan hii
                __TST_iterator_buffer_append(iterator, nhead->c);

            break;
        }

        last = forward_list_pop_front(iterator->stack);
    }

    return last->val;
}

// inicializar o iterador
TST_iterator* TST_iterator_init(TST *tst) {
    TST_iterator *iterator = malloc(sizeof(*iterator));

    iterator->tst = tst;
    iterator->stack = forward_list_construct();

    iterator->buf_capacity = 16;
    iterator->buf_index = 0;
    iterator->buffer = malloc(iterator->buf_capacity * sizeof(char));

    __TST_iterator_find_leftest(iterator, tst->root);

    return iterator;
}

// Função auxiliar para destruir o iterador
void TST_iterator_free(TST_iterator *iterator) {
    free(iterator->buffer);
    forward_list_destroy(iterator->stack);

    free(iterator);
}

void TST_traverse(TST *tst, void (*visit)(char *, data_type)) {
    TST_iterator *iterator = TST_iterator_init(tst);
    while (TST_iterator_has_next(iterator)) {
        char *key;
        data_type val = TST_iterator_next(iterator, &key);
        visit(key, val);
        free(key);
    }
    TST_iterator_free(iterator);
}

