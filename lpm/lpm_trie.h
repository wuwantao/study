#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

/* flags for BPF_MAP_UPDATE_ELEM command */
#define BPF_ANY		0 /* create new element or update existing */
#define BPF_NOEXIST	1 /* create new element if it didn't exist */
#define BPF_EXIST	2 /* update existing element */

#define CONFIG_X86_64   1
#define VALUE_SIZE      256                 //节点存放数据的大小
#define MAX_ENTRIES     (1UL<<20)           //设置能创建的最大的节点数
#define KEY_SIZE        16                  //要支持IPV4和IPV6这里就设置为16,如果只是IPV4设置为4
#define MAX_PREFIXLEN   (KEY_SIZE * 8)      //这个根据key_size变,不用管

struct lpm_trie_node;

typedef struct lpm_trie_node {
    struct lpm_trie_node *child[2];
    uint32_t		prefixlen;              //前缀长度数值
    uint32_t		flags;                  //是否为中间节点的标志 LPM_TREE_NODE_FLAG_IM
    uint8_t			data[0];                //[data + value] 如果是中间节点value为空
} lpm_trie_node_t;

typedef struct {
    lpm_trie_node_t     *root;
    size_t              n_entries;          //lpm_trie字典树已经创建的节点数
    size_t				max_prefixlen;      //lpm_trie字典树最大的前缀长度
    size_t				data_size;          //ipv4为4个字节,兼容ipv4和ipv6是16个字节
    size_t              value_size;         //存放value占用的字节数
    size_t              max_entries;        //lpm_trie字典树能创建的最大节点数
} lpm_trie_t;

/* Key of an a BPF_MAP_TYPE_LPM_TRIE entry */
typedef struct  {
    uint32_t	prefixlen;	/* up to 32 for AF_INET, 128 for AF_INET6 */
    uint8_t	    data[0];	/* Arbitrary size */
}lpm_trie_key_t;

lpm_trie_t *trie_alloc();                                           //创建lpm_trie字典树
void trie_free(lpm_trie_t *trie);                                   //释放lpm_trie字典树
/**
 *  添加,更新节点
 *  @_key  是lpm_trie_key_t类型
 *  @value 是节点的要存储的value
 *  @flags 是BPF_ANY or BPF_NOEXIST or BPF_EXIST
 */
int trie_update_elem(lpm_trie_t *trie, void *_key, void *value, uint64_t flags);   
int trie_delete_elem(lpm_trie_t *trie, void *_key);                 //删除lpm_trie某一个节点
void *trie_lookup_elem(lpm_trie_t *trie, void *_key);               //查找lpm_trie某一个节点的值
lpm_trie_node_t *trie_lookup_node(lpm_trie_t *trie, void *_key);    //查找lpm_trie某一个节点的结构体
