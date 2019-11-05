#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

typedef struct trie_node trie_node_t;

struct trie_node {
    uint8_t is_word;
    trie_node_t *branch[38];   
};

char   n[128] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,'-','.',47,'0','1','2','3','4','5','6','7','8','9',58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,'a',98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,'z',123,124,125,126,127};
char idx[128] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,-1,2,3,4,5,6,7,8,9,10,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,-1,-1,-1,-1,-1};

trie_node_t *root;
trie_node_t *nodes = NULL;

void insert(char *str);
bool search(char*str);

int tolower(int c)
{
    if ((c >= 'A') && (c <= 'Z'))
        return c + ('a' - 'A');
        
    return c;
}

char *strlowr(char *str)
{
    char *orign = str;
    
    for (; *str != '\0'; str++)
        *str = tolower(*str);
        
    return orign;
}


void insert(char *str)
{
    static int count = 0;
    trie_node_t *tt = root;
    trie_node_t **tmp = NULL;
    
    for (int i = 0; str[i]; i++) {
        tmp = &tt->branch[idx[str[i]]];
        if (*tmp == NULL) {
            *tmp = nodes + count++;////malloc(sizeof(trie_node_t));
            memset(*tmp, 0, sizeof(trie_node_t));
        }
        
        tt = *tmp;
    }
    
    tt->is_word = 1;
}
bool search(char *str)
{
    trie_node_t *tt = root;
    
    while (tt != NULL && *str != 0) {
        tt = tt->branch[idx[*str]];
        str++;
    }
    
    if (tt && tt->is_word == 1) {
        return true;
    }
    
    return false;
}

int main()
{
    clock_t startTime, endTime;
    FILE *fp = NULL;
    uint64_t size = 0;
    char line[1024];
    root = malloc(sizeof(trie_node_t));
    nodes = malloc(sizeof(trie_node_t) * 6100000);
    memset(root, 0, sizeof(trie_node_t));
    
    fp = fopen("file.txt", "r");
    
    if (fp == NULL) {
        printf("fopen failed.\n");
        return -1;
    }

    while (fgets(line, 1024, fp) != NULL) {
        strlowr(line);
        line[strlen(line) - 1] = 0;
        insert(line);
        size ++;
        memset(line, 0, sizeof(line));
    }
    
    printf("read size:%lld\n", size);
    fflush(fp);
    
    size = 0;
    fseek(fp, 0L, SEEK_SET);
    startTime = clock();
    while (fgets(line, 1024, fp) != NULL) {
        line[strlen(line) - 1] = 0;
        
        if (search(line) != true) {
            printf("search failed. str:%s\n", line);
            break;
        }
        
        size ++;
        memset(line, 0, sizeof(line));
    }
    
    endTime = clock();
    printf("size: %lld the running time is %lld\n", size, (double)(endTime - startTime));
    fclose(fp);
    getchar();
    return 0;
}

