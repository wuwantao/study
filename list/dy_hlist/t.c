#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "list.h"

static uint32_t CRC32[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};


uint32_t CRCHash(uint32_t hash, void *buf, uint16_t len)
{
    int i;
    uint8_t *pbuf = (uint8_t *)buf;
    
    for (i = 0; i < len; i++) {
        hash = CRC32[((hash & 0xFF) ^ pbuf[i])] ^ (hash >> 8);
    }
    
    return hash;
}

uint32_t hash_shift(uint32_t hash, uint8_t bits)
{
    return hash & ((1UL << bits) - 1);
}

uint32_t hash(void *data, uint8_t len, uint8_t hash_bits)
{
    uint32_t hash = 0;
    
    hash = CRCHash(hash, data, len);
    
    return hash_shift(hash, hash_bits);
}

typedef struct {
    uint32_t capcity;        //初始值为4,容量用光时,扩容一倍,删除hash时,发现容量多余3/4时,空间减半
    uint32_t size;
    uint32_t hash_bits;
    struct hlist_head *head;
} dy_hlist_mgr_t;

typedef struct {
    uint32_t key_len;
    void *key;
    void *val;
    struct hlist_node hnode;
} dy_hlist_node_t;

int dy_hlist_init(dy_hlist_mgr_t *dy_hlist_mgr)
{
    uint32_t i = 0;
    dy_hlist_mgr->capcity = 4;
    dy_hlist_mgr->size = 0;
    dy_hlist_mgr->hash_bits = 2;//log(dy_hlist_mgr->capcity)/log(2);
    dy_hlist_mgr->head = malloc(sizeof(struct hlist_head) * dy_hlist_mgr->capcity);
    
    for (i = 0; i < 4; i++) {
        INIT_HLIST_HEAD(dy_hlist_mgr->head + i);
    }
    
    return 0;
}

void dy_hlist_exit(dy_hlist_mgr_t *dy_hlist_mgr)
{
    dy_hlist_node_t *dy_hlist_node = NULL;
    struct hlist_node *pos = NULL, *n = NULL;
    uint32_t i = 0;
    struct hlist_head *h = NULL;
    
    for (i = 0; i < dy_hlist_mgr->capcity; i++) {
        h = dy_hlist_mgr->head + i;
        hlist_for_each_safe(pos, n, h) {
            dy_hlist_node = hlist_entry(pos, dy_hlist_node_t, hnode);
            free(dy_hlist_node->key);
            free(dy_hlist_node->val);
            hlist_del(&dy_hlist_node->hnode);
            free(dy_hlist_node);
        }
    }
    
    free(dy_hlist_mgr->head);
}

int dy_hlsit_find_by_head(dy_hlist_mgr_t *dy_hlist_mgr, void *key, uint32_t key_len, struct hlist_head *h, dy_hlist_node_t **dy_hlist_node)
{
    struct hlist_node *pos = NULL, *n = NULL;
    hlist_for_each_safe(pos, n, h) {
        *dy_hlist_node = hlist_entry(pos, dy_hlist_node_t, hnode);
        
        if ((*dy_hlist_node)->key_len == key_len && memcmp((*dy_hlist_node)->key, key, key_len) == 0) {
            return 0;
        }
    }
    *dy_hlist_node = NULL;
    return 0;
}

int dy_hlist_find(dy_hlist_mgr_t *dy_hlist_mgr, void *key, uint32_t key_len, void **val)
{
    struct hlist_node *pos = NULL, *n = NULL;
    uint32_t hash_val = hash(key, key_len, dy_hlist_mgr->hash_bits);
    struct hlist_head *h = dy_hlist_mgr->head + hash_val;
    dy_hlist_node_t *dy_hlist_node = NULL;
    hlist_for_each_safe(pos, n, h) {
        dy_hlist_node = hlist_entry(pos, dy_hlist_node_t, hnode);
        
        if (dy_hlist_node->key_len == key_len && memcmp(dy_hlist_node->key, key, key_len) == 0) {
            *val = dy_hlist_node->val;
            return 0;
        }
    }
    *val = NULL;
    return 0;
}

int dy_hlist_resize(dy_hlist_mgr_t *dy_hlist_mgr, uint32_t new_capcity)
{
    uint32_t i = 0;
    uint32_t hash_bits = 0;
    uint32_t hash_val  = 0;
    struct hlist_head *new_h = malloc(sizeof(struct hlist_head) * new_capcity);
    struct hlist_head *h = NULL;
    dy_hlist_node_t *dy_hlist_node = NULL;
    struct hlist_node *pos = NULL, *n = NULL;
    
    if (new_capcity < dy_hlist_mgr->capcity) {
        hash_bits = dy_hlist_mgr->hash_bits - 1;
    } else {
        hash_bits = dy_hlist_mgr->hash_bits + 1;
    }
    
    if (new_h == NULL) {
        printf("malloc new hlist failed.\n");
        return -1;
    }
    
    for (i = 0; i < new_capcity; i++) {
        INIT_HLIST_HEAD(new_h + i);
    }
    
    for (i = 0; i < dy_hlist_mgr->capcity; i++) {
        h = dy_hlist_mgr->head + i;
        hlist_for_each_safe(pos, n, h) {
            dy_hlist_node = hlist_entry(pos, dy_hlist_node_t, hnode);
            hlist_del(&dy_hlist_node->hnode);
            hash_val = hash(dy_hlist_node->key, dy_hlist_node->key_len, hash_bits);
            hlist_add_head(&dy_hlist_node->hnode, new_h + hash_val);
        }
    }
    
    free(dy_hlist_mgr->head);
    dy_hlist_mgr->head = new_h;
    dy_hlist_mgr->capcity = new_capcity;
    dy_hlist_mgr->hash_bits = hash_bits;
    
    return 0;
}


int dy_hlist_add(dy_hlist_mgr_t *dy_hlist_mgr, void *key, uint32_t key_len, void *val, uint32_t val_len)
{
    uint32_t hash_val = hash(key, key_len, dy_hlist_mgr->hash_bits);
    struct hlist_head *h = dy_hlist_mgr->head + hash_val;
    dy_hlist_node_t *dy_hlist_node = NULL;
    
    dy_hlsit_find_by_head(dy_hlist_mgr, key, key_len, h, &dy_hlist_node);
    
    if (dy_hlist_node == NULL) {
        dy_hlist_node = malloc(sizeof(dy_hlist_node_t));
        
        if (dy_hlist_node == NULL) {
            printf("malloc dy_hlist_node failed.\n");
            return -1;
        }
        
        dy_hlist_node->key = malloc(key_len);
        dy_hlist_node->val = malloc(val_len);
        
        if (dy_hlist_node->key == NULL || dy_hlist_node->val == NULL) {
            printf("malloc dy_hlist_node->key or val failed.\n");
            
            if (dy_hlist_node->key != NULL) {
                free(dy_hlist_node->key);
            }
            
            if (dy_hlist_node->val != NULL) {
                free(dy_hlist_node->val);
            }
            
            free(dy_hlist_node);
            return -1;
        }
        
        dy_hlist_node->key_len = key_len;
        INIT_HLIST_NODE(&dy_hlist_node->hnode);
        memcpy(dy_hlist_node->key, key, key_len);
        memcpy(dy_hlist_node->val, val, val_len);
        hlist_add_head(&dy_hlist_node->hnode, h);
        dy_hlist_mgr->size++;
    } else {
        memcpy(dy_hlist_node->val, val, val_len);
        return 0;
    }
    
    if (dy_hlist_mgr->size > dy_hlist_mgr->capcity) {
        if (dy_hlist_resize(dy_hlist_mgr, dy_hlist_mgr->capcity * 2) != 0) {
            printf("resize dy_hlist failed.\n");
            return -1;
        }
    }
    
    return 0;
}

int dy_hlist_del(dy_hlist_mgr_t *dy_hlist_mgr, void *key, uint32_t key_len)
{
    uint32_t hash_val = hash(key, key_len, dy_hlist_mgr->hash_bits);
    struct hlist_head *h = dy_hlist_mgr->head + hash_val;
    dy_hlist_node_t *dy_hlist_node = NULL;
    
    dy_hlsit_find_by_head(dy_hlist_mgr, key, key_len, h, &dy_hlist_node);
    
    if (dy_hlist_node == NULL) {
        return -1;
    }
    
    free(dy_hlist_node->key);
    free(dy_hlist_node->val);
    hlist_del(&dy_hlist_node->hnode);
    free(dy_hlist_node);
    dy_hlist_mgr->size--;
    
    if (dy_hlist_mgr->capcity > 4 && dy_hlist_mgr->size <= dy_hlist_mgr->capcity / 4) {
        if (dy_hlist_resize(dy_hlist_mgr, dy_hlist_mgr->capcity / 2) != 0) {
            printf("resize dy_hlist failed.\n");
            return -1;
        }
    }
    
    return 0;
}

dy_hlist_mgr_t dy_hlist_mgr;
//白盒测试,保存接口里面的每个if 等每行代码都能执行过,来设计测试case
int main(int argc, char *argv[])
{
    uint32_t i = 0;
    uint32_t count = 0;
    char val[100] = {0};
    char *p = NULL;    
//    mallopt(M_MMAP_THRESHOLD, 0);

    if (argc == 2) {
        count = atoi(argv[1]);
    } else {
        count = 10000 * 500;
    }
    
    dy_hlist_init(&dy_hlist_mgr);
    
#if 0
    
//测试添加100万数据,删除100万数据
    for (int j = 0; j < 6; j++) {
        printf("\n###########################\n");
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        
        for (i = 0; i < count; i++) {
            sprintf(val, "hello world! idx:%08d", i);
            dy_hlist_add(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void *)val, strlen(val));
        }
        
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        
        printf("%dk\n", (sizeof(uint32_t) + strlen(val) + sizeof(dy_hlist_node_t))*count / 1024 + sizeof(struct hlist_head) * dy_hlist_mgr.capcity / 1024);
        
        
        char *p = NULL;
        
        for (i = 0; i < 3; i++) {
            dy_hlist_find(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void **)&p);
            printf("key:%d val:%s\n", i, p);
        }
        
        printf("add end\n");
        getchar();
        
        for (i = 0; i < count; i++) {
            dy_hlist_del(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t));
        }
        
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("del end\n");
        getchar();
    }
    
#endif
    
#if 0
    
//测试添加100万数据,删除前50万数据,然后在添加50万数据
    for (int j = 0; j < 1; j++) {
        printf("\n###########################\n");
        
        for (i = 0; i < count; i++) {
            sprintf(val, "hello world! idx:%08d", i);
            dy_hlist_add(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void *)val, strlen(val));
        }
        
        printf("\nadd count\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("%dk\n", (sizeof(uint32_t) + strlen(val) + sizeof(dy_hlist_node_t))*count / 1024 + sizeof(struct hlist_head) * dy_hlist_mgr.capcity / 1024);
        
        char *p = NULL;
        
        for (i = 0; i < 3; i++) {
            dy_hlist_find(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void **)&p);
            printf("key:%d val:%s\n", i, p);
        }
        
        printf("add end\n");
        
        for (i = 0; i < count / 2; i++) {
            if (dy_hlist_del(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t)) == -1) {
                printf("del failed.\n");
                exit(-1);
            }
        }
        
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("del count/2 end\n");
        
        for (i = 0; i < count / 2; i++) {
            sprintf(val, "hello world! idx:%08d", i);
            
            if (dy_hlist_add(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void *)val, strlen(val)) == -1) {
                printf("add failed.\n");
                exit(-1);
            }
        }
        
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("add count/2 end\n");
        
        for (i = 0; i < count / 2; i++) {
            if (dy_hlist_del(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t)) == -1) {
                printf("del failed.\n");
                exit(-1);
            }
        }
        
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("add count/2 end\n");
    }
    
#endif
    
    
#if 0
    
//测试添加100万数据,在添加100万重复数据
    for (int j = 0; j < 1; j++) {
        printf("\n###########################\n");
        
        for (i = 0; i < count; i++) {
            sprintf(val, "hello world! idx:%08d", i);
            
            if (dy_hlist_add(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void *)val, strlen(val)) == -1) {
                printf("add failed.\n");
                exit(-1);
            }
        }
        
        printf("\nadd count\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("%dk\n", (sizeof(uint32_t) + strlen(val) + sizeof(dy_hlist_node_t))*count / 1024 + sizeof(struct hlist_head) * dy_hlist_mgr.capcity / 1024);
        
        for (i = 0; i < count; i++) {
            sprintf(val, "hello world! idx:%08d", i);
            
            if (dy_hlist_add(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void *)val, strlen(val)) == -1) {
                printf("add failed.\n");
                exit(-1);
            }
        }
        
        printf("\ncapcity:%d\n", dy_hlist_mgr.capcity);
        printf("size:%d\n", dy_hlist_mgr.size);
        printf("hash_bits:%d\n", dy_hlist_mgr.hash_bits);
        printf("add count end\n");
        
    }
    
#endif
    
#if 1
    
//测试性能
    clock_t startTime, endTime;

    
    printf("\n###########################\n");
    sprintf(val, "hello world!");
    startTime = clock();
    
    for (i = 0; i < count; i++) {
        if (dy_hlist_add(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void *)val, strlen(val)) == -1) {
            printf("add failed.\n");
            exit(-1);
        }
    }
    
    endTime = clock();
    printf("dy_hlist_add pps:%f\n",  count / ((double)(endTime - startTime) / CLOCKS_PER_SEC) );
    
    startTime = clock();
    
    for (i = 0; i < count; i++) {
        if (dy_hlist_del(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t)) == -1) {
            printf("del failed.\n");
            exit(-1);
        }
    }
    
    endTime = clock();
    printf("dy_hlist_del pps:%f\n",  count / ((double)(endTime - startTime) / CLOCKS_PER_SEC) );
    
    
    startTime = clock();
    
    for (i = 0; i < count; i++) {
        if (dy_hlist_find(&dy_hlist_mgr, (void *)&i, sizeof(uint32_t), (void **)&p) == -1) {
            printf("find failed.\n");
            exit(-1);
        }
    }
    
    endTime = clock();
    printf("dy_hlist_find pps:%f\n",  count / ((double)(endTime - startTime) / CLOCKS_PER_SEC) );
    
    
#endif
    
    
    dy_hlist_exit(&dy_hlist_mgr);
    printf("exit end\n");
//    getchar();

    return 0;
}
