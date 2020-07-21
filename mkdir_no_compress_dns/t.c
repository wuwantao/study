#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include <pcap.h>
#include <unistd.h>
#include <ldns/ldns.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <net/ethernet.h>
#include <linux/udp.h>

#define ETHER_HEADER_LENGTH 14
#define UDP_HEADER_LENGTH 8
#define IP6_HEADER_LENGTH 40

/* some systems don't have this? */
#ifndef ETHERTYPE_IPV6
#define ETHERTYPE_IPV6 0x86dd
#endif

uint8_t packet[2048];
uint32_t pkt_len = 0;

#define rte_be_to_cpu_16 ntohs

#define rte_cpu_to_be_16 htons


/**
 * Ethernet header: Contains the destination address, source address
 * and frame type.
 */
struct ether_hdr {
    struct ether_addr d_addr; /**< Destination address. */
    struct ether_addr s_addr; /**< Source address. */
    uint16_t ether_type;      /**< Frame type. */
} __attribute__((__packed__));


/**
 * IPv4 Header
 */
struct ipv4_hdr {
    uint8_t  version_ihl;		/**< version and header length */
    uint8_t  type_of_service;	/**< type of service */
    uint16_t total_length;		/**< length of packet */
    uint16_t packet_id;		/**< packet ID */
    uint16_t fragment_offset;	/**< fragmentation offset */
    uint8_t  time_to_live;		/**< time to live */
    uint8_t  next_proto_id;		/**< protocol ID */
    uint16_t hdr_checksum;		/**< header checksum */
    uint32_t src_addr;		/**< source address */
    uint32_t dst_addr;		/**< destination address */
} __attribute__((__packed__));


/**
 * UDP Header
 */
struct udp_hdr {
    uint16_t src_port;    /**< UDP source port. */
    uint16_t dst_port;    /**< UDP destination port. */
    uint16_t dgram_len;   /**< UDP datagram length */
    uint16_t dgram_cksum; /**< UDP datagram checksum */
} __attribute__((__packed__));

/**
 * @internal Calculate a sum of all words in the buffer.
 * Helper routine for the rte_raw_cksum().
 *
 * @param buf
 *   Pointer to the buffer.
 * @param len
 *   Length of the buffer.
 * @param sum
 *   Initial value of the sum.
 * @return
 *   sum += Sum of all words in the buffer.
 */
static inline uint32_t
__rte_raw_cksum(const void *buf, size_t len, uint32_t sum)
{
    /* workaround gcc strict-aliasing warning */
    uintptr_t ptr = (uintptr_t)buf;
    typedef uint16_t __attribute__((__may_alias__)) u16_p;
    const u16_p *u16_buf = (const u16_p *)ptr;
    
    while (len >= (sizeof(*u16_buf) * 4)) {
        sum += u16_buf[0];
        sum += u16_buf[1];
        sum += u16_buf[2];
        sum += u16_buf[3];
        len -= sizeof(*u16_buf) * 4;
        u16_buf += 4;
    }
    
    while (len >= sizeof(*u16_buf)) {
        sum += *u16_buf;
        len -= sizeof(*u16_buf);
        u16_buf += 1;
    }
    
    /* if length is in odd bytes */
    if (len == 1)
        sum += *((const uint8_t *)u16_buf);
        
    return sum;
}

/**
 * @internal Reduce a sum to the non-complemented checksum.
 * Helper routine for the rte_raw_cksum().
 *
 * @param sum
 *   Value of the sum.
 * @return
 *   The non-complemented checksum.
 */
static inline uint16_t
__rte_raw_cksum_reduce(uint32_t sum)
{
    sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
    sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
    return (uint16_t)sum;
}

/**
 * Process the non-complemented checksum of a buffer.
 *
 * @param buf
 *   Pointer to the buffer.
 * @param len
 *   Length of the buffer.
 * @return
 *   The non-complemented checksum.
 */
static inline uint16_t
rte_raw_cksum(const void *buf, size_t len)
{
    uint32_t sum;
    
    sum = __rte_raw_cksum(buf, len, 0);
    return __rte_raw_cksum_reduce(sum);
}

/**
 * Process the IPv4 checksum of an IPv4 header.
 *
 * The checksum field must be set to 0 by the caller.
 *
 * @param ipv4_hdr
 *   The pointer to the contiguous IPv4 header.
 * @return
 *   The complemented checksum to set in the IP packet.
 */
static inline uint16_t
rte_ipv4_cksum(const struct ipv4_hdr *ipv4_hdr)
{
    uint16_t cksum;
    cksum = rte_raw_cksum(ipv4_hdr, sizeof(struct ipv4_hdr));
    return (cksum == 0xffff) ? cksum : (uint16_t)~cksum;
}


/**
 * Process the pseudo-header checksum of an IPv4 header.
 *
 * The checksum field must be set to 0 by the caller.
 *
 * Depending on the ol_flags, the pseudo-header checksum expected by the
 * drivers is not the same. For instance, when TSO is enabled, the IP
 * payload length must not be included in the packet.
 *
 * When ol_flags is 0, it computes the standard pseudo-header checksum.
 *
 * @param ipv4_hdr
 *   The pointer to the contiguous IPv4 header.
 * @param ol_flags
 *   The ol_flags of the associated mbuf.
 * @return
 *   The non-complemented checksum to set in the L4 header.
 */
#define PKT_TX_TCP_SEG       (1ULL << 50)
static inline uint16_t
rte_ipv4_phdr_cksum(const struct ipv4_hdr *ipv4_hdr, uint64_t ol_flags)
{
    struct ipv4_psd_header {
        uint32_t src_addr; /* IP address of source host. */
        uint32_t dst_addr; /* IP address of destination host. */
        uint8_t  zero;     /* zero. */
        uint8_t  proto;    /* L4 protocol type. */
        uint16_t len;      /* L4 length. */
    } psd_hdr;
    
    psd_hdr.src_addr = ipv4_hdr->src_addr;
    psd_hdr.dst_addr = ipv4_hdr->dst_addr;
    psd_hdr.zero = 0;
    psd_hdr.proto = ipv4_hdr->next_proto_id;
    
    if (ol_flags & PKT_TX_TCP_SEG) {
        psd_hdr.len = 0;
    } else {
        psd_hdr.len = rte_cpu_to_be_16(
                          (uint16_t)(rte_be_to_cpu_16(ipv4_hdr->total_length)
                                     - sizeof(struct ipv4_hdr)));
    }
    
    return rte_raw_cksum(&psd_hdr, sizeof(psd_hdr));
}


/**
 * Process the IPv4 UDP or TCP checksum.
 *
 * The IPv4 header should not contains options. The IP and layer 4
 * checksum must be set to 0 in the packet by the caller.
 *
 * @param ipv4_hdr
 *   The pointer to the contiguous IPv4 header.
 * @param l4_hdr
 *   The pointer to the beginning of the L4 header.
 * @return
 *   The complemented checksum to set in the IP packet.
 */
static inline uint16_t
rte_ipv4_udptcp_cksum(const struct ipv4_hdr *ipv4_hdr, const void *l4_hdr)
{
    uint32_t cksum;
    uint32_t l4_len;
    
    l4_len = (uint32_t)(rte_be_to_cpu_16(ipv4_hdr->total_length) -
                        sizeof(struct ipv4_hdr));
                        
    cksum = rte_raw_cksum(l4_hdr, l4_len);
    cksum += rte_ipv4_phdr_cksum(ipv4_hdr, 0);
    
    cksum = ((cksum & 0xffff0000) >> 16) + (cksum & 0xffff);
    cksum = (~cksum) & 0xffff;
    
    if (cksum == 0)
        cksum = 0xffff;
        
    return (uint16_t)cksum;
}


bool send_dns(uint8_t *out_buf, uint32_t out_len)          //ok
{
    uint8_t *p = NULL;
    
    struct iphdr *iptr = NULL;
    struct udphdr *udp_hdr = NULL;
    struct ether_header *eptr = NULL;
    
    memset(packet, 0, sizeof(packet));
    
    eptr = (struct ether_header *)packet;
    iptr = (struct iphdr *)(packet + sizeof(struct ether_header));
    udp_hdr = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
    p = (uint8_t *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr));
    
    eptr->ether_dhost[0] = eptr->ether_dhost[1] = eptr->ether_dhost[2] = eptr->ether_dhost[3] = eptr->ether_dhost[4] = eptr->ether_dhost[5] = 0x11;
    eptr->ether_shost[0] = eptr->ether_shost[1] = eptr->ether_shost[2] = eptr->ether_shost[3] = eptr->ether_shost[4] = eptr->ether_shost[5] = 0x22;
    eptr->ether_type = 0x8;

    
    eptr->ether_type = 0x8;
    
    iptr->version = 4;
    iptr->ihl     = 5;
    iptr->tos     = 0;
    iptr->tot_len = htons(out_len + sizeof(struct iphdr) + sizeof(struct udphdr));
    iptr->id      = 1;
    iptr->frag_off = 0;
    iptr->ttl     = 60;
    iptr->protocol = 17;
    iptr->check   = rte_ipv4_cksum((const struct ipv4_hdr *)iptr);
    iptr->saddr = 0x0100a8c0;
    iptr->daddr = 0x01000a0a;

    
    udp_hdr->source = 0x3500;
    udp_hdr->dest   = 0x5cc6;
    udp_hdr->len    = htons(out_len + sizeof(struct udphdr));
    udp_hdr->check  = rte_ipv4_udptcp_cksum((const struct ipv4_hdr *)iptr, (const void *)udp_hdr);
    
    if (out_len + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr) > sizeof(packet)) {
        printf("can't > packet'len\n");
        return false;
    }
    
    memcpy(p, out_buf, out_len);
    pkt_len = out_len + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr);
    
    return true;
}


bool send_dns3(uint8_t *out_buf, uint32_t out_len)          //ok
{
    uint8_t *p = NULL;
    
    struct iphdr *iptr = NULL;
    struct udphdr *udp_hdr = NULL;
    struct ether_header *eptr = NULL;
    
    memset(packet, 0, sizeof(packet));
    
    eptr = (struct ether_header *)packet;
    iptr = (struct iphdr *)(packet + sizeof(struct ether_header));
    udp_hdr = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
    p = (uint8_t *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr));
    
    eptr->ether_dhost[0] = 0x8c;
    eptr->ether_dhost[1] = 0xec;
    eptr->ether_dhost[2] = 0x4b;
    eptr->ether_dhost[3] = 0x5c;
    eptr->ether_dhost[4] = 0xae;
    eptr->ether_dhost[5] = 0xed;
    
    eptr->ether_shost[0] = 0xe4;
    eptr->ether_shost[1] = 0x68;
    eptr->ether_shost[2] = 0xa3;
    eptr->ether_shost[3] = 0x4f;
    eptr->ether_shost[4] = 0xf2;
    eptr->ether_shost[5] = 0x32;
    
    eptr->ether_type = 0x8;
    
    iptr->version = 4;
    iptr->ihl     = 5;
    iptr->tos     = 0;
    iptr->tot_len = htons(out_len + sizeof(struct iphdr) + sizeof(struct udphdr));
    iptr->id      = 1;
    iptr->frag_off = 0;
    iptr->ttl     = 60;
    iptr->protocol = 17;
    iptr->check   = 0;

    iptr->saddr = 0x1e05a8c0;
    iptr->daddr = 0xf188a8c0;
    
    udp_hdr->source = 0x3500;
    udp_hdr->dest   = 0x5cc6;
    udp_hdr->len    = htons(out_len + sizeof(struct udphdr));
    udp_hdr->check  = 0;
    
    if (out_len + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr) > sizeof(packet)) {
        printf("can't > packet'len\n");
        return false;
    }
    
    memcpy(p, out_buf, out_len);
    pkt_len = out_len + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr);
    
    return true;
}

bool send_dns2(uint8_t *out_buf, uint32_t out_len)          //ok
{
    uint8_t *p = NULL;
    
    struct iphdr *iptr = NULL;
    struct udphdr *udp_hdr = NULL;
    struct ether_header *eptr = NULL;
    
    memset(packet, 0, sizeof(packet));
    
    eptr = (struct ether_header *)packet;
    iptr = (struct iphdr *)(packet + sizeof(struct ether_header));
    udp_hdr = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
    p = (uint8_t *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr));
    
    eptr->ether_dhost[0] = 0x8c;
    eptr->ether_dhost[1] = 0xec;
    eptr->ether_dhost[2] = 0x4b;
    eptr->ether_dhost[3] = 0x5c;
    eptr->ether_dhost[4] = 0xae;
    eptr->ether_dhost[5] = 0xed;
    
    eptr->ether_shost[0] = 0xe4;
    eptr->ether_shost[1] = 0x68;
    eptr->ether_shost[2] = 0xa3;
    eptr->ether_shost[3] = 0x4f;
    eptr->ether_shost[4] = 0xf2;
    eptr->ether_shost[5] = 0x32;
    
    eptr->ether_type = 0x8;
    
    iptr->version = 4;
    iptr->ihl     = 5;
    iptr->tos     = 0;
    iptr->tot_len = htons(out_len + sizeof(struct iphdr) + sizeof(struct udphdr));
    iptr->id      = 1;
    iptr->frag_off = 0;
    iptr->ttl     = 60;
    iptr->protocol = 17;
//    iptr->check   = 0xa59c;//rte_ipv4_cksum((const struct ipv4_hdr *)iptr);

    iptr->saddr = 0x1e05a8c0;
    iptr->daddr = 0xf188a8c0;
    
    udp_hdr->source = 0x3500;
    udp_hdr->dest   = 0x5cc6;
    udp_hdr->len    = htons(out_len + sizeof(struct udphdr));
    udp_hdr->check  = rte_ipv4_udptcp_cksum((const struct ipv4_hdr *)iptr, (const void *)udp_hdr);
    
    if (out_len + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr) > sizeof(packet)) {
        printf("can't > packet'len\n");
        return false;
    }
    
    memcpy(p, out_buf, out_len);
    pkt_len = out_len + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr);
    
    return true;
}

ldns_rr * create_subnet_rr(void){
	static uint32_t ip = 0;
	ldns_rr * edns_rr = ldns_rr_new();
	ldns_rdf * option_code = ldns_native2rdf_int16(LDNS_RDF_TYPE_INT16, 8);
	ldns_rr_push_rdf(edns_rr, option_code);

	ldns_rdf * option_len = ldns_native2rdf_int16(LDNS_RDF_TYPE_INT16, 8);
	ldns_rr_push_rdf(edns_rr, option_len);
	
	ldns_rdf * family = ldns_native2rdf_int16(LDNS_RDF_TYPE_INT16, 1);
	ldns_rr_push_rdf(edns_rr, family);
	
	ldns_rdf * source_netmask = ldns_native2rdf_int8(LDNS_RDF_TYPE_INT8, 32);
	ldns_rr_push_rdf(edns_rr, source_netmask);

	ldns_rdf * scope_netmask = ldns_native2rdf_int8(LDNS_RDF_TYPE_INT8, 0);
	ldns_rr_push_rdf(edns_rr,scope_netmask);

	ldns_rdf * client_subnet = ldns_native2rdf_int32(LDNS_RDF_TYPE_INT32,ip++);

	ldns_rr_push_rdf(edns_rr,client_subnet);
	
	ldns_rr_set_type(edns_rr, LDNS_RR_TYPE_OPT);
	edns_rr->_rr_class = 4096;
	ldns_rr_set_ttl(edns_rr, 0);
	ldns_rdf *edns_name ;
	ldns_str2rdf_dname(&edns_name,".");
	ldns_rr_set_owner(edns_rr, edns_name);
	return edns_rr;
}


bool mkdir_dns(const uint8_t *dnspkt, int dnspkt_len, uint8_t **out_buf, uint32_t *out_len)
{
    ldns_pkt *in_pkt = NULL;
    ldns_rr_list *info = NULL;
    ldns_status status = LDNS_STATUS_OK;

    
    
    status = ldns_wire2pkt(&in_pkt, dnspkt, dnspkt_len);
    
    if (status != LDNS_STATUS_OK) {
        printf("Error creating ldns_pkt: %s\n", ldns_get_errorstr_by_id(status));
        goto FAIL;
    }


	/*构造edns*/
	ldns_rr *edns_rr = create_subnet_rr();
	ldns_pkt_push_rr(in_pkt, LDNS_SECTION_ADDITIONAL, edns_rr);    

    status = ldns_pkt2wire(out_buf, in_pkt, (size_t *)out_len);
    
    if (status != LDNS_STATUS_OK) {
        printf("Error creating out_buf: %s\n", ldns_get_errorstr_by_id(status));
        goto FAIL;
    }
    
    if (in_pkt != NULL) {
        ldns_pkt_free(in_pkt);
    }
    
    return true;
    
FAIL:

    if (in_pkt != NULL) {
        ldns_pkt_free(in_pkt);
        in_pkt = NULL;
    }
    
    if (*out_buf != NULL) {
        LDNS_FREE(*out_buf);
        *out_buf = NULL;
    }
    
    return false;
}

bool handle_ether_packet(const uint8_t *data, struct pcap_pkthdr cur_hdr)
{
    int dnspkt_len = 0;
    uint32_t out_len = 0;
    uint16_t ip_flags = 0;
    uint8_t *out_buf = NULL;
    struct iphdr *iptr = NULL;
    const uint8_t *dnspkt = NULL;
    struct ip6_hdr *ip6_hdr = NULL;
    struct ether_header *eptr = NULL;
    
    eptr = (struct ether_header *) data;
    
    if (ntohs (eptr->ether_type) != ETHERTYPE_IP && ntohs (eptr->ether_type) != ETHERTYPE_IPV6) {
        return true;
    }
    
    iptr = (struct iphdr *) (data + ETHER_HEADER_LENGTH);
    ip_flags = ldns_read_uint16(&(iptr->frag_off));
    
    if (ntohs (eptr->ether_type) == ETHERTYPE_IP) {         //处理IPV4
        if (ip_flags & IP_MF) {                             //分片不处理
            return true;
        }
        
        if ((uint8_t) iptr->protocol != IPPROTO_UDP) {          //非UDP不处理
            return true;
        }
        
        dnspkt = data + ETHER_HEADER_LENGTH + ((int) iptr->ihl * 4) + UDP_HEADER_LENGTH;
        dnspkt_len = cur_hdr.caplen - ETHER_HEADER_LENGTH - ((int) iptr->ihl * 4) - UDP_HEADER_LENGTH;
    }
    
    if (ntohs (eptr->ether_type) == ETHERTYPE_IPV6) {       //处理IPV6
    
        ip6_hdr = (struct ip6_hdr *) (data + ETHER_HEADER_LENGTH);
        
        if ((uint8_t) ip6_hdr->ip6_ctlun.ip6_un1.ip6_un1_nxt != IPPROTO_UDP) {
            return true;
        }
        
        dnspkt = data + ETHER_HEADER_LENGTH + IP6_HEADER_LENGTH + UDP_HEADER_LENGTH;
        dnspkt_len = cur_hdr.caplen - ETHER_HEADER_LENGTH - IP6_HEADER_LENGTH - UDP_HEADER_LENGTH;
        
    }
    
    if (mkdir_dns(dnspkt, dnspkt_len, &out_buf, &out_len) != true) {
        printf("mkdir_dns failed.\n");
        return false;
    }
    
    if (send_dns(out_buf, out_len) != true) {
        printf("send_send failed.\n");
        return false;
    }
    
    LDNS_FREE(out_buf);
    return true;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <netpacket/packet.h>
#include <net/if.h>
#include <net/ethernet.h>


int raw_send(uint8_t *data, uint32_t data_len)
{
    int skfd, n;
    struct sockaddr_ll toaddr;
    struct ifreq ifr;
    
    if (0 > (skfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_UDP))) {
        perror("Create Error");
        exit(1);
    }
    
    bzero(&toaddr, sizeof(toaddr));
    bzero(&ifr, sizeof(ifr));
    strcpy(ifr.ifr_name, "enp59s0f3");
    
    //获取接口索引
    if (-1 == ioctl(skfd, SIOCGIFINDEX, &ifr)) {
        perror("get dev index error:");
        exit(1);
    }
    
    toaddr.sll_ifindex = ifr.ifr_ifindex;
    printf("interface Index:%d\n", ifr.ifr_ifindex);
    
    
    toaddr.sll_family = PF_PACKET;
    n = sendto(skfd, data, data_len, 0, (struct sockaddr*)&toaddr, sizeof(toaddr));
    printf("sendto n:%d\n", n);
    
    
    close(skfd);
    return 0;
}


int main()
{
    int ret = 0;
    char *output_dev = "eno1";
    char *infile_str = "./taobao.pcap";
    int promisc = 1;
    int timeout = 0;
    int snaplen = 2048;
    pcap_t *out_dev = NULL;
    pcap_t *in_dev = NULL;
    struct pcap_pkthdr in_packet;
    const uint8_t *pktStr = NULL;
    char errBuf[PCAP_ERRBUF_SIZE];
    
    in_dev = pcap_open_offline(infile_str, errBuf);
    
    if (in_dev == NULL) {
        printf("error: pcap_open_offline:%s\n", errBuf);
        return -1;
    }
    
    while (1) {
        pktStr = pcap_next(in_dev, &in_packet);
        
        if (pktStr == NULL) {
            printf("did not capture a packet!\n");
            return -1;
        }
        
        break;
    }
    
    handle_ether_packet(pktStr, in_packet);
    
    out_dev = pcap_open_live("eno1", snaplen, promisc, timeout, errBuf);
    
    if (out_dev == NULL) {
        printf("error: pcap_open_live(): %s\n", errBuf);
        return -1;
    }
    
    while (1) {

//        raw_send(packet, pkt_len);
        ret = pcap_inject(out_dev, (const u_char *)packet, pkt_len);
        printf("ret:%d, out_len:%d error:%s\n", ret, pkt_len);
        
        
//        ret = pcap_sendpacket(out_dev, (const u_char *)pktStr, in_packet.caplen);
//        printf("ret:%d, out_len:%d\n", ret, in_packet.caplen);

        sleep(2);
    }
    
    pcap_close(in_dev);
    pcap_close(out_dev);
    return 1;
}
