#include <stdlib.h>
#include "include/containerslib/exceptions.h"
#include "include/containerslib/tst.h"

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

static node* create_node(node *parent) {
    node* n = malloc(sizeof(*n));
    n->val = NULL;
    n->c = '\0';
    n->l = NULL;
    n->m = NULL;
    n->r = NULL;
    n->p = parent;
    return n;
}

TST* TST_init() {
    TST* t = malloc(sizeof(*t));
    t->root = NULL;
    return t;
}

// imagino que quebre se a string for vazia, favor nao ousar
static node* rec_insert(TST *tst, node* t, char* key, data_type val, node* parent) {
    // TODO: checar se a logica do parent esta correta
    unsigned char c = *key;
    if (t == NULL) { t = create_node(parent); t->c = c;}
    if      (c < t->c) { t->l = rec_insert(tst, t->l, key, val, parent); }
    else if (c > t->c) { t->r = rec_insert(tst, t->r, key, val, parent); }
    else if (key[1] != '\0') {
        t->m = rec_insert(tst, t->m, ++key, val, t);
        tst->size++;
    } else { t->val = val; }
    return t;
}

key_value TST_insert(TST* t, char* key , data_type val) {
    node *n = rec_insert(t, t->root, key, val, NULL);

    key_value kv = {key, n->val};

    if (t->root == NULL) { t->root = n; }

    return kv;
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
    free(t);
}

void TST_free(TST* t) {
    rec_free(t->root);
    free(t);
}

// TST ITERATOR

// Definindo a estrutura do iterador
struct TSTIterator{
    TST *tst;
    char *buffer;
    node *current;
    int index;
};

// Função auxiliar para encontrar o primeiro nó
// enquanto procura, preenche o buffer com os caracteres
static node *find_first(node *t, TSTIterator *iterator) {
    if (t == NULL) {
        return NULL;
    }

    //busca o nó mais a esquerda para começar
    while (t->l != NULL) {
        t = t->l;
    }

    // Se o nó atual não for NULL, adicione o caractere ao buffer
    iterator->buffer[iterator->index++] = t->c;

    // Se o valor do nó atual não for NULL, retorne o nó atual
    if (t->val != NULL) {
        iterator->buffer[iterator->index] = '\0';
        return t;
    }

    // Se o nó atual não for NULL, mas o valor do nó atual for NULL, continue procurando
    if (t->l != NULL) {
        node *n = find_first(t->l, iterator);
        if (n != NULL) {
            return n;
        }
    }

    if (t->m != NULL) {
        node *n = find_first(t->m, iterator);
        if (n != NULL) {
            return n;
        }
    }

    if (t->r != NULL) {
        node *n = find_first(t->r, iterator);
        if (n != NULL) {
            return n;
        }
    }

    // Se o nó atual não for NULL, mas o valor do nó atual for NULL, remova o caractere do buffer
    iterator->index--;

    return NULL;
}

// Função auxiliar para encontrar o próximo nó
// enquanto procura, preenche o buffer com os caracteres
static node *find_next(node *t, TSTIterator *iterator) {
    // Assume que está em um nó valido e com o buffer preenchido

    // Continua procurando nos nós filhos para ver se encontra um nó com valor
    if (t->l != NULL) {
        node *n = find_first(t->l, iterator);
        if (n != NULL) {
            return n;
        }
    }

    if (t->m != NULL) {
        node *n = find_first(t->m, iterator);
        if (n != NULL) {
            return n;
        }
    }

    if (t->r != NULL) {
        node *n = find_first(t->r, iterator);
        if (n != NULL) {
            return n;
        }
    }

    // Se não encontrar, remove o caractere do buffer e começa a procurar no pai
    // ALERTA DE INEFICIENCIA: checa o filho todo de novo
    // ALERTA DE BUG: se o pai for NULL, vai dar segmentation fault
    // esse ultimo foi o copilot que falou, mas eu confio nele
    iterator->index--;
    return find_next(t->p, iterator);
}

int TSTIterator_has_next(TSTIterator *iterator) {
    return iterator->current != NULL;
}

key_value TSTIterator_next(TSTIterator *iterator) {
    if (iterator->current == NULL) {
        //throw("TSTIterator: next: no more elements");
    }

    key_value kv = {iterator->buffer, iterator->current->val};

    iterator->current = find_next(iterator->current, iterator);

    return kv;
}

// inicializar o iterador
TSTIterator* TST_iterator_init(TST *tst) {
    TSTIterator *iterator = malloc(sizeof(*iterator));
    iterator->tst = tst;
    iterator->buffer = malloc(256); // TODO: tamanho dinamico conforme necessidade
    iterator->index = 0;
    iterator->current = find_first(tst->root, iterator);

    return iterator;
}

// Função auxiliar para destruir o iterador
void TST_iterator_free(TSTIterator *iterator) {
    free(iterator->buffer);
    free(iterator);
}

void TST_traverse(TST *tst, void (*visit)(char *, data_type)) {
    TSTIterator* iterator = TST_iterator_init(tst);

    while (TSTIterator_has_next(iterator)) {
        key_value kv = TSTIterator_next(iterator);
        visit(kv.key, kv.val);
    }

    TST_iterator_free(iterator);
}