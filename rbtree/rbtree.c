#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"

#define rb_parent(r)        ((r)->parent)
#define rb_color(r)         ((r)->color)
#define rb_is_red(r)        ((r)->color == RB_COLOR_RED)
#define rb_is_black(r)      ((r)->color == RB_COLOR_BLACK)
#define rb_set_black(r)     do { (r)->color = RB_COLOR_BLACK; } while (0)
#define rb_set_red(r)       do { (r)->color = RB_COLOR_RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)   do { (r)->color = (c); } while (0)

/*
 * 创建红黑树，返回"红黑树的根"！
 */
rbroot_t *create_rbtree(void)
{
    rbroot_t *root;

    root = (rbroot_t *)malloc(sizeof(rbroot_t));
    if (!root) {
        //DHCP_SYSLOG(LOG_LEVEL_ERROR, "create rbtree, no memory");
        return NULL;
    }

    root->rbnode = NULL;

    return root;
}

/*
 * 前序遍历"红黑树"
 */
static void preorder(rbnode_t *rbnode)
{
    if (rbnode != NULL) {
        printf("%u ", rbnode->key);
        preorder(rbnode->left);
        preorder(rbnode->right);
    }
}

void preorder_rbtree(rbroot_t *root)
{
    preorder(root->rbnode);
}

/*
 * 中序遍历"红黑树"
 */
static void inorder(rbnode_t *rbnode)
{
    if (rbnode != NULL) {
        inorder(rbnode->left);
        printf("%u ", rbnode->key);
        inorder(rbnode->right);
    }
}

void inorder_rbtree(rbroot_t *root)
{
    inorder(root->rbnode);
}

/*
 * 后序遍历"红黑树"
 */
static void postorder(rbnode_t *rbnode)
{
    if (rbnode != NULL) {
        postorder(rbnode->left);
        postorder(rbnode->right);
        printf("%u ", rbnode->key);
    }
}

void postorder_rbtree(rbroot_t *root)
{
    postorder(root->rbnode);
}

/*
 * (递归实现)查找"红黑树x"中键值为key的节点
 */
rbnode_t *iterative_rbtree_search(rbnode_t *rbnode, uint32_t key)
{
    if (rbnode == NULL || rbnode->key == key)
        return rbnode;

    if (key < rbnode->key)
        return iterative_rbtree_search(rbnode->left, key);
    else
        return iterative_rbtree_search(rbnode->right, key);
}

/*
 * (非递归实现)查找"红黑树x"中键值为key的节点
 */
rbnode_t *rbtree_search(rbnode_t *rbnode, uint32_t key)
{
    while ((rbnode != NULL) && (rbnode->key != key)) {
        if (key < rbnode->key)
            rbnode = rbnode->left;
        else
            rbnode = rbnode->right;
    }

    return rbnode;
}

/*
 * 查找最小结点：返回tree为根结点的红黑树的最小结点。
 */
static rbnode_t *rbmin(rbnode_t *rbnode)
{
    if (rbnode == NULL)
        return NULL;

    while (rbnode->left != NULL) {
        rbnode = rbnode->left;
    }

    return rbnode;
}

rbnode_t *rbtree_min(rbroot_t *root)
{
    return rbmin(root->rbnode);
}

/*
 * 查找最大结点：返回tree为根结点的红黑树的最大结点。
 */
rbnode_t *rbmax(rbnode_t *rbnode)
{
    if (rbnode == NULL)
        return NULL;

    while (rbnode->right != NULL) {
        rbnode = rbnode->right;
    }

    return rbnode;
}

rbnode_t *rbtree_max(rbroot_t *root)
{
    return rbmax(root->rbnode);
}

/*
 * 找结点(rbnode)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
 */
rbnode_t *rbnode_next(rbnode_t *rbnode)
{
    rbnode_t *next;

    // 如果rbnode存在右孩子，则"rbnode的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (rbnode->right != NULL)
        return rbmin(rbnode->right);

    /* 如果rbnode没有右孩子。则rbnode有以下两种可能：
     * (01) rbnode是"一个左孩子"，则"rbnode的后继结点"为 "它的父结点"。
     * (02) rbnode是"一个右孩子"，则查找"rbnode的最低的父结点，并且该父结点要具有左孩子"，
     * 找到的这个"最低的父结点"就是"rbnode的后继结点"。
     */
    next = rbnode->parent;
    while ((next != NULL) && (rbnode == next->right)) {
        rbnode = next;
        next   = next->parent;
    }

    return next;
}

/*
 * 找结点(rbnode)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
 */
rbnode_t *rbnode_prev(rbnode_t *rbnode)
{
    rbnode_t *prev;

    // 如果rbnode存在左孩子，则"rbnode的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (rbnode->left != NULL)
        return rbmax(rbnode->left);

    /* 如果x没有左孩子。则rbnode有以下两种可能：
     * (01) rbnode是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
     * (01) rbnode是"一个左孩子"，则查找"rbnode的最低的父结点，并且该父结点要具有右孩子"，
     * 找到的这个"最低的父结点"就是"x的前驱结点"。
     */
    prev = rbnode->parent;
    while ((prev != NULL) && (rbnode == prev->left)) {
        rbnode = prev;
        prev   = prev->parent;
    }

    return prev;
}

/*
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 *
 */
static void rbtree_left_rotate(rbroot_t *root, rbnode_t *x)
{
    // 设置x的右孩子为y
    rbnode_t *y = x->right;

    // 将 “y的左孩子” 设为 “x的右孩子”；
    // 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // 将 “x的父亲” 设为 “y的父亲”
    y->parent = x->parent;

    if (x->parent == NULL) {
        root->rbnode = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
    } else {
        if (x->parent->left == x)
            x->parent->left = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
    }

    // 将 “x” 设为 “y的左孩子”
    y->left = x;
    // 将 “x的父节点” 设为 “y”
    x->parent = y;
}

/*
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */
static void rbtree_right_rotate(rbroot_t *root, rbnode_t *y)
{
    // 设置x是当前节点的左孩子。
    rbnode_t *x = y->left;

    // 将 “x的右孩子” 设为 “y的左孩子”；
    // 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // 将 “y的父亲” 设为 “x的父亲”
    x->parent = y->parent;

    if (y->parent == NULL) {
        root->rbnode = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
    } else {
        if (y == y->parent->right)
            y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->left = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }

    // 将 “y” 设为 “x的右孩子”
    x->right = y;

    // 将 “y的父节点” 设为 “x”
    y->parent = x;
}

/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */
static void rbtree_insert_fixup(rbroot_t *root, rbnode_t *node)
{
    rbnode_t *parent, *gparent;
    rbnode_t *uncle, *tmp;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = rb_parent(node)) && rb_is_red(parent)) {
        gparent = rb_parent(parent);

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left) {
            // Case 1条件：叔叔节点是红色
            uncle = gparent->right;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }

            // Case 2条件：叔叔是黑色，且当前节点是右孩子
            if (parent->right == node) {
                rbtree_left_rotate(root, parent);
                tmp    = parent;
                parent = node;
                node   = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        } else { //若“z的父节点”是“z的祖父节点的右孩子”
            // Case 1条件：叔叔节点是红色
            uncle = gparent->left;
            if (uncle && rb_is_red(uncle)) {
                rb_set_black(uncle);
                rb_set_black(parent);
                rb_set_red(gparent);
                node = gparent;
                continue;
            }

            // Case 2条件：叔叔是黑色，且当前节点是左孩子
            if (parent->left == node) {
                rbtree_right_rotate(root, parent);
                tmp    = parent;
                parent = node;
                node   = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是右孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root->rbnode);
}

/*
 * 添加节点：将节点(node)插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */
static void rbtree_insert(rbroot_t *root, rbnode_t *node)
{
    rbnode_t *y = NULL;
    rbnode_t *x = root->rbnode;

    // 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    while (x != NULL) {
        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    rb_parent(node) = y;

    if (y != NULL) {
        if (node->key < y->key) {
            // 情况2：若“node所包含的值” < “y所包含的值”，则将node设为“y的左孩子”
            y->left = node;
        } else {
            // 情况3：(“node所包含的值” >= “y所包含的值”)将node设为“y的右孩子”
            y->right = node;
        }
    } else {
        root->rbnode = node;    // 情况1：若y是空节点，则将node设为根
    }

    // 2. 设置节点的颜色为红色
    node->color = RB_COLOR_RED;

    // 3. 将它重新修正为一颗二叉查找树
    rbtree_insert_fixup(root, node);
}

/*
 * 创建结点
 *
 * 参数说明：
 *     key 是键值。
 */
static rbnode_t *rbnode_init(uint32_t key)
{
    rbnode_t *rbnode;

    rbnode = (rbnode_t *)malloc(sizeof(rbnode_t));
    if (!rbnode) {
        //DHCP_SYSLOG(LOG_LEVEL_ERROR, "rbnode init, no memory");
        return NULL;
    }

    rbnode->key    = key;
    rbnode->left   = NULL;
    rbnode->right  = NULL;
    rbnode->parent = NULL;
    rbnode->color  = RB_COLOR_BLACK;

    INIT_LIST_HEAD(&rbnode->content);

    return rbnode;
}

/*
 * 新建结点(节点键值为key)，并将其插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根
 *     key 插入结点的键值
 */
rbnode_t *add_rbnode(rbroot_t *root, uint32_t key)
{
    rbnode_t *rbnode;

    rbnode = rbtree_search(root->rbnode, key);
    if (rbnode)
        return rbnode;

    // 如果新建结点失败，则返回。
    rbnode = rbnode_init(key);
    if (rbnode == NULL)
        return NULL;

    rbtree_insert(root, rbnode);

    return rbnode;
}

/*
 * 红黑树删除修正函数
 *
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 待修正的节点
 */
static void rbtree_delete_fixup(rbroot_t *root, rbnode_t *node, rbnode_t *parent)
{
    rbnode_t *other;

    while ((!node || rb_is_black(node)) && node != root->rbnode) {
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->right || rb_is_black(other->right)) {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->rbnode;
                break;
            }
        } else {
            other = parent->left;
            if (rb_is_red(other)) {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            } else {
                if (!other->left || rb_is_black(other->left)) {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->rbnode;
                break;
            }
        }
    }

    if (node)
        rb_set_black(node);
}

/*
 * 删除结点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 *     node 删除的结点
 */
void rbtree_delete(rbroot_t *root, rbnode_t *node)
{
    rbnode_t *child, *parent, *replace;
    uint8_t color;

    // 被删除节点的"左右孩子都不为空"的情况。
    if ((node->left != NULL) && (node->right != NULL)) {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node)) {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } else
            // "node节点"是根节点，更新根节点。
            root->rbnode = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代节点"的颜色
        color = rb_color(replace);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node) {
            parent = replace;
        } else {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent    = node->parent;
        replace->color     = node->color;
        replace->left      = node->left;
        node->left->parent = replace;

        if (color == RB_COLOR_BLACK)
            rbtree_delete_fixup(root, child, parent);
        free(node);

        return;
    }

    if (node->left != NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    // 保存"取代节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点
    if (parent) {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    } else
        root->rbnode = child;

    if (color == RB_COLOR_BLACK)
        rbtree_delete_fixup(root, child, parent);
    free(node);
}

/*
 * 删除键值为key的结点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 *     key 键值
 */
void delete_rbnode(rbroot_t *root, uint32_t key)
{
    rbnode_t *rbnode;

    rbnode = rbtree_search(root->rbnode, key);
    if (rbnode)
        rbtree_delete(root, rbnode);
}

/*
 * 销毁红黑树
 */
static void rbnode_destroy(rbnode_t *rbnode)
{
    if (rbnode == NULL)
        return ;

    if (rbnode->left != NULL)
        rbnode_destroy(rbnode->left);

    if (rbnode->right != NULL)
        rbnode_destroy(rbnode->right);

    free(rbnode);
}

void destroy_rbtree(rbroot_t *root)
{
    rbnode_destroy(root->rbnode);
}

/*
 * 打印"红黑树"
 *
 * rbnode     -- 红黑树的节点
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */
static void rbnode_print(rbnode_t *rbnode, uint32_t key, int direction)
{
    if (rbnode != NULL) {
        if (direction == 0) {
            printf("%u(B) is root\n", rbnode->key);
        } else {
            printf("%u(%s) is %u's %s child\n",
                   rbnode->key,
                   rb_is_red(rbnode) ? "R" : "B",
                   key,
                   direction == 1 ? "right" : "left");
        }

        rbnode_print(rbnode->left, rbnode->key, -1);
        rbnode_print(rbnode->right, rbnode->key,  1);
    }
}

void print_rbtree(rbroot_t *root)
{
    if (root->rbnode) {
        rbnode_print(root->rbnode, root->rbnode->key, 0);
    }
}
