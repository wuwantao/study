#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

char seed[39] = "abcdefghijklmnopqrstuvwxyz0123456789.-";

void genstr(char *str, uint64_t strLen)
{
    uint64_t i = 0;
    
    for (i = 0; i < strLen; i++) {
//        str[i] = seed[rand()%38];
        str[i] = rand()%26 + 97;
    }
    
}

int main(int argc, char *argv[])
{
    uint64_t i = 0;
    uint64_t strLen = 0;
    uint64_t fileSize = 0;
    uint64_t cnt = 0;
    char *str = NULL;
    
    FILE *fp = NULL;
    
    if (argc != 3) {
        printf("./t 字符串长度        文件大小(M)\n");
    }
    
    strLen = atoi(argv[1]);
    fileSize = atoi(argv[2]);
    cnt = fileSize * 1024 * 1024 / strLen;
    str = malloc(strLen + 1);
    
    if (str == NULL) {
        printf("str malloc failed.\n");
        return -1;
    }
    
    fp = fopen("file.txt", "w");
    
    if (fp == NULL) {
        printf("create file failed.\n");
        return -1;
    }
    
    for (i = 0; i < cnt; i++) {
        memset(str, '\n', strLen + 1);
        genstr(str, strLen);
        fwrite(str, 1, strlen(str), fp);
    }
    
    fclose(fp);
    return 0;
}
