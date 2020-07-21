#include "lpm_trie.h"
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void t1() {
  uint32_t count = 1UL << 20;
  uint32_t i = 0;
  lpm_trie_t *trie = NULL;
  clock_t start = 0;
  clock_t end = 0;
  //	  mallopt(M_MMAP_THRESHOLD, 0);
  trie = trie_alloc();

  if (trie == NULL) {
    printf("malloc trie failed.\n");
    return -1;
  }

  lpm_trie_key_t *key = malloc(sizeof(lpm_trie_key_t) + trie->data_size);

  if (key == NULL) {
    printf("malloc key failed.\n");
    return -1;
  }

  start = clock();

  for (i = 0; i < count; i++) {
    memset(key, 0, sizeof(lpm_trie_key_t));
    key->prefixlen = i % 127 + 1;
    uint32_t *p = (uint32_t *)key->data;
    *p = i;
    int ret = trie_update_elem(trie, key, (void *)p, BPF_ANY);

    if (ret < 0) {
      printf("add or update failed. ret:%d error:%s\n", ret, strerror(errno));
      return -1;
    }
  }

  end = clock();
  printf("entries:%ld\n", trie->n_entries);
  printf("add or update pps:%lf\n",
         count / ((double)(end - start) / CLOCKS_PER_SEC));

  start = clock();

  for (i = 0; i < count; i++) {
    memset(key, 0, sizeof(lpm_trie_key_t));
    key->prefixlen = i % 127 + 1;
    uint32_t *p = (uint32_t *)key->data;
    *p = i;
    int ret = trie_delete_elem(trie, key);
  }

  end = clock();
  printf("entries:%ld\n", trie->n_entries);
  printf("delete pps:%lf\n", count / ((double)(end - start) / CLOCKS_PER_SEC));

  getchar();

  return 0;
}

int main() {
  lpm_trie_t *trie = NULL;
  int ret = 0;
  int i = 0;
  uint8_t val[256];
  uint8_t data[16] = {0xc0, 0xa8, 0x1, 0x2};
  lpm_trie_node_t *node = NULL;

  trie = trie_alloc();

  if (trie == NULL) {
    printf("malloc trie failed.\n");
    return -1;
  }

  lpm_trie_key_t *key = malloc(sizeof(lpm_trie_key_t) + trie->data_size);

  if (key == NULL) {
    printf("malloc key failed.\n");
    return -1;
  }

  for (i = 0; i < 5; i++) {
    key->prefixlen = 2 * i;
    memcpy(key->data, data, 16);

    trie_update_elem(trie, key, (void *)&val, BPF_ANY);
  }

  key->prefixlen = 9;
  memcpy(key->data, data, 16);

  node = trie_lookup_node(trie, key);


  if (node != NULL) {
  	printf("node->prefix:%d\n", node->prefixlen);
  }


  return 0;
}
