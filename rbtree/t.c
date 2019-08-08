#include <stdio.h>
#include <stdlib.h>

#include <rbtree.h>
#include <list.h>


static rbroot_t     g_rbroot;

void output()
{
    rbnode_t    *rbnode, *prev;
    rbnode = rbtree_max(&g_rbroot);
    struct list_head *pos, *n, *h;
    while (rbnode) {
        /*
           h = &rbnode->content;
           list_for_each_safe(pos, n, h) {
           limit = list_entry(pos, limit_mac_t, lnode);
           if (limit->start < g_limit->report_time) {
           list_del_init(&limit->lnode);
           } else {
           count++;
           add_topn_mac_to_report(limit->mac, limit->count);
           if (count == 100)
           break;
           }
           }
           */
        printf("key:%d\n", rbnode->key);
        prev = rbnode_prev(rbnode);
        rbnode = prev;
    }
}

int main()
{

    g_rbroot.rbnode = NULL;
    int i = 0;
    rbnode_t    *rbnode;

    for(i = 0; i < 100*10000; i++)
    {

        rbnode = add_rbnode(&g_rbroot, rand());
    }

    //    output();

    destroy_rbtree(&g_rbroot);


}
