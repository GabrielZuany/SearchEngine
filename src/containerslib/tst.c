#include <stdlib.h>
// #include "include/containerslib/exceptions.h"
// #include "include/containerslib/tst.h"
// #include "include/containerslib/types.h"
// #include "include/containerslib/forward_list.h"

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

static void rec_free(node* t) {
    if (t == NULL) { return; }
    rec_free(t->l);
    rec_free(t->m);
    rec_free(t->r);
    free(t->val);
    free(t);
}

void TST_free(TST* t) {
    rec_free(t->root);
    free(t);
}

// TST ITERATOR

// Definindo a estrutura do iterador
struct TST_iterator{
    TST *tst;
    ForwardList *stack;
    char *buffer;
    node *current;
    int index;
};

// Função auxiliar para encontrar o primeiro nó
// enquanto procura, preenche o buffer com os caracteres
// Ao mesmo tempo também mantem uma pilha de nós, para poder voltar recursivamente
static node *rec_find_first(node *n, TST_iterator *iterator){
    if (n == NULL) {
        return NULL;
    }

    // Se o nó atual tiver valor, retorna ele
    if (n->val != NULL) {
        // adiciona o '\0' no final do buffer
        iterator->buffer[iterator->index] = '\0';
        return n;
    }

    // Se não tiver valor, adiciona o caractere no buffer e continua procurando
    iterator->buffer[iterator->index++] = n->c;

    // Se o nó tiver filho a esquerda, adiciona ele na pilha e continua procurando
    if (n->l != NULL) {
        forward_list_push_front(iterator->stack, n->l);
    }

    // Se o nó tiver filho do meio, adiciona ele na pilha e continua procurando
    if (n->m != NULL) {
        forward_list_push_front(iterator->stack, n->m);
    }

    // Se o nó tiver filho a direita, adiciona ele na pilha e continua procurando
    if (n->r != NULL) {
        forward_list_push_front(iterator->stack, n->r);
    }

    // Se a pilha estiver vazia, retorna NULL
    if (forward_list_is_empty(iterator->stack)) {
        return NULL;
    }

    // Se não estiver vazia, pega o primeiro elemento da pilha e continua procurando
    node *next = forward_list_get_head_value(iterator->stack);
    forward_list_pop_front(iterator->stack);
    return rec_find_first(next, iterator);
}

static node *find_first(node *t, TST_iterator *iterator) {
    // Reinicia o buffer e a pilha
    iterator->index = 0;
    forward_list_destroy(iterator->stack);
    iterator->stack = forward_list_construct();

    // Começa a procurar
    return rec_find_first(t, iterator);
}

// Função auxiliar para encontrar o próximo nó
// enquanto procura, preenche o buffer com os caracteres
// Ao mesmo tempo também mantem uma pilha de nós, para poder voltar recursivamente
static node *find_next(node *t, TST_iterator *iterator) {
    // Se o nó atual tiver filho do meio, adiciona ele na pilha e continua procurando
    if (t->m != NULL) {
        forward_list_push_front(iterator->stack, t->m);
    }

    // Se o nó atual tiver filho a direita, adiciona ele na pilha e continua procurando
    if (t->r != NULL) {
        forward_list_push_front(iterator->stack, t->r);
    }

    // Se a pilha estiver vazia, retorna NULL
    if (forward_list_is_empty(iterator->stack)) {
        return NULL;
    }

    // Se não estiver vazia, pega o primeiro elemento da pilha e continua procurando
    node *next = forward_list_get_head_value(iterator->stack);
    forward_list_pop_front(iterator->stack);
    return find_next(next, iterator);
}

bool TST_iterator_has_next(TST_iterator *iterator) {
    return iterator->current != NULL;
}

data_type TST_iterator_next(TST_iterator *iterator, char **out_key) {
    if (iterator->current == NULL) {
        //throw("TSTIterator: next: no more elements");
    }

    *out_key = iterator->buffer;
    data_type *out_val = iterator->current->val;

    iterator->current = find_next(iterator->current, iterator);

    return out_val;
}

// inicializar o iterador
TST_iterator* TST_iterator_init(TST *tst) {
    TST_iterator *iterator = malloc(sizeof(*iterator));
    iterator->tst = tst;
    iterator->buffer = malloc(256); // TODO: tamanho dinamico conforme necessidade
    // ^^^^
    // jheam disse que isso aqui é criminoso e não concorda com suas atitudes
    iterator->index = 0;
    iterator->current = find_first(tst->root, iterator);

    return iterator;
}

// Função auxiliar para destruir o iterador
void TST_iterator_free(TST_iterator *iterator) {
    free(iterator->buffer);
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
