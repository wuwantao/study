#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("argc must be 2\n ./t file.txt\n");
        return -1;
    }
    char fname[100];
    char line[1024];
    char *p = NULL;
    char *t = NULL;


    FILE *src = NULL;
    FILE *dst = NULL;

    int i = 0;
    for (i = 1; i < argc; i++) {
        src = fopen(argv[i], "r");
        if (src == NULL) {
            printf("src open failed. src_file:%s\n", argv[i]);
            return -1;
        }
        strcpy(fname, argv[i]);
        strcat(fname, ".dst");
        printf("dst_file:%s\n", fname);

        dst = fopen(fname, "w");
        if (dst == NULL) {
            printf("dst open failed. dst_file:%s\n", fname);
            return -1;
        }
        while (fgets(line, 1024, src) != NULL) {
            p = strchr(line, '|');
            t = p + 1;
            p = strchr(t, '|');
            *p = '\n';
            *(p + 1) = '\0';
            fwrite(t, 1, strlen(t), dst);
        }

        fclose(src);
        fclose(dst);

    }


    return 0;
}
