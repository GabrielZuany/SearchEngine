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
    // if (t->val != NULL) { free_fn(t->val); }
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
    ForwardList *stack;
    char *buffer;
    node *current;
    int index;
};

// Função auxiliar para encontrar o próximo nó
// enquanto procura, preenche o buffer com os caracteres
// Ao mesmo tempo também mantem uma pilha de nós, para poder voltar recursivamente
static node *find_next(TST_iterator *iterator, stack_node *sn) {
    // libera o stack_node
    node *t = sn->n;
    char sn_char = sn->c;
    free(sn);

    // confere se ha algum char novo para ser inserido
    if(sn_char != '\0'){
        iterator->buffer[iterator->index++] = sn_char;
    }

    if (t == NULL) {
        return NULL;
    }

    // Se o nó atual tiver valor, retorna ele
    if (t->val != NULL) {
        // adiciona o caracter no buffer junto do '\0'
        iterator->buffer[iterator->index++] = t->c;
        iterator->buffer[iterator->index] = '\0';
        return t;
    }


    // Se o nó tiver filho a direita, adiciona ele na pilha e continua procurando
    if (t->r != NULL) {
        forward_list_push_front(iterator->stack, stack_node_construct(t->r, '\0'));
    }
    
    // Se o nó tiver filho do meio, adiciona ele na pilha e continua procurando
    if (t->m != NULL) {
        stack_node *sn = stack_node_construct(t->m, t->c);
        forward_list_push_front(iterator->stack, sn);
    }
    
    // Se o nó tiver filho a esquerda, adiciona ele na pilha e continua procurando
    if (t->l != NULL) {
        forward_list_push_front(iterator->stack, stack_node_construct(t->l, '\0'));
    }

    // Se a pilha estiver vazia, retorna NULL
    if (forward_list_is_empty(iterator->stack)) {
        return NULL;
    }

    // Se não estiver vazia, pega o primeiro elemento da pilha e continua procurando
    stack_node *next = forward_list_get_head_value(iterator->stack);
    forward_list_pop_front(iterator->stack);

    return find_next(iterator, next);
}

bool TST_iterator_has_next(TST_iterator *iterator) {
    return iterator->current != NULL;
}

data_type TST_iterator_next(TST_iterator *iterator, char **out_key) {
    if (iterator->current == NULL) {
        //throw("TSTIterator: next: no more elements");
    }

    *out_key = strdup(iterator->buffer);
    data_type out_val = iterator->current->val;

    // confere se a palavra atual é uma substring de outra palavra
    if(iterator->current->m != NULL){
        node *out = find_next(iterator, stack_node_construct(iterator->current->m, '\0'));
        if(out != NULL){
            iterator->current = out;
            return out_val;
        }
    }

    // confere se a lista esta vazia
    if(forward_list_is_empty(iterator->stack)){
        iterator->current = NULL;
        return out_val;
    }

    // ve qual o proximo node a ser visitado
    stack_node *next = forward_list_get_head_value(iterator->stack);
    forward_list_pop_front(iterator->stack);

    node *current = iterator->current;

    // vai retirando os caracteres do buffer ate chegar no node que tem o proximo node a ser visitado
    while(current != NULL){
        if(current->r == next->n || current->l == next->n || current->m == next->n){
            if(current->r == next->n || current->l == next->n){
                iterator->buffer[--iterator->index] = '\0';
            }
            iterator->current = next->n;
            break;
        }

        iterator->buffer[--iterator->index] = '\0';
        current = current->p;
    }

    iterator->current = find_next(iterator, next);

    return out_val;
}

static node *find_first(node *t, TST_iterator *iterator) {
    // Reinicia o buffer e a pilha
    iterator->index = 0;
    iterator->stack = forward_list_construct();

    // Começa a procurar
    return find_next(iterator, stack_node_construct(t, '\0'));
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

