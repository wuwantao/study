#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#include "zlog.h"

int main(int argc, char *argv[])
{
    int rc;
    
    
    zlog_category_t *zc;
    
    rc = zlog_init("zlog.conf");
    
    if (rc) {
        printf("init failed\n");
        
        return -1;
        
    }
    
    zc = zlog_get_category("default");
    
    if (!zc) {
    
        printf("get cat fail\n");
        zlog_fini();
        
        return -2;
        
    }
    uint64_t i = 0;
    while (1) {
        zlog_info(zc, "%10d.hello, zlog", i++);
    }
    
    zlog_fini();  //释放zlog
    return 0;
}

