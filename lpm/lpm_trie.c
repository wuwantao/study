/*
 * Longest prefix match list implementation
 *
 * Copyright (c) 2016,2017 Daniel Mack
 * Copyright (c) 2016 David Herrmann
 *
 * This file is subject to the terms and conditions of version 2 of the GNU
 * General Public License.  See the file COPYING in the main directory of the
 * Linux distribution for more details.
 */


#include "lpm_trie.h"


#define BIT(nr)			(1UL << (nr))

#define min(a, b) ((a) < (b) ? (a) : (b))

/* Intermediate node */
#define LPM_TREE_NODE_FLAG_IM BIT(0)

void *yr_malloc(uint64_t size)
{
    return malloc(size);
}

void yr_free(void *p)
{
    free(p);
}

void *yr_memcpy(void *dst, const void *src, uint64_t len)
{
    return memcpy(dst, src, len);
}


/**
 * fls - find last set bit in word
 * @x: the word to search
 *
 * This is defined in a similar way as the libc and compiler builtin
 * ffs, but returns the position of the most significant set bit.
 *
 * fls(value) returns 0 if value is 0 or the position of the last
 * set bit if value is nonzero. The last (most significant) bit is
 * at position 32.
 */
//作用是：返回输入参数的最高有效bit位（从低位往左数最后的有效bit位）的序号，该序号与常规0起始序号不同，它是1起始的（当没有有效位时返回0）。
static inline int fls(int x)
{
    int r;
    
#ifdef CONFIG_X86_64
    /*
     * AMD64 says BSRL won't clobber the dest reg if x==0; Intel64 says the
     * dest reg is undefined if x==0, but their CPU architect says its
     * value is written to set it to the same as before, except that the
     * top 32 bits will be cleared.
     *
     * We cannot do this on 32 bits because at the very least some
     * 486 CPUs did not behave this way.
     */
    asm("bsrl %1,%0"
        : "=r" (r)
        : "rm" (x), "0" (-1));
#elif defined(CONFIG_X86_CMOV)
    asm("bsrl %1,%0\n\t"
        "cmovzl %2,%0"
        : "=&r" (r) : "rm" (x), "rm" (-1));
#else
    asm("bsrl %1,%0\n\t"
        "jnz 1f\n\t"
        "movl $-1,%0\n"
        "1:" : "=r" (r) : "rm" (x));
#endif
    return r + 1;
}


/* This trie implements a longest prefix match algorithm that can be used to
 * match IP addresses to a stored set of ranges.
 *
 * Data stored in @data of struct bpf_lpm_key and lpm_trie_node_t is
 * interpreted as big endian, so data[0] stores the most significant byte.
 *
 * Match ranges are internally stored in instances of lpm_trie_node_t
 * which each contain their prefix length as well as two pointers that may
 * lead to more nodes containing more specific matches. Each node also stores
 * a value that is defined by and returned to userspace via the update_elem
 * and lookup functions.
 *
 * For instance, let's start with a trie that was created with a prefix length
 * of 32, so it can be used for IPv4 addresses, and one single element that
 * matches 192.168.0.0/16. The data array would hence contain
 * [0xc0, 0xa8, 0x00, 0x00] in big-endian notation. This documentation will
 * stick to IP-address notation for readability though.
 *
 * As the trie is empty initially, the new node (1) will be places as root
 * node, denoted as (R) in the example below. As there are no other node, both
 * child pointers are %NULL.
 *
 *              +----------------+
 *              |       (1)  (R) |
 *              | 192.168.0.0/16 |
 *              |    value: 1    |
 *              |   [0]    [1]   |
 *              +----------------+
 *
 * Next, let's add a new node (2) matching 192.168.0.0/24. As there is already
 * a node with the same data and a smaller prefix (ie, a less specific one),
 * node (2) will become a child of (1). In child index depends on the next bit
 * that is outside of what (1) matches, and that bit is 0, so (2) will be
 * child[0] of (1):
 *
 *              +----------------+
 *              |       (1)  (R) |
 *              | 192.168.0.0/16 |
 *              |    value: 1    |
 *              |   [0]    [1]   |
 *              +----------------+
 *                   |
 *    +----------------+
 *    |       (2)      |
 *    | 192.168.0.0/24 |
 *    |    value: 2    |
 *    |   [0]    [1]   |
 *    +----------------+
 *
 * The child[1] slot of (1) could be filled with another node which has bit #17
 * (the next bit after the ones that (1) matches on) set to 1. For instance,
 * 192.168.128.0/24:
 *
 *              +----------------+
 *              |       (1)  (R) |
 *              | 192.168.0.0/16 |
 *              |    value: 1    |
 *              |   [0]    [1]   |
 *              +----------------+
 *                   |      |
 *    +----------------+  +------------------+
 *    |       (2)      |  |        (3)       |
 *    | 192.168.0.0/24 |  | 192.168.128.0/24 |
 *    |    value: 2    |  |     value: 3     |
 *    |   [0]    [1]   |  |    [0]    [1]    |
 *    +----------------+  +------------------+
 *
 * Let's add another node (4) to the game for 192.168.1.0/24. In order to place
 * it, node (1) is looked at first, and because (4) of the semantics laid out
 * above (bit #17 is 0), it would normally be attached to (1) as child[0].
 * However, that slot is already allocated, so a new node is needed in between.
 * That node does not have a value attached to it and it will never be
 * returned to users as result of a lookup. It is only there to differentiate
 * the traversal further. It will get a prefix as wide as necessary to
 * distinguish its two children:
 *
 *                      +----------------+
 *                      |       (1)  (R) |
 *                      | 192.168.0.0/16 |
 *                      |    value: 1    |
 *                      |   [0]    [1]   |
 *                      +----------------+
 *                           |      |
 *            +----------------+  +------------------+
 *            |       (4)  (I) |  |        (3)       |
 *            | 192.168.0.0/23 |  | 192.168.128.0/24 |
 *            |    value: ---  |  |     value: 3     |
 *            |   [0]    [1]   |  |    [0]    [1]    |
 *            +----------------+  +------------------+
 *                 |      |
 *  +----------------+  +----------------+
 *  |       (2)      |  |       (5)      |
 *  | 192.168.0.0/24 |  | 192.168.1.0/24 |
 *  |    value: 2    |  |     value: 5   |
 *  |   [0]    [1]   |  |   [0]    [1]   |
 *  +----------------+  +----------------+
 *
 * 192.168.1.1/32 would be a child of (5) etc.
 *
 * An intermediate node will be turned into a 'real' node on demand. In the
 * example above, (4) would be re-used if 192.168.0.0/23 is added to the trie.
 *
 * A fully populated trie would have a height of 32 nodes, as the trie was
 * created with a prefix length of 32.
 *
 * The lookup starts at the root node. If the current node matches and if there
 * is a child that can be used to become more specific, the trie is traversed
 * downwards. The last node in the traversal that is a non-intermediate one is
 * returned.
 */

static inline int extract_bit(const uint8_t *data, size_t index)
{
    return !!(data[index / 8] & (1 << (7 - (index % 8))));
}

/**
 * longest_prefix_match() - determine the longest prefix
 * @trie:	The trie to get internal sizes from
 * @node:	The node to operate on
 * @key:	The key to compare to @node
 *
 * Determine the longest prefix of @node that matches the bits in @key.
 */
static size_t longest_prefix_match(const lpm_trie_t *trie,
                                   const lpm_trie_node_t *node,
                                   const lpm_trie_key_t *key)
{
    size_t prefixlen = 0;
    size_t i;
    
    for (i = 0; i < trie->data_size; i++) {
        size_t b;
        
        b = 8 - fls(node->data[i] ^ key->data[i]);
        prefixlen += b;
        
        if (prefixlen >= node->prefixlen || prefixlen >= key->prefixlen)
            return min(node->prefixlen, key->prefixlen);
            
        if (b < 8)
            break;
    }
    
    return prefixlen;
}

/* Called from syscall or from eBPF program */
void *trie_lookup_elem(lpm_trie_t *trie, void *_key)
{
    lpm_trie_node_t *node, *found = NULL;
    lpm_trie_key_t *key = _key;
    
    /* Start walking the trie from the root node ... */
    
    for (node = trie->root; node;) {
        unsigned int next_bit;
        size_t matchlen;
        
        /* Determine the longest prefix of @node that matches @key.
         * If it's the maximum possible prefix for this trie, we have
         * an exact match and can return it directly.
         */
        matchlen = longest_prefix_match(trie, node, key);
        
        if (matchlen == trie->max_prefixlen) {
            found = node;
            break;
        }
        
        /* If the number of bits that match is smaller than the prefix
         * length of @node, bail out and return the node we have seen
         * last in the traversal (ie, the parent).
         */
        if (matchlen < node->prefixlen)
            break;
            
        /* Consider this node as return candidate unless it is an
         * artificially added intermediate one.
         */
        if (!(node->flags & LPM_TREE_NODE_FLAG_IM))
            found = node;
            
        /* If the node match is fully satisfied, let's see if we can
         * become more specific. Determine the next bit in the key and
         * traverse down.
         */
        next_bit = extract_bit(key->data, node->prefixlen);
        node = node->child[next_bit];
    }
    
    if (!found)
        return NULL;
        
    return found->data + trie->data_size;
}

/* Called from syscall or from eBPF program */
lpm_trie_node_t *trie_lookup_node(lpm_trie_t *trie, void *_key)
{
    lpm_trie_node_t *node, *found = NULL;
    lpm_trie_key_t *key = _key;
    
    /* Start walking the trie from the root node ... */
    
    for (node = trie->root; node;) {
        unsigned int next_bit;
        size_t matchlen;
        
        /* Determine the longest prefix of @node that matches @key.
         * If it's the maximum possible prefix for this trie, we have
         * an exact match and can return it directly.
         */
        matchlen = longest_prefix_match(trie, node, key);
        
        if (matchlen == trie->max_prefixlen) {
            found = node;
            break;
        }
        
        /* If the number of bits that match is smaller than the prefix
         * length of @node, bail out and return the node we have seen
         * last in the traversal (ie, the parent).
         */
        if (matchlen < node->prefixlen)
            break;
            
        /* Consider this node as return candidate unless it is an
         * artificially added intermediate one.
         */
        if (!(node->flags & LPM_TREE_NODE_FLAG_IM))
            found = node;
            
        /* If the node match is fully satisfied, let's see if we can
         * become more specific. Determine the next bit in the key and
         * traverse down.
         */
        next_bit = extract_bit(key->data, node->prefixlen);
        node = node->child[next_bit];
    }
    
    if (!found)
        return NULL;
        
    return found;
}


static lpm_trie_node_t *lpm_trie_node_alloc(const lpm_trie_t *trie,
                                                 const void *value)
{
    lpm_trie_node_t *node;
    size_t size = sizeof(lpm_trie_node_t) + trie->data_size;
    
    if (value)
        size += trie->value_size;
        
    node = yr_malloc(size);
    
    if (!node)
        return NULL;
        
    node->flags = 0;
    
    if (value)
        yr_memcpy(node->data + trie->data_size, value,
               trie->value_size);
               
    return node;
}

/* Called from syscall or from eBPF program */
int trie_update_elem(lpm_trie_t *trie,
                            void *_key, void *value, uint64_t flags)
{
    lpm_trie_node_t *node, *im_node = NULL, *new_node = NULL;
    lpm_trie_node_t  **slot;
    lpm_trie_key_t *key = _key;
    unsigned long irq_flags;
    unsigned int next_bit;
    size_t matchlen = 0;
    int ret = 0;
    
    if ((flags > BPF_EXIST))
        return -EINVAL;
        
    if (key->prefixlen > trie->max_prefixlen)
        return -EINVAL;
        
        
    /* Allocate and fill a new node */
    
    if (trie->n_entries == trie->max_entries) {
        ret = -ENOSPC;
        goto out;
    }
    
    new_node = lpm_trie_node_alloc(trie, value);
    
    if (!new_node) {
        ret = -ENOMEM;
        goto out;
    }
    
    trie->n_entries++;
    
    new_node->prefixlen = key->prefixlen;
    new_node->child[0] = NULL;
    new_node->child[1] = NULL;
    yr_memcpy(new_node->data, key->data, trie->data_size);
    
    /* Now find a slot to attach the new node. To do that, walk the tree
     * from the root and match as many bits as possible for each node until
     * we either find an empty slot or a slot that needs to be replaced by
     * an intermediate node.
     */
    slot = &trie->root;
    
    while ((node = *slot)) {
        matchlen = longest_prefix_match(trie, node, key);
        
        if (node->prefixlen != matchlen ||
            node->prefixlen == key->prefixlen ||
            node->prefixlen == trie->max_prefixlen)
            break;
            
        next_bit = extract_bit(key->data, node->prefixlen);
        slot = &node->child[next_bit];
    }
    
    /* If the slot is empty (a free child pointer or an empty root),
     * simply assign the @new_node to that slot and be done.
     */
    if (!node) {
        *slot = new_node;
        goto out;
    }
    
    /* If the slot we picked already exists, replace it with @new_node
     * which already has the correct data array set.
     */
    if (node->prefixlen == matchlen) {
        new_node->child[0] = node->child[0];
        new_node->child[1] = node->child[1];
        
        if (!(node->flags & LPM_TREE_NODE_FLAG_IM))
            trie->n_entries--;
            
        *slot = new_node;
        
        goto out;
    }
    
    /* If the new node matches the prefix completely, it must be inserted
     * as an ancestor. Simply insert it between @node and *@slot.
     */
    if (matchlen == key->prefixlen) {
        next_bit = extract_bit(node->data, matchlen);
        new_node->child[next_bit] = node;
        *slot = new_node;
        goto out;
    }
    
    im_node = lpm_trie_node_alloc(trie, NULL);
    
    if (!im_node) {
        ret = -ENOMEM;
        goto out;
    }
    
    im_node->prefixlen = matchlen;
    im_node->flags |= LPM_TREE_NODE_FLAG_IM;
    yr_memcpy(im_node->data, node->data, trie->data_size);
    
    /* Now determine which child to install in which slot */
    if (extract_bit(key->data, matchlen)) {
        im_node->child[0] = node;
        im_node->child[1] = new_node;
    } else {
        im_node->child[0] = new_node;
        im_node->child[1] = node;
    }
    
    /* Finally, assign the intermediate node to the determined spot */
    *slot = im_node;
    
out:

    if (ret) {
        if (new_node)
            trie->n_entries--;
            
        yr_free(new_node);
        yr_free(im_node);
    }
    
    return ret;
}

/* Called from syscall or from eBPF program */
int trie_delete_elem(lpm_trie_t *trie, void *_key)
{
    lpm_trie_key_t *key = _key;
    lpm_trie_node_t  **trim, **trim2;
    lpm_trie_node_t *node, *parent;
    unsigned long irq_flags;
    unsigned int next_bit;
    size_t matchlen = 0;
    int ret = 0;
    
    if (key->prefixlen > trie->max_prefixlen)
        return -EINVAL;
        
        
    /* Walk the tree looking for an exact key/length match and keeping
     * track of the path we traverse.  We will need to know the node
     * we wish to delete, and the slot that points to the node we want
     * to delete.  We may also need to know the nodes parent and the
     * slot that contains it.
     */
    trim = &trie->root;
    trim2 = trim;
    parent = NULL;
    
    while ((node = *trim)) {
        matchlen = longest_prefix_match(trie, node, key);
        
        if (node->prefixlen != matchlen ||
            node->prefixlen == key->prefixlen)
            break;
            
        parent = node;
        trim2 = trim;
        next_bit = extract_bit(key->data, node->prefixlen);
        trim = &node->child[next_bit];
    }
    
    if (!node || node->prefixlen != key->prefixlen ||
        (node->flags & LPM_TREE_NODE_FLAG_IM)) {
        ret = -ENOENT;
        goto out;
    }
    
    trie->n_entries--;
    
    /* If the node we are removing has two children, simply mark it
     * as intermediate and we are done.
     */
    if (node->child[0] &&
        node->child[1]) {
        node->flags |= LPM_TREE_NODE_FLAG_IM;
        goto out;
    }
    
    /* If the parent of the node we are about to delete is an intermediate
     * node, and the deleted node doesn't have any children, we can delete
     * the intermediate parent as well and promote its other child
     * up the tree.  Doing this maintains the invariant that all
     * intermediate nodes have exactly 2 children and that there are no
     * unnecessary intermediate nodes in the tree.
     */
    if (parent && (parent->flags & LPM_TREE_NODE_FLAG_IM) &&
        !node->child[0] && !node->child[1]) {
        if (node == parent->child[0])
            *trim2 = parent->child[1];
        else
            *trim2 = parent->child[0];
            
        yr_free(parent);
        yr_free(node);
        goto out;
    }
    
    /* The node we are removing has either zero or one child. If there
     * is a child, move it into the removed node's slot then delete
     * the node.  Otherwise just clear the slot and delete the node.
     */
    if (node->child[0])
        *trim = node->child[0];
    else if (node->child[1])
        *trim = node->child[1];
    else
        *trim = NULL;
        
    yr_free(node);
    
out:

    return ret;
}

#define LPM_DATA_SIZE_MAX	256
#define LPM_DATA_SIZE_MIN	1

#define LPM_VAL_SIZE_MAX	(1024*1024 - LPM_DATA_SIZE_MAX - \
				 sizeof(lpm_trie_node_t))
#define LPM_VAL_SIZE_MIN	1

#define LPM_KEY_SIZE(X)		(sizeof(lpm_trie_key_t) + (X))
#define LPM_KEY_SIZE_MAX	LPM_KEY_SIZE(LPM_DATA_SIZE_MAX)
#define LPM_KEY_SIZE_MIN	LPM_KEY_SIZE(LPM_DATA_SIZE_MIN)


lpm_trie_t *trie_alloc()
{
    lpm_trie_t *trie = NULL;
    
    trie = yr_malloc(sizeof(*trie));
    
    if (!trie)
        return NULL;
        
    /* copy mandatory map attributes */
    trie->data_size = KEY_SIZE;                            
    trie->max_prefixlen = MAX_PREFIXLEN;
    trie->max_entries = MAX_ENTRIES;                            
    trie->value_size = VALUE_SIZE;                              
    
    return trie;
}

void trie_free(lpm_trie_t *trie)
{
    lpm_trie_node_t **slot;
    lpm_trie_node_t *node;
    
    /* Wait for outstanding programs to complete
     * update/lookup/delete/get_next_key and free the trie.
     */
    
    /* Always start at the root and walk down to a node that has no
     * children. Then free that node, nullify its reference in the parent
     * and start over.
     */
    
    for (;;) {
        slot = &trie->root;
        
        for (;;) {
            node = *slot;
            
            if (!node)
                goto out;
                
            if (node->child[0]) {
                slot = &node->child[0];
                continue;
            }
            
            if (node->child[1]) {
                slot = &node->child[1];
                continue;
            }
            
            yr_free(node);
            *slot = NULL;
            break;
        }
    }
    
out:
    yr_free(trie);
}

static int trie_get_next_key(lpm_trie_t *trie, void *_key, void *_next_key)
{
    lpm_trie_node_t *node, *next_node = NULL, *parent, *search_root;
    lpm_trie_key_t *key = _key, *next_key = _next_key;
    lpm_trie_node_t **node_stack = NULL;
    int err = 0, stack_ptr = -1;
    unsigned int next_bit;
    size_t matchlen;
    
    /* The get_next_key follows postorder. For the 4 node example in
     * the top of this file, the trie_get_next_key() returns the following
     * one after another:
     *   192.168.0.0/24
     *   192.168.1.0/24
     *   192.168.128.0/24
     *   192.168.0.0/16
     *
     * The idea is to return more specific keys before less specific ones.
     */
    
    /* Empty trie */
    search_root = trie->root;
    
    if (!search_root)
        return -ENOENT;
        
    /* For invalid key, find the leftmost node in the trie */
    if (!key || key->prefixlen > trie->max_prefixlen)
        goto find_leftmost;
        
    node_stack = yr_malloc(trie->max_prefixlen * sizeof(lpm_trie_node_t *));
    
    if (!node_stack)
        return -ENOMEM;
        
    /* Try to find the exact node for the given key */
    for (node = search_root; node;) {
        node_stack[++stack_ptr] = node;
        matchlen = longest_prefix_match(trie, node, key);
        
        if (node->prefixlen != matchlen ||
            node->prefixlen == key->prefixlen)
            break;
            
        next_bit = extract_bit(key->data, node->prefixlen);
        node = node->child[next_bit];
    }
    
    if (!node || node->prefixlen != key->prefixlen ||
        (node->flags & LPM_TREE_NODE_FLAG_IM))
        goto find_leftmost;
        
    /* The node with the exactly-matching key has been found,
     * find the first node in postorder after the matched node.
     */
    node = node_stack[stack_ptr];
    
    while (stack_ptr > 0) {
        parent = node_stack[stack_ptr - 1];
        
        if (parent->child[0] == node) {
            search_root = parent->child[1];
            
            if (search_root)
                goto find_leftmost;
        }
        
        if (!(parent->flags & LPM_TREE_NODE_FLAG_IM)) {
            next_node = parent;
            goto do_copy;
        }
        
        node = parent;
        stack_ptr--;
    }
    
    /* did not find anything */
    err = -ENOENT;
    goto free_stack;
    
find_leftmost:

    /* Find the leftmost non-intermediate node, all intermediate nodes
     * have exact two children, so this function will never return NULL.
     */
    for (node = search_root; node;) {
        if (!(node->flags & LPM_TREE_NODE_FLAG_IM))
            next_node = node;
            
        node = node->child[0];
    }
    
do_copy:
    next_key->prefixlen = next_node->prefixlen;
    yr_memcpy((void *)next_key + offsetof(lpm_trie_key_t, data),
           next_node->data, trie->data_size);
free_stack:
    yr_free(node_stack);
    return err;
}




