#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <zlib.h>

int dump_buffer(const uint8_t* buf, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    const char* inBuf = "1234,abcd,ABCD,^#@!.";
    uint8_t outBuf[1024] = {0};
    uint8_t restoreBuf[1024] = {0};
    int outLen = 0;
    int restoreLen = 0;
    int err = 0;
    z_stream stream;
    int fmt = 0; // 0: zlib; 1: gzip; 2: raw

    if (argc != 2) {
        printf(
        "./t [0-2]\n"
        "0 zlib\n"
        "1 gzip\n"
        "2 raw\n");
        return -1;
    }
   
   fmt = atoi(argv[1]); 

    printf("source string:%s\r\n", inBuf);

    // 压缩
    stream.next_in = (z_const uint8_t *)inBuf;
    stream.avail_in = (uInt)strlen(inBuf);

    stream.next_out = (uint8_t *)outBuf;
    stream.avail_out = 1024;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    if (0 == fmt) {
        // zlib
        err = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
        assert(Z_OK == err);

        err = deflate(&stream, Z_FINISH);
        assert(err == Z_STREAM_END);

        outLen = stream.total_out;

        err = deflateEnd(&stream);

        printf("zlib string(HEX):");
    } else if (1 == fmt) {
        // gzip
        err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
        assert(Z_OK == err);

        err = deflate(&stream, Z_FINISH);
        assert(err == Z_STREAM_END);

        outLen = stream.total_out;

        err = deflateEnd(&stream);

        printf("gzip string(HEX):");
    } else if (2 == fmt) {
        // raw
        err = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS * -1, 8, Z_DEFAULT_STRATEGY);
        assert(Z_OK == err);

        err = deflate(&stream, Z_FINISH);
        assert(err == Z_STREAM_END);

        outLen = stream.total_out;

        err = deflateEnd(&stream);

        printf("raw deflate string(HEX):");
    } else {
        assert(0);
    }

    dump_buffer(outBuf, outLen);
    printf("\r\n");

    // 解压缩
    stream.next_in = (z_const uint8_t *)outBuf;
    stream.avail_in = (uInt)outLen;

    stream.next_out = (uint8_t *)restoreBuf;
    stream.avail_out = 1024;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    if (0 == fmt) {
        // zlib
        err = inflateInit(&stream);
        assert(Z_OK == err);

        err = inflate(&stream, Z_FINISH);
        assert(err == Z_STREAM_END);

        restoreLen = stream.total_out;

        err = inflateEnd(&stream);
    } else if (1 == fmt) {
        // gzip
        err = inflateInit2(&stream, MAX_WBITS + 16);
        assert(Z_OK == err);

        err = inflate(&stream, Z_FINISH);
        assert(err == Z_STREAM_END);

        restoreLen = stream.total_out;

        err = inflateEnd(&stream);
    } else if (2 == fmt) {
        // raw
        err = inflateInit2(&stream, MAX_WBITS * -1);
        assert(Z_OK == err);

        err = inflate(&stream, Z_FINISH);
        assert(err == Z_STREAM_END);

        restoreLen = stream.total_out;

        err = inflateEnd(&stream);
    } else {
        assert(0);
    }

    printf("restored string:%s\r\n", (char*)restoreBuf);

    return err;
}
