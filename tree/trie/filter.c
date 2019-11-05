#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

bool is_valid(char *str)
{
    while (*str != 0) {
        if ( !((*str >= 97 && *str <= 122) || ( *str >= 45 && *str <= 57)) )
            return false;
        str++;
    }
    return true;
}

void str_reverse(char *str)
{
    int len;
    char t;
    int i = 0;
    len = strlen(str);
    for (i = 0; i < len/2; i++) {
        t = str[i];
        str[i] = str[len - i - 1];
        str[len -i - 1] = t;
    }
}
void test_str_reverse()
{
    char *p = malloc(100);
    memset(p, 0, 100);
    memcpy(p,"123",4);
    str_reverse(p);
    printf("p:%s\n",p);
}

int main(int argc, char *argv[])
{
    
    if (argc != 3) {
        printf("./t src.txt dst.txt\n");
        return -1;
    }
    FILE *src = NULL;
    FILE *dst = NULL;
    char line[1024];

    src = fopen(argv[1], "r");
    if (src == NULL) {
        printf("fopen failed.src:%s\n", argv[1]);
        return -1;
    }

    dst = fopen(argv[2], "w");
    if (dst == NULL) {
        printf("fopen failed.dst:%s\n", argv[2]);
        return -1;
    }

    while (fgets(line, 1024, src) != NULL) {
        line[strlen(line) - 1] = 0;

        if (is_valid(line) != true) {
            printf("invalid str:%s\n", line);
            continue;
        }
        str_reverse(line);
        line[strlen(line)] = '\n';
        fwrite(line, 1, strlen(line), dst);
        memset(line, 0, sizeof(line));
    }

    printf("filter ok\n");
    fclose(src);
    fclose(dst);

    return 0;
}
