#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct rx {
    uint8_t  if_idx;            //从哪个网卡进入的网卡索引
    uint8_t  unicast;           //单播 or 广播
    uint8_t  buf_type;          //RX_CC or RX_DH4 or RX_DH6
    uint16_t buf_len;           //buf_len    
    uint32_t slot_id;           //slot_id
    uint8_t  *udp_data;         //udp层头指针
    uint8_t  buf[0];            //epoll or dpdk rx的buf
} rx_t;
/* *
 * ***#
 * **##
 * **** ####
 * **** ****
 * 共计 24 个字节 
 */



int main()
{
    printf("%d\n",sizeof(rx_t));
    return 0;
}
