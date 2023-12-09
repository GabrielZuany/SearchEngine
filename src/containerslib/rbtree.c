#include "containerslib/exceptions.h"
#include "containerslib/forward_list.h"

#include "containerslib/rbtree.h"

#include <stdio.h>

struct _RbNode
{
    void *key;
    void *val;
    bool isRed;
    struct _RbNode *left;
    struct _RbNode *right;
};

static struct _RbNode *__rbtree_node_init(void *key, void *val, bool isRed) {
    struct _RbNode *node = malloc(sizeof(*node));
    node->key = key;
    node->val = val;
    node->isRed = isRed;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static bool __rbtree_node_is_red(struct _RbNode *node) {
    if (node == NULL)
        return false;

    return node->isRed;
}

static struct _RbNode *__rbtree_node_rotate_left(struct _RbNode *node) {
    struct _RbNode *x = node->right;
    node->right = x->left;
    x->left = node;
    x->isRed = x->left->isRed;
    x->left->isRed = true;
    return x;
}

static struct _RbNode *__rbtree_node_rotate_right(struct _RbNode *node) {
    struct _RbNode *x = node->left;
    node->left = x->right;
    x->right = node;
    x->isRed = x->right->isRed;
    x->right->isRed = true;
    return x;
}

static void __rbtree_node_flip_colors(struct _RbNode *node) {
    node->isRed = true;
    node->left->isRed = false;
    node->right->isRed = false;
}

struct RbTree
{
    struct _RbNode *root;

    size_t size;

    cmp_fn key_cmp;
};

RbTree *rbtree_init(cmp_fn key_cmp) {
    RbTree *self = malloc(sizeof(*self));

    self->root = NULL;

    self->size = 0;

    self->key_cmp = key_cmp;

    return self;
}

static struct _RbNode *__rbtree_put_node(struct _RbNode *node, void *key, void *val, cmp_fn tree_key_cmp, size_t *tree_size, void **out_key, void **out_value) {
    // Insert at bottom and color it red.
    if (node == NULL) {
        (*tree_size)++;
        return __rbtree_node_init(key, val, true);
    }

    int cmp = tree_key_cmp(key, node->key);
    if (cmp < 0)
        node->left = __rbtree_put_node(node->left, key, val, tree_key_cmp, tree_size, out_key, out_value);
    else if (cmp > 0)
        node->right = __rbtree_put_node(node->right, key, val, tree_key_cmp, tree_size, out_key, out_value);
    else {
        *out_key = node->key;
        *out_value = node->val;
        node->key = key;
        node->val = val;
    }

    // Lean left.
    if (__rbtree_node_is_red(node->right) && !__rbtree_node_is_red(node->left))
        node = __rbtree_node_rotate_left(node);

    // Balance 4-node.
    if (__rbtree_node_is_red(node->left) && __rbtree_node_is_red(node->left->left))
        node = __rbtree_node_rotate_right(node);

    // Split 4-node.
    if (__rbtree_node_is_red(node->left) && __rbtree_node_is_red(node->right))
        __rbtree_node_flip_colors(node);

    return node;
}

void *rbtree_put(RbTree *self, void *key, void *val, void **out_key) {
    *out_key = NULL;
    void *out_val = NULL;

    self->root = __rbtree_put_node(self->root, key, val, self->key_cmp, &self->size, out_key, &out_val);
    self->root->isRed = false;

    return out_val;
}

void *rbtree_get(RbTree *self, void *key, void **out_key) {
    struct _RbNode *node = self->root;

    while (node != NULL) {
        int cmp = self->key_cmp(key, node->key);
        if (cmp < 0)
            node = node->left;
        else if (cmp > 0)
            node = node->right;
        else {
            *out_key = node->key;
            return node->val;
        }
    }

    *out_key = NULL;
    return NULL;
}

bool rbtree_contains(RbTree *self, void *key) {
    void *_;
    return rbtree_get(self, key, &_) != NULL;
}

void *rbtree_delete(RbTree *self, void *key, void **out_key) {
    if (self) {}; // warning suppression hihiih
    if (key) {}; // warning suppression hihiih
    if (out_key) {}; // warning suppression hihiih
    exception_throw_failure("rbtree_delete - Not implemented yet.");
    return NULL;
}

bool rbtree_empty(RbTree *self) {
    return self->size == 0;
}

size_t rbtree_size(RbTree *self) {
    return self->size;
}

void *rbtree_min(RbTree *self) {
    struct _RbNode *node = self->root;

    while (node->left != NULL)
        node = node->left;

    return node->key;
}

void *rbtree_max(RbTree *self) {
    struct _RbNode *node = self->root;

    while (node->right != NULL)
        node = node->right;

    return node->key;
}

static struct _RbNode *__rbtree_floor_node(struct _RbNode *node, void *key, cmp_fn tree_key_cmp) {
    if (node == NULL)
        return NULL;

    int cmp = tree_key_cmp(key, node->key);
    if (cmp == 0)
        return node;

    if (cmp < 0)
        return __rbtree_floor_node(node->left, key, tree_key_cmp);

    struct _RbNode *t = __rbtree_floor_node(node->right, key, tree_key_cmp);
    if (t != NULL)
        return t;
    else
        return node;
}

void *rbtree_floor(RbTree *self, void *key) {
    struct _RbNode *node = __rbtree_floor_node(self->root, key, self->key_cmp);

    if (node == NULL)
        return NULL;

    return node->key;
}

static struct _RbNode *__rbtree_ceiling_node(struct _RbNode *node, void *key, cmp_fn tree_key_cmp) {
    if (node == NULL)
        return NULL;

    int cmp = tree_key_cmp(key, node->key);
    if (cmp == 0)
        return node;

    if (cmp > 0)
        return __rbtree_ceiling_node(node->right, key, tree_key_cmp);

    struct _RbNode *t = __rbtree_ceiling_node(node->left, key, tree_key_cmp);
    if (t != NULL)
        return t;
    else
        return node;
}

void *rbtree_ceiling(RbTree *self, void *key) {
    struct _RbNode *node = __rbtree_ceiling_node(self->root, key, self->key_cmp);

    if (node == NULL)
        return NULL;

    return node->key;    
}

void *rbtree_delmin(RbTree *self, void **out_key) {
    if (rbtree_empty(self))
        exception_throw_failure("rbtree_delmin - Empty tree.");

    if (out_key) {}; // warning suppression hihiih

    exception_throw_failure("rbtree_delmin - Not implemented yet.");

    return NULL;
}

void *rbtree_delmax(RbTree *self, void **out_key) {
    if (rbtree_empty(self))
        exception_throw_failure("rbtree_delmax - Empty tree.");

    if (out_key) {}; // warning suppression hihiih

    exception_throw_failure("rbtree_delmax - Not implemented yet.");

    return NULL;
}

static void __rbtree_traverse_node(struct _RbNode *node, void (*visit)(void *,  void *)) {
    if (node == NULL)
        return;

    __rbtree_traverse_node(node->left, visit);
    visit(node->key, node->val);
    __rbtree_traverse_node(node->right, visit);
}

void rbtree_traverse(RbTree *self, void (*visit)(void *,  void *)) {
    __rbtree_traverse_node(self->root, visit);
}

static void __rbtree_finish_node(struct _RbNode *node) {
    if (node == NULL)
        return;

    __rbtree_finish_node(node->left);
    __rbtree_finish_node(node->right);
    free(node);
}

void rbtree_finish(RbTree *self) {
    __rbtree_finish_node(self->root);

    free(self);
}

// Definindo a estrutura do iterador
struct RbTreeIterator {
    RbTree *rbtree;
    ForwardList *stack;
    struct _RbNode *current;
};

static struct _RbNode *__find_min(struct _RbNode *node, ForwardList *stack) {
    if (node->left != NULL) {
        forward_list_push_front(stack, node->left);
        return __find_min(node->left, stack);
    }

    if(forward_list_is_empty(stack))
        return NULL;

    forward_list_pop_front(stack);
    return node;
}

// Função auxiliar para encontrar o próximo nó
// enquanto procura, preenche o buffer com os caracteres
// Ao mesmo tempo também mantem uma pilha de nós, para poder voltar recursivamente
static struct _RbNode *__find_next(RbTreeIterator *iterator) {
    struct _RbNode *curr = iterator->current;

    if (curr->right != NULL) {
        forward_list_push_front(iterator->stack, curr->right);
        return __find_min(curr->right, iterator->stack);
    }

    while (!forward_list_is_empty(iterator->stack)) {
        struct _RbNode *node = forward_list_pop_front(iterator->stack);

        if (node->right == curr) {
            curr = node;
            continue;
        }

        return node;
    }

    return NULL;
}

bool rbtree_iterator_has_next(RbTreeIterator *iterator) {
    return iterator->current != NULL;
}

void *rbtree_iterator_next(RbTreeIterator *iterator, void **out_key) {
    if (!rbtree_iterator_has_next(iterator))
        exception_throw_failure("rbtree_iterator_next - No next element.");

    *out_key = iterator->current->key;
    void *out_val = iterator->current->val;

    iterator->current = __find_next(iterator);

    return out_val;
}

// inicializar o iterador
RbTreeIterator* rbtree_iterator_init(RbTree *rbtree) {
    RbTreeIterator *iterator = malloc(sizeof(*iterator));

    iterator->rbtree = rbtree;
    iterator->stack = forward_list_construct();
    if(rbtree != NULL)
        iterator->current = __find_min(rbtree->root, iterator->stack);

    return iterator;
}

// Função auxiliar para destruir o iterador
void rbtree_iterator_free(RbTreeIterator *iterator) {
    forward_list_destroy(iterator->stack);
    free(iterator);
}
