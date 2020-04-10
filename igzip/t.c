#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "igzip_lib.h"

#define BUF_LEN (2<<21)
#define YASUO_BUF_LEN (2<<22)

int dump_buffer(uint8_t* buf, size_t len)
{
    int i = 0;
    for (i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
    }
    
    return 0;
}
int main(int argc, char *argv[])
{
    char sname[100];
    char dname[100];
    FILE *src = NULL;
    FILE *dst = NULL;
    uint8_t *inbuf = malloc(BUF_LEN);
    uint8_t *outbuf = malloc(YASUO_BUF_LEN);
    printf("begin\n");
    if (argc >= 3) {
        strcpy(sname, argv[1]);
        strcpy(dname, argv[2]);
    } else {
        printf("./t src.txt dst.gz\n");
        strcpy(sname, "src.txt");
        strcpy(dname, "dst.gz");
    }
    
    src = fopen(sname, "r");
    
    if (src == NULL) {
        printf("src open failed.src:%s\n", argv[1]);
        return -1;
    }
    
    dst = fopen(dname, "w");
    
    if (dst == NULL) {
        printf("dst open failed.src:%s\n", argv[2]);
        return -1;
    }
    
    LZ_Stream2 stream;
    init_stream(&stream);
#if 1
//    while (fgets(line, sizeof(line), src) != NULL) {
//
//        stream.end_of_stream = feof(src);
//        stream.avail_in = strlen(line);
//        stream.next_in = line;

//        stream.avail_out = BUF_LEN;
//        stream.next_out = rst;
//        fast_lz(&stream);

//        fwrite(rst, 1, BUF_LEN - stream.avail_out, dst);

//        memset(line, 0, sizeof(line));
//        memset(rst, 0, sizeof(rst));

//    }

    do {
        stream.avail_in = (UINT32) fread(inbuf, 1, BUF_LEN, src);
        stream.end_of_stream = feof(src);
        stream.next_in = inbuf;
        
        do {
            stream.avail_out = YASUO_BUF_LEN;
            stream.next_out = outbuf;
            fast_lz(&stream);
            
            fwrite(outbuf, 1, YASUO_BUF_LEN - stream.avail_out, dst);
            
        } while (stream.avail_out == 0);
        
        assert(stream.avail_in == 0);
    } while (! stream.end_of_stream);
    
    
#else
    memset(inbuf, 0, sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    strcpy(inbuf, "1234,abcd,ABCD,^#@!.!@#@!#@!$#$!#$SADJKAJDHWIOEQNNNLAJSDLJALJDQWJE(_@)#@~J#SLDerxcafsafas");
    
    printf("inbuf:%s\n", inbuf);
    stream.avail_in = strlen(inbuf);
    stream.next_in = outbuf;
    stream.end_of_stream = 0;
    
    stream.avail_out = BUF_LEN;
    stream.next_out = outbuf;
    fast_lz(&stream);
    
    dump_buffer(outbuf, BUF_LEN - stream.avail_out);
    printf("\n");
    printf("\n");
    
//    init_stream(&stream);
    memset(inbuf, 0, sizeof(inbuf));
    memset(outbuf, 0, sizeof(outbuf));
    strcpy(inbuf, "1234,abcd,ABCD,^#@!.!@#@!#@!$#$!#$SADJKAJDHWIOEQNNNLAJSDLJALJDQWJE(_@)#@~J#SLDerxcafsafas");
    
    printf("inbuf:%s\n", inbuf);
    stream.avail_in = strlen(inbuf);
    stream.next_in = outbuf;
    
    stream.avail_out = BUF_LEN;
    stream.next_out = outbuf;
    stream.end_of_stream = -1;
    fast_lz(&stream);
    
    dump_buffer(outbuf, BUF_LEN - stream.avail_out);
    
    printf("\n");
#endif
    
    
    fclose(src);
    fclose(dst);
    return 0;
}
