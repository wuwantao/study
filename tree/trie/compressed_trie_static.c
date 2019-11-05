#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdint.h>
#include<stdbool.h>

typedef struct trie_node trie_node_t;

#define CHAR_CNT 38

struct trie_node {
    uint8_t is_word;
    uint8_t len;
    char label[128];
    trie_node_t *branch[CHAR_CNT];
};

char   n[128] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, '-', '.', 47, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 'a', 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 'z', 123, 124, 125, 126, 127};
char idx[128] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1, -1,
                  2, 3, 4, 5, 6, 7, 8, 9, 10,11, -1, -1, -1, -1, -1, -1, -1, -1,
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12,
                 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
                 29, 30, 31, 32, 33, 34, 35, 36, 37, -1, -1, -1, -1, -1};
char bdx[38]  = {45, 46, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100,
                101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 
                115, 116, 117, 118, 119, 120, 121, 122};

trie_node_t *root;
trie_node_t *nodes = NULL;
int node_count = 0;

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
    trie_node_t *tt = root;
    trie_node_t **tmp = NULL;
    
    for (int i = 0; str[i]; i++) {
        tmp = &tt->branch[idx[str[i]]];
        
        if (*tmp == NULL) {
            *tmp = nodes + node_count;
            //*tmp = malloc(sizeof(trie_node_t));
            node_count++;
            memset(*tmp, 0, sizeof(trie_node_t));
            //(*tmp)->label = (char*)malloc(sizeof(char) +2);
            (*tmp)->label[0] = str[i];
            (*tmp)->label[1] = '\0';
            (*tmp)->len = 1;
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

void trieToRadix(trie_node_t *root)
{
    trie_node_t *tmp = NULL;
    int i, count = 0, len, index = 0, j = 0;
    bool multi_child = false;
    bool is_leaf = true;
    
    for (i = 0; i < CHAR_CNT; ++i) {
        if (root->branch[i] != NULL) {
            count++;
            index = i;
        }
    }
    
    if (count > 1) {
        multi_child = true;
    }
    
    if (root->label == NULL) {
        multi_child = true;
    }
    
    if (count == 1 && root->label != NULL && root->is_word != true) {
        trie_node_t *tmp = root->branch[index];
        //root->label = stradd(root->label, index);
        root->label[root->len]     = bdx[index];
        root->label[root->len + 1] = '\x0';
        root->len   += 1;//strlen(root->label);
        
        for (i = 0; i < CHAR_CNT; ++i) {
            root->branch[i] = tmp->branch[i];
        }
        
        root->is_word = tmp->is_word;
        
        node_count--;
        //free(tmp);
        multi_child = false;    //未知多少个孩子,重新计算
    } else {
        multi_child = true;     //多个孩子
    }
    
    is_leaf = true;
    
    for (i = 0; i < CHAR_CNT; ++i) {
        if (root->branch[i] != NULL) {
            is_leaf = false;
            break;
        }
    }
    
    if (is_leaf == true)
        return;
        
    for (j = 0; j < CHAR_CNT; ++j) {
        if (root->branch[j] != NULL) {
            if (multi_child == true) {
                trieToRadix(root->branch[j]);
            } else {
                trieToRadix(root);
            }
        }
    }
}

trie_node_t *search2(trie_node_t *root, const char *word)
{
    trie_node_t *currentNode = root;
    int index = 0;
    int size = strlen(word);
    int len = 0;
    
    while (index < size) {
        currentNode = currentNode->branch[idx[*word]];
        if (currentNode != NULL) {
            len = currentNode->len;
            
            if (memcmp(word, currentNode->label, len) == 0) {
                index += len;
                word += len;
            } else {
                printf("word:%s\t label:%s\n", word, currentNode->label);
                return NULL;
            }
            
        }
    }
    
    if (index == size && currentNode->is_word == 1) {
        return currentNode;
    }
    
    return NULL;
}


int main()
{
    clock_t startTime, endTime;
    FILE *fp = NULL;
    uint64_t size = 0;
    char line[1024];
    root = malloc(sizeof(trie_node_t));
    nodes = malloc(sizeof(trie_node_t) * 22258479);
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
    printf("start trieToRadix, node count = %d\n", node_count);
    trieToRadix(root);
    printf("trieToRadix, node count=%d\n", node_count);
    startTime = clock();
    
    while (fgets(line, 1024, fp) != NULL) {
        line[strlen(line) - 1] = 0;
        
        if (search2(root, line) == NULL) {
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

