#include "main.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wuwantao");
MODULE_DESCRIPTION("dns module");

#define DNAME_MAX_LEN 61
#define DNS_HASH_CNT  (1 << 20)

#define TRUST_HASH_BUCKETS_LONG 14340032

#define DOMAIN_TXT "/"

typedef struct {
    char dname[DNAME_MAX_LEN];
} dns_hash_key_t;

typedef struct {
    uint32_t ip;
} dns_hash_val_t;

typedef struct {
    struct hlist_node hnode;
    dns_hash_key_t    dns_hash_key;
    dns_hash_val_t    dns_hash_val;
} dns_hash_entry_t;

struct hlist_head dns_hash[DNS_HASH_CNT];

extern int dev_queue_xmit_old(struct sk_buff *skb, void *accel_priv);

atomic64_t send_cnt;

static inline unsigned int calc_hash_key(const unsigned char *dname, int dname_len, int buckets_long)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;
    char         tmp_char;

    int i = 0;
    while (i < dname_len) {
        tmp_char = tolower(dname[i]); //全部转成小写进行比较
        hash     = hash * seed + tmp_char;
        i++;
    }

    return (hash & 0x7FFFFFFF) % buckets_long;
}

static char *read_line(char *buf, int buf_len, struct file *fp)
{
    int          ret = 0;
    int          i   = 0;
    mm_segment_t fs;

    fs = get_fs();
    set_fs(KERNEL_DS);
    ret = fp->f_op->read(fp, buf, buf_len, &(fp->f_pos));
    set_fs(fs);

    if (ret <= 0)
        return NULL;

    while (buf[i++] != '\n' && i < ret)
        ;

    if (i < ret) {
        fp->f_pos += i - ret;
    }

    if (i < buf_len) {
        buf[i] = 0;
    }
    return buf;
}

static void add_cache_from_file()
{
    char               dname[DNAME_MAX_LEN];
    struct file *      file           = NULL;
    int                size           = 0;
    int                dname_len      = 0;
    int                key            = 0;
    struct hlist_head *h              = NULL;
    dns_hash_entry_t * dns_hash_entry = NULL;
    uint32_t           ip             = 0;

    file = filp_open(DOMAIN_TXT, O_RDWR | O_CREAT, 0644);
    if (IS_ERR(file)) {
        printk(KERN_ERR "error file_open =[%s]\n", DOMAIN_TXT);
        return;
    }

    while (read_line(dname, DNAME_MAX_LEN, file) != NULL) {
        dname_len = strlen(dname);
        key       = calc_hash_key(dname, dname_len, TRUST_HASH_BUCKETS_LONG);

        key = key % DNS_HASH_CNT;

        h = &dns_hash[key];

        dns_hash_entry = vmalloc(sizeof(dns_hash_entry_t));

        if (dns_hash_entry == NULL) {
            printk(KERN_ERR "malloc dns_hash_entry failed.");
            return;
        }
        memset(dns_hash_entry, 0, sizeof(dns_hash_entry_t));
        memcpy(dns_hash_entry->dns_hash_key.dname, dname, DNAME_MAX_LEN);
        dns_hash_entry->dns_hash_val.ip = ip++;
        hlist_add_head(&dns_hash_entry->hnode, h);
    }

    filp_close(file, 0);
}

static void dns_hash_init()
{
    struct hlist_head *h   = NULL;
    dns_hash_entry_t * pos = NULL;
    dns_hash_entry_t * n   = NULL;
    int                i   = 0;

    for (i = 0; i < DNS_HASH_CNT; i++) {
        h = &dns_hash[i];
        hlist_for_each_entry_safe(pos, n, h, hnode)
        {
            hlist_del(&pos->hnode);
            vfree(pos);
        }
    }
}

static void dns_hash_exit()
{
    int i = 0;

    for (i = 0; i < DNS_HASH_CNT; i++) {
        INIT_HLIST_HEAD(&dns_hash[i]);
    }
    add_cache_from_file();
}

int packet_recv(struct sk_buff *skb)
{
    //  unsigned char *mac = skb->mac_header;
    struct ethhdr * eth         = NULL;
    struct iphdr *  iph         = NULL;
    unsigned char * p           = NULL;
    
    struct sk_buff *rskb        = NULL;
    struct udphdr * udphdr      = NULL;
    struct iphdr *  iphdr       = NULL;
    struct ethhdr * ethhdr      = NULL;
    uint8_t         app_data[3] = {0x1, 0x2, 0x3};
    int rskb_len = 0;

    unsigned char *mac = eth->h_source;
    eth                = (struct ethhdr *)(skb->data - ETH_HLEN);

    iph = ip_hdr(skb);

    p = (unsigned char *)&iph->saddr;

    if (iph->version == 4) {
    }

    if (skb->dev->ifindex == 12) {
		rskb_len = 2 + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(app_data);
        rskb = alloc_skb(rskb_len, GFP_ATOMIC);
        if (unlikely(rskb == NULL)) {
            if (net_ratelimit())
                printk(KERN_ERR "alloc_skb error.\n");
            return -1;
        }
		rskb->dev = skb->dev;
        /* 保留skb区域 */
        skb_reserve(rskb, rskb_len);

        /* 构造数据区 */
        p = skb_push(rskb, sizeof(app_data));
        memcpy(p, &app_data[0], sizeof(app_data));
        p      = skb_push(rskb, sizeof(struct udphdr));
        udphdr = (struct udphdr *)p; // 填充udphdr字段，略
        udphdr->dest = ntohs(8888);
        udphdr->source = ntohs(6666);
        udphdr->len = 20;
        skb_reset_transport_header(rskb);
        /* 构造IP头 */
        p     = skb_push(rskb, sizeof(struct iphdr));
        iphdr = (struct iphdr *)p; // 填充iphdr字段，略
        skb_reset_network_header(rskb);
		iphdr->daddr = 1;
		iphdr->saddr = 2;
        /* 构造以太头 */
        p      = skb_push(rskb, sizeof(struct ethhdr));
        ethhdr = (struct ethhdr *)p; // 填充ethhdr字段，略
        skb_reset_mac_header(rskb);

        dev_queue_xmit_old(rskb, NULL);
        atomic64_inc(&send_cnt);
        if (net_ratelimit())
            printk(KERN_ERR "send_cnt:%lld.\n", send_cnt.counter);
    }

    return -1;
}

static int __init hello_init(void)
{
    printk(KERN_ERR "hello world!\n");

    packet_recv_hook = packet_recv;

    return 0;
}

static void __exit hello_exit(void)
{
    packet_recv_hook = 0;
    printk(KERN_EMERG "hello exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
