#include "containerslib/exceptions.h"

#include "containerslib/rbtree.h"

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

static struct _RbNode *__rbtree_put_node(struct _RbNode *node, void *key, void *val, cmp_fn tree_key_cmp, size_t *tree_size) {
    // Insert at bottom and color it red.
    if (node == NULL) {
        (*tree_size)++;
        return __rbtree_node_init(key, val, true);
    }

    int cmp = tree_key_cmp(key, node->key);
    if (cmp < 0)
        node->left = __rbtree_put_node(node->left, key, val, tree_key_cmp, tree_size);
    else if (cmp > 0)
        node->right = __rbtree_put_node(node->right, key, val, tree_key_cmp, tree_size);
    else
        node->val = val;

    // Lean left.
    if (__rbtree_node_is_red(node->right) && !__rbtree_node_is_red(node->left))
        node = __rbtree_node_rotate_left(node);

    // Balance 4-node.
    if (__rbtree_node_is_red(node->left) && __rbtree_node_is_red(node->left->left))
        node = rotate_rignodet(node);

    // Split 4-node.
    if (__rbtree_node_is_red(node->left) && __rbtree_node_is_red(node->right))
        __rbtree_node_flip_colors(node);

    return node;
}

void rbtree_put(RbTree *self, void *key, void *val) {
    self->root = __rbtree_put_node(self->root, key, val, self->key_cmp, &self->size);
    self->root->isRed = false;
}

void *rbtree_get(RbTree *self, void *key) {
    struct _RbNode *node = self->root;

    while (node != NULL) {
        int cmp = self->key_cmp(key, node->key);
        if (cmp < 0)
            node = node->left;
        else if (cmp > 0)
            node = node->right;
        else
            return node->val;
    }

    return NULL;
}

bool rbtree_contains(RbTree *self, void *key) {
    return rbtree_get(self, key) != NULL;
}

void *rbtree_delete(RbTree *self, void **ref_key) {
    exception_throw_failure("rbtree_delete - Not implemented yet.");
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
    exception_throw_failure("rbtree_delmin - Not implemented yet.");
}

void *rbtree_delmax(RbTree *self, void **out_key) {
    exception_throw_failure("rbtree_delmax - Not implemented yet.");
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
