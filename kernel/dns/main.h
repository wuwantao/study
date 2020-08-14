#include <linux/init.h>
#include <linux/module.h>  
#include <linux/skbuff.h>
#include <linux/if_ether.h>  // ethhdr, 以太网 帧类型的定义都在此处.
#include <linux/ip.h>        // for iph
#include <linux/udp.h>        // for iph

#include <linux/list.h>
#include <linux/ctype.h>
#include <linux/netdevice.h>


extern int (*packet_recv_hook)(struct sk_buff *pskb);
extern int (*packet_send_hook)(struct sk_buff *pskb);
