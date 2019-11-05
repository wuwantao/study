#include <stdio.h>
#include <stdlib.h>
#include "list.h"


#define CNT 10

struct hlist_head test_hlist[CNT];
typedef struct {
    int cnt;
    struct hlist_node hnode;
}test_t;

void print_first()
{
    int i = 0;
    struct hlist_head *h = NULL;
    printf("print\n");
    for(i = 0; i < CNT; i++) {
        h = test_hlist + i;
        printf("h->first:%p\n", h->first);
    }
}
int main()
{
    int i = 0;
    test_t *test = NULL;
    struct hlist_head *h = NULL;

    for(i = 0; i < CNT; i++) {
        INIT_HLIST_HEAD(test_hlist + i);
    }

    print_first();

    for (i = 0 ; i < CNT; i++) {
        test = malloc(sizeof(test_t));
        memset(test,0,sizeof(test_t));
        test->cnt = i;
        INIT_HLIST_NODE(&test->hnode);
        h = test_hlist + i;
        hlist_add_head(&test->hnode, h);
    }

    print_first();

    hlist_del(&test->hnode);

    print_first();

    return 0;
}
