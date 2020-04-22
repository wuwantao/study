#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

//扩展操作(无需关心dst)
void test1()
{

    int8_t   src = 0xff;
    uint8_t  u_src = 0xff;
    uint16_t u_dst = 0;
    int16_t  dst = 0;

    //扩展操作(无需关心dst)
    u_dst = src;                    //符号扩展操作
    printf("u_dst:%d\n", u_dst);
    u_dst = u_src;                  //零扩展操作
    printf("u_dst:%d\n", u_dst);    


    dst = src;                    //符号扩展操作
    printf("dst:%d\n", dst);
    dst = u_src;                  //零扩展操作
    printf("dst:%d\n", dst);    


}

//截断操作(无需关心dst)
void test2()
{
    int16_t    src = 0xffff;
    uint16_t u_src = 0xffff;
    
    uint8_t  u_dst = 0;
    int8_t   dst = 0;

    
    u_dst = src;                    //符号扩展操作
    printf("u_dst:%d\n", u_dst);
    u_dst = u_src;                  //零扩展操作
    printf("u_dst:%d\n", u_dst);    

    dst = src;                    //符号扩展操作
    printf("dst:%d\n", dst);
    dst = u_src;                  //零扩展操作
    printf("dst:%d\n", dst);    


}


int main() 
{

//    test1();
    test2();

	return 1;
}
