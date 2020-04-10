#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <pcap.h>

#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>



#include "nameser_compat.h"

#define IPV4 4
#define IPV6 6
#define LOG_MAX_LEN 1024
#define TIME_STR_LEN 21
typedef struct dnshdr dnshdr_t;
struct timeval g_time;
char fields_str[2][2] = {"0", "1"};
char qtype_str[255][8];
char rcode_str[16][8] = {
    "0", "1", "2", "3", "4", "5", "6",  "7",
    "8", "9", "10", "11", "12", "13", "14", "15"
};
char opcode_str[6][2] = {"0", "1", "2", "3", "4", "5"};



#define IP2STR(x, y, z) _ip2str((x), (y), (z))

void init_str()
{
    strcpy(qtype_str[1], "1");
    strcpy(qtype_str[2], "2");
    strcpy(qtype_str[5], "5");
    strcpy(qtype_str[6], "6");
    strcpy(qtype_str[12], "12");
    strcpy(qtype_str[15], "15");
    strcpy(qtype_str[16], "16");
    strcpy(qtype_str[28], "28");
    strcpy(qtype_str[255], "255");
}

static inline char *_ip2str(char *str, int str_len, struct sockaddr_storage *ip)
{
    if (!str || !ip || str_len < 1) {
        return NULL;
    }
    
    if (ip->ss_family == AF_INET) {
        struct sockaddr_in *_ip = (struct sockaddr_in*)ip;
        inet_ntop(AF_INET, &_ip->sin_addr, str, str_len);
    } else if (ip->ss_family == AF_INET6) {
        struct sockaddr_in6 *_ip = (struct sockaddr_in6*)ip;
        inet_ntop(AF_INET6, &_ip->sin6_addr, str, str_len);
    } else {
        str[0] = 0;
    }
    
    return str;
}

int dump_buffer(uint8_t* buf, size_t len)
{
    size_t i = 0;
    
    for (i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
    }
    
    return 0;
}

#define ADD_SPLIT \
*d = '|';\
d += 1;

#define ADD_IPSTR(x, y, len)\
if (y == IPV4){\
    addr.ss_family = AF_INET;\
    memcpy(&((struct sockaddr_in *)(&addr))->sin_addr.s_addr, x, len);\
} else{\
    addr.ss_family = AF_INET6;\
    memcpy(&((struct sockaddr_in6 *)(&addr))->sin6_addr, x, len);\
}\
IP2STR(ip_str, sizeof(ip_str), &addr);\
tmp_len = strlen(ip_str);\
memcpy(d, ip_str, tmp_len);\
d += tmp_len;\
ADD_SPLIT;


#define ADD_NUM(x)\
sprintf(num_str, "%d", x);\
tmp_len = strlen(num_str);\
memcpy(d, num_str, tmp_len);\
d += tmp_len;\
ADD_SPLIT;

#define ADD_STR(x)\
tmp_len = strlen(x);\
memcpy(d, x, tmp_len);\
d += tmp_len;\
ADD_SPLIT;


#define ADD_DOMAIN(x) \
    t= NULL;\
if (((uint8_t)*x & 0xc0) == 0xc0) {\
    t = x + 2;\
    x = (const uint8_t *)dnshdr + (((uint16_t)ntohs(*(uint16_t *)x))&0x3FF);\
} else {\
    t = NULL;\
}\
while (*x != 0){ \
    memcpy(d, x + 1, *x); \
    d += *x;\
    x += 1 + *x;\
    if (*x != 0) {\
        *d = '.';\
        d += 1;\
    }\
}\
x +=1;\
if (t != NULL) {\
    x = t;\
}\
ADD_SPLIT;


bool add_domain(const uint8_t **ss, uint8_t **dd, dnshdr_t *dnshdr)
{
    const uint8_t *t = NULL;
    const uint8_t *s = *ss;
    uint8_t *d = *dd;
    if (*s == 0) {
        s += 1;
        *d = '.';
        d += 1;        
        return true;
    }
    
    if (((uint8_t)*s & 0xc0) == 0xc0) {
        t = s + 2;
        s = (const uint8_t *)dnshdr + (((uint16_t)ntohs(*(uint16_t *)s))&0x3FF);
    } else {
        t = NULL;
    }
    while (*s != 0){
        
        if ((((uint8_t)*s & 0xc0) == 0xc0)) {
            add_domain((uint8_t **)&s, &d, dnshdr);
            break;
        }
        
        memcpy(d, s + 1, *s); 
        d += *s;
        s += 1 + *s;
        if (*s != 0) {
            *d = '.';
            d += 1;
        }
    }
    if (*s == 0) {
        s += 1;
    }
    if (t != NULL) {
        s = t;
    }
    *ss = s;
    *dd = d;
    return true;
}


int dns_parse(const uint8_t *pkt, uint16_t pkt_len, uint8_t *dns_log)
{
    const uint8_t *s = pkt;
    uint8_t *d = dns_log;
    dnshdr_t *dnshdr = NULL;
    char time_str[TIME_STR_LEN + 1] = "20150213090616.902314|";
    char ip_str[64];
    char num_str[64];
    struct ethhdr *ethhdr = NULL;
    struct iphdr *iphdr = NULL;
    struct udphdr *udphdr = NULL;
    uint8_t tmp_len = 0;
    uint8_t *qr = NULL;
    
    uint16_t i = 0;
    uint16_t qdcount = 0;
    uint16_t ancount = 0;
    uint16_t nscount = 0;
    uint16_t arcount = 0;
    uint8_t qtype = 0;
    
    const uint8_t *domain = NULL;
    uint16_t dns_class = 0;
    uint16_t data_len = 0;
    const uint8_t *t = NULL;
    
    struct sockaddr_storage addr;
    
    //add time
    memcpy(d, time_str, TIME_STR_LEN + 1);
    d += TIME_STR_LEN + 1;
    
    
    //step ether
    ethhdr = (struct ethhdr *)s;
    s += sizeof(struct ethhdr);
    
    //step iphdr
    iphdr = (struct iphdr *)s;
    s += sizeof(struct iphdr);
    
    //step udphdr
    udphdr = (struct udphdr *)s;
    qr = d;
    d += 1;
    ADD_SPLIT;
    ADD_IPSTR(&iphdr->saddr, IPV4, 4);
    ADD_NUM(ntohs(udphdr->source));
    ADD_IPSTR(&iphdr->daddr, IPV4, 4);
    ADD_NUM(ntohs(udphdr->dest));
    s += sizeof(struct udphdr);
    
    //step dnshdr
    dnshdr = (dnshdr_t *)s;
    *qr = fields_str[dnshdr->qr][0];
    s += sizeof(dnshdr_t);
    
    //step query
    add_domain(&s, &d, dnshdr);
    ADD_SPLIT;
    qtype = ntohs(*(uint16_t *)s);
    s += 2;
    dns_class = ntohs(*(uint16_t *)s);
    s += 2;
    
    ADD_STR(qtype_str[qtype]);
    ADD_NUM(ntohs(dnshdr->id));
    ADD_STR(rcode_str[dnshdr->rcode]);     //0
    ADD_STR(opcode_str[dnshdr->opcode]);   //0
    ADD_STR(fields_str[dnshdr->aa]);       //0
    ADD_STR(fields_str[dnshdr->tc]);       //0
    ADD_STR(fields_str[dnshdr->rd]);       //1
    ADD_STR(fields_str[dnshdr->ra]);       //1
    
    qdcount = ntohs(dnshdr->qdcount);      //1
    ancount = ntohs(dnshdr->ancount);      //3
    nscount = ntohs(dnshdr->nscount);      //6
    arcount = ntohs(dnshdr->arcount);      //8
    
    ADD_NUM(qdcount);
    ADD_NUM(ancount);
    ADD_NUM(nscount);
    ADD_NUM(arcount);
    
    //ADD CLASS
    ADD_NUM(dns_class);                     //1
    
    
    //ADD answers ancount + nscount + arcount
    for (i = 0; i < ancount + nscount + arcount; ++i) {

        *d = '[';
        d += 1;
        //ADD DOMAIN
        add_domain(&s, &d, dnshdr);
        ADD_SPLIT;
        //ADD QTYPE
        qtype = ntohs(*(uint16_t *)s);
        ADD_NUM(qtype);
        s += 2;
        //step class
        dns_class = ntohs(*(uint16_t *)s);
        s += 2;
        //ADD TTL
        ADD_NUM(ntohl(*(uint64_t *)s));
        s += 4;
        //step data len
        data_len  = ntohs(*(uint16_t *)s);
        s += 2;
        
        switch (qtype) {
            case 1:     //A
                ADD_IPSTR(s, IPV4, 4);
                d -= 1; //去掉|
                s += 4;
                break;
                
            case 2:     //NS
                add_domain(&s, &d, dnshdr);
                break;
                
            case 5:     //CNAME
                add_domain(&s, &d, dnshdr);
                break;
                
            case 6:     //SOA
                break;
                
            case 12:    //PTR
                break;
                
            case 15:    //MX
                break;
                
            case 16:    //TXT
                break;
                
            case 28:    //AAAA
                ADD_IPSTR(s, IPV6, data_len);
                d -= 1; //去掉|
                s += data_len;
                break;
                
            case 255:   //ANY
                break;
                
            default:
                printf("unknown qtype:%d\n", qtype);
                return false;
        }
        if ((i + 1) < ancount + nscount + arcount) {
            *d = ']';
            d += 1;
            *d = '|';
            d += 1;
        } else {
            *d = ']';
            d += 1;
            
        }
    }
    *d = 0;
    printf("dns_log:%s\n", dns_log);
    
    if ((s - pkt) > pkt_len) {
        printf("解析报文越界\n");
        return false;
    }
    
    if (d - dns_log > LOG_MAX_LEN) {
        printf("dns_log 空间不够\n");
        return false;
    }
    
    return true;
}

int main(int argc, char *argv[])
{
    int a = 0;
    a = ntohs(0x1122);

    exit(0);
    char fname[100];
    char errBuf[PCAP_ERRBUF_SIZE], *devStr;
    pcap_t *device = NULL;
    struct pcap_pkthdr packet;
    const uint8_t *pktStr = NULL;
    uint8_t dns_log[LOG_MAX_LEN];
    init_str();
    
    if (argc != 2) {
        printf("./t dns.pcap\n");
        strcpy(fname, "dns.pcap");
    } else {
        strcpy(fname, argv[1]);
    }
    
    gettimeofday(&g_time, NULL);
    
    device = pcap_open_offline(fname, errBuf);
    
    if (device == NULL) {
        printf("error: pcap_open_live(): %s\n", errBuf);
        return -1;
    }
    
    
    while (1) {
        pktStr = pcap_next(device, &packet);
        
        if (pktStr == NULL) {
            printf("did not capture a packet!\n");
            return -1;
        }
        
#if DEBUG_PACK_LEN
        printf("Packet length: %d\n", packet.len);
        printf("Number of bytes: %d\n", packet.caplen);
        printf("Recieved time: %s\n", ctime((const time_t *)&packet.ts.tv_sec));
#endif
        if (dns_parse(pktStr, packet.len, dns_log) == false) {
            printf("dns_parse failed.\n");
            return false;
        }
    }
    
    return 0;
}
