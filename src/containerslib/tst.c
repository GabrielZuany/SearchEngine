#include <stdlib.h>
#include "containerslib/exceptions.h"
#include "containerslib/tst.h"

typedef struct node node;

struct node {
    data_type val;
    unsigned char c;
    TST *l, *m, *r;
};

struct TST {
    node* root;
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
static node* rec_insert(node* t, char* key, data_type val) {
    unsigned char c = *key;
    if (t == NULL) { t = create_node(); t->c = c;}
    if      (c < t->c) { t->l = rec_insert(t->l, key, val); }
    else if (c > t->c) { t->r = rec_insert(t->r, key, val); }
    else if (key[1] != '\0') {
        t->m = rec_insert(t->m, key++, val);
    } else { t->val = val; }
    return t;
}

TST* TST_insert(TST* t, char* key , data_type val) {
    return rec_insert(t, key, val);
}

// favor nao buscar strings vazias
static node* rec_search(node* t, char* key) {
    if (t == NULL) { return NULL; }
    unsigned char c = *key;
    if      (c < t->c) { return rec_search(t->l, key); }
    else if (c > t->c) { return rec_search(t->r, key); }
    else if (key[1] != '\0') {
        return rec_search(t->m, key++);
    } else { return t; }
}

data_type TST_search(TST* t, char* key) {
    node *n = rec_search(t->root, key);
    if (n == NULL)  { return NULL; } // se for modificar o tipo de retorno, favor mudar aqui
    else            { return n->val; }
}

void TST_free(TST* t) {
    // TODO
}