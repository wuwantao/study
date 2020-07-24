#include <linux/init.h>
#include <linux/module.h>  
#include <linux/skbuff.h>
#include <linux/if_ether.h>  // ethhdr, 以太网 帧类型的定义都在此处.
#include <linux/ip.h>        // for iph
#include <linux/list.h>


extern int (*packet_frame_hook)(struct sk_buff *pskb);
extern int (*packet_send_hook)(struct sk_buff *pskb);
