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
    struct node *l, *m, *r, *p;
};

struct TST {
    node* root;
    int size;
};

typedef struct{
    node *n;
    char c;
}stack_node;

static stack_node* stack_node_construct(node *n, char c){
    stack_node *sn = malloc(sizeof(*sn));
    sn->n = n;
    sn->c = c;
    return sn;
}

static node* create_node(node *p) {
    node* n = malloc(sizeof(*n));
    n->val = NULL;
    n->c = '\0';
    n->l = NULL;
    n->m = NULL;
    n->r = NULL;
    n->p = p;
    return n;
}

TST* TST_init() {
    TST* t = malloc(sizeof(*t));
    t->root = NULL;
    return t;
}

// imagino que quebre se a string for vazia, favor nao ousar
static node* rec_put(TST *tst, node* t, const char* key, data_type val, data_type *out_val, node *p) {
    const unsigned char c = *key;
    if (t == NULL) { t = create_node(p); t->c = c;}
    if      (c < t->c) { t->l = rec_put(tst, t->l, key, val, out_val, t); }
    else if (c > t->c) { t->r = rec_put(tst, t->r, key, val, out_val, t); }
    else if (key[1] != '\0') {
        t->m = rec_put(tst, t->m, ++key, val, out_val, t);
    }
    else {
        *out_val = t->val;
        t->val = val;
    }
    return t;
}

data_type TST_put(TST* t, const char* key , data_type val) {
    data_type out_val;
    node *n = rec_put(t, t->root, key, val, &out_val, NULL);

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
    node *current;

    char *buffer;
    size_t buf_index;
    size_t buf_capacity;
};

bool TST_iterator_has_next(TST_iterator *iterator) {
    return iterator->current != NULL;
}

data_type TST_iterator_next(TST_iterator *iterator, char **out_key) {
    if (iterator->current == NULL) return NULL;

    node *n = iterator->current;

    while (n->m != NULL) {
        forward_list_push_head(iterator->stack, stack_node_construct(n->m, n->c));
        n = n->m;
    }

    iterator->buf_index = 0;
    while (iterator->stack->len > 0) {
        stack_node *sn = forward_list_pop_head(iterator->stack);
        iterator->buffer[iterator->buf_index++] = sn->c;
        n = sn->n;
        free(sn);

        while (n->l != NULL) {
            forward_list_push_head(iterator->stack, stack_node_construct(n->l, '\0'));
            n = n->l;
        }

        if (n->val != NULL) {
            iterator->current = n;
            iterator->buffer[iterator->buf_index] = '\0';
            *out_key = iterator->buffer;
            return n->val;
        }
    }

    iterator->current = NULL;
    return NULL;
}

static __TST_iterator_find_leftest(node *n, TST_iterator *iterator) {
    if (n == NULL) return NULL;

    forward_list_push_front(iterator->stack, n);

    while (n->l != NULL) {
        forward_list_push_front(iterator->stack, n->l);
        n = n->l;
    }

    // TODO: parei aqui
    

    return n;
}

// inicializar o iterador
TST_iterator* TST_iterator_init(TST *tst) {
    TST_iterator *iterator = malloc(sizeof(*iterator));

    iterator->tst = tst;
    iterator->stack = forward_list_init();

    iterator->buf_capacity = 16;
    iterator->buf_index = 0;
    iterator->buffer = malloc(iterator->buf_capacity * sizeof(char));

    iterator->current = __TST_iterator_find_leftest(tst->root, iterator);

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

