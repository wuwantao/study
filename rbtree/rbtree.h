#ifndef __RBTREE_H_
#define __RBTREE_H_

#include <stdint.h>
#include "list.h"

enum {
    RB_COLOR_RED   = 0,
    RB_COLOR_BLACK = 1
};

typedef struct rbnode {
    uint8_t  color;
    uint32_t key;
    struct list_head content;
    struct rbnode *left;
    struct rbnode *right;
    struct rbnode *parent;
} rbnode_t;

typedef struct {
    rbnode_t *rbnode;
} rbroot_t;

void destroy_rbtree(rbroot_t *root);
void delete_rbnode(rbroot_t *root, uint32_t key);
void rbtree_delete(rbroot_t *root, rbnode_t *node);
void preorder_rbtree(rbroot_t *root);
void inorder_rbtree(rbroot_t *root);
void postorder_rbtree(rbroot_t *root);
rbnode_t *rbtree_search(rbnode_t *rbnode, uint32_t key);
rbnode_t *iterative_rbtree_search(rbnode_t *rbnode, uint32_t key);
rbnode_t *rbtree_min(rbroot_t *root);
rbnode_t *rbtree_max(rbroot_t *root);
rbnode_t *add_rbnode(rbroot_t *root, uint32_t key);
rbnode_t *rbnode_next(rbnode_t *rbnode);
rbnode_t *rbnode_prev(rbnode_t *rbnode);
void print_rbtree(rbroot_t *root);

#endif

