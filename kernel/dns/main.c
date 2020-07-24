#include "main.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wuwantao");
MODULE_DESCRIPTION("dns module");


struct list_head 



int packet_recv_handle(struct sk_buff *skb)
{
    //  unsigned char *mac = skb->mac_header;
    struct ethhdr *    eth = NULL;
    struct iphdr *     iph = NULL;
    unsigned char *    p   = NULL;

    unsigned char *mac = eth->h_source;
    eth                = (struct ethhdr *)(skb->data - ETH_HLEN);

    iph = ip_hdr(skb);

    printk(KERN_ERR "proto:%0x\n", ntohs(eth->h_proto));

    //  printk("src:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    printk(KERN_ERR "src:%02x:%02x:%02x:%02x:%02x:%02x\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

    p = (unsigned char *)&iph->saddr;
    if (iph->version == 4) {
        printk(KERN_ERR "srcip: %d.%d.%d.%d\n", p[0], p[1], p[2], p[3]);
    }

    return 0;
}

static int __init hello_init(void)
{
    printk(KERN_ERR "hello world!\n");

    packet_frame_hook = packet_recv_handle;

    return 0;
}

static void __exit hello_exit(void)
{
    packet_frame_hook = 0;
    printk(KERN_EMERG "hello exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
