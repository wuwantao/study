#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "list.h"

#define CNT (1<<20)

struct hlist_head dns_hlist[CNT];

typedef struct {
    char *dns;
    uint16_t len;
    struct hlist_node hnode;
} dns_node_t;


static inline int yr_tolower(int c)
{
    switch (c) {
    case 'A'...'Z':
        c += 'a' - 'A';
        break;
    default:
        break;
    }

    return c;
}

uint32_t bkdr_hash(char *dname,  int dname_len)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    char tmp_char;

    int i = 0;
    while (i < dname_len) {
        tmp_char = yr_tolower(dname[i]); //全部转成小写进行比较
        hash = hash * seed + tmp_char;
        i++;
    }


    return (hash & ((1UL << 20) - 1));
    // return (hash & 0x7FFFFFFF);
}


void hash_add(char *dns, uint16_t len)
{
    struct hlist_head *h = NULL;
    dns_node_t *dns_node = NULL;

    h = dns_hlist + bkdr_hash(dns, len);
    dns_node = malloc(sizeof(dns_node_t));
    memset(dns_node, 0, sizeof(dns_node_t));
    dns_node->len = len;
    dns_node->dns = malloc(len);
    memcpy(dns_node->dns, dns, len);

    INIT_HLIST_NODE(&dns_node->hnode);

    hlist_add_head(&dns_node->hnode, h);
}

bool hash_search(char *dns, uint16_t len)
{
    dns_node_t *dns_node = NULL;
    struct hlist_head *h = NULL;
    h = dns_hlist + bkdr_hash(dns, len);
    hlist_for_each_entry(dns_node, h, hnode) {
        if (len == dns_node->len && memcmp(dns_node->dns, dns, len) == 0) {
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    clock_t startTime, endTime;
    int i = 0;
    FILE *fp = NULL;
    char line[1024];
    uint64_t size = 0;

    if (argc != 2) {
        printf("./t dns.txt\n");
        return -1;
    }

    for (i = 0; i < CNT; i++) {
        INIT_HLIST_HEAD(dns_hlist + i);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("file open failed.%s\n", argv[1]);
        return -1;
    }

    while (fgets(line, 1024, fp) != NULL) {
        line[strlen(line) - 1] = 0;
        hash_add(line, strlen(line));
        size ++;
    }
    printf("read size:%lld\n", size);

    fseek(fp, 0L, SEEK_SET);

    size = 0;
    startTime = clock();
    while (fgets(line, 1024, fp) != NULL) {
        line[strlen(line) - 1] = 0;
        if (hash_search(line, strlen(line)) != true) {
            printf("not find dns. dns:%s\n", line);
            break;
        }
        size++;
    }

    endTime = clock();
    printf("size: %lld the running time is %lld\n", size, (double)(endTime - startTime));
    getchar();
    return 0;
}
