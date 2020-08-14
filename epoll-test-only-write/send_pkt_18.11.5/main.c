/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/queue.h>
#include <unistd.h>

#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_memory.h>
#include <rte_per_lcore.h>

#include <rte_atomic.h>
#include <rte_branch_prediction.h>
#include <rte_byteorder.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_debug.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_interrupts.h>
#include <rte_ip.h>
#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_log.h>
#include <rte_mbuf.h>
#include <rte_memcpy.h>
#include <rte_memory.h>
#include <rte_mempool.h>
#include <rte_pci.h>
#include <rte_per_lcore.h>
#include <rte_tcp.h>
#include <rte_udp.h>

#define APP_DEFAULT_MEMPOOL_BUFFERS    8192 * 32
#define APP_DEFAULT_MEMPOOL_CACHE_SIZE 256

static struct rte_eth_conf port_conf = {
	.rxmode = {
			.max_rx_pkt_len = ETHER_MAX_LEN,
	},
};

struct rte_mbuf *send_array[256];
struct rte_mbuf *recv_array[256];
uint64_t         cnt = 0;

static void *get_lps_loop(__attribute__((unused)) void *arg)
{
    time_t   old_time;
    uint64_t old_cnt = 0;
    time_t   now;

    sleep(5);

    for (;;) {
        old_time = time(NULL);
        old_cnt  = cnt;
        sleep(2);
        now = time(NULL);
        printf("pps:%ld\n", (cnt - old_cnt) / ((uint64_t)(now - old_time)));
    }
    return NULL;
}

static int create_time_thread()
{
    int       ret = 0;
    pthread_t pid;

    ret = pthread_create(&pid, NULL, get_lps_loop, NULL);

    if (ret < 0) {
        printf("创建获取实时时间线程失败\n");
        return -1;
    }
    pthread_detach(pid);
    return 0;
}

static int send_loop(int port)
{
    int n_mbufs = 0;
    int i       = 0;
    //开始发送数据
    while (1) {
        n_mbufs = rte_eth_tx_burst(port, 0, send_array, 256);

        if (unlikely(n_mbufs == 0)) {
            continue;
        } else {
            cnt += n_mbufs;
            for (i = 0; i < n_mbufs; i++) {
                rte_mbuf_refcnt_update(send_array[i], 2);
            }
        }
    }
    return 0;
}

static int send_init(int port)
{
    unsigned                lcore_id = 0;
    struct rte_mbuf *       send_pkt = NULL;
    struct rte_mempool *    pool     = NULL;
    int                     socket   = 0;
    struct rte_eth_txconf   txq_conf;
    struct rte_eth_rxconf   rxq_conf;
    struct rte_eth_dev_info dev_info;
    struct rte_eth_conf     local_port_conf = port_conf;
    int                     ret;
    uint16_t                nic_tx_ring_size = 256;
    int                     i                = 0;
    struct ether_hdr        ethhdr;
    struct ipv4_hdr         iphdr;
    struct udp_hdr          udphdr;
    uint8_t t[100];

    memset(t, 1, sizeof(t));

    //创建监控pps线程
    create_time_thread();

    lcore_id = rte_lcore_id();
    printf("hello from core %u\n", lcore_id);

    //根据发送用的nic获取socket
    socket = rte_eth_dev_socket_id(port);

    //创建pool
    pool = rte_pktmbuf_pool_create("send_pkt", APP_DEFAULT_MEMPOOL_BUFFERS,
                                   APP_DEFAULT_MEMPOOL_CACHE_SIZE,
                                   0, 2048, socket);

    if (pool == NULL) {
        rte_panic("Cannot create mbuf pool on socket %u\n", socket);
    }

    //初始化nic
    /* Init port */
    printf("Initializing NIC port %u ...\n", port);
    rte_eth_dev_info_get(port, &dev_info);

    printf("capa:%0x\n", dev_info.tx_offload_capa);

	//DEV_TX_OFFLOAD_IPV4_CKSUM | DEV_TX_OFFLOAD_UDP_CKSUM,
//    if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_IPV4_CKSUM) {
//    	printf("DEV_TX_OFFLOAD_IPV4_CKSUM\n");
//    	local_port_conf.txmode.offloads |= DEV_TX_OFFLOAD_IPV4_CKSUM;
//    }

//    if (dev_info.tx_offload_capa & DEV_TX_OFFLOAD_UDP_CKSUM) {
//    	printf("添加DEV_TX_OFFLOAD_UDP_CKSUM\n");	    
//    	local_port_conf.txmode.offloads |= DEV_TX_OFFLOAD_UDP_CKSUM;
//    }    

    ret = rte_eth_dev_configure(
        port,
        1,
        1, //TODO
        &local_port_conf);

    rte_eth_promiscuous_enable(port);

    ret = rte_eth_dev_adjust_nb_rx_tx_desc(port, &nic_tx_ring_size, &nic_tx_ring_size);
    if (ret < 0) {
        rte_panic("Cannot adjust number of descriptors for port %u (%d)\n", port, ret);
    }

    //安装网卡发送队列
    txq_conf          = dev_info.default_txconf;
    txq_conf.offloads = local_port_conf.txmode.offloads;

    ret = rte_eth_tx_queue_setup(
        port,
        0,
        nic_tx_ring_size,
        socket,
        &txq_conf);
    if (ret < 0) {
        rte_panic("Cannot init TX queue %u for port %u (%d)\n", 0, port, ret);
    }        

    //安装网卡接收队列
    rxq_conf          = dev_info.default_rxconf;
    rxq_conf.offloads = local_port_conf.rxmode.offloads;
    ret               = rte_eth_rx_queue_setup(
        port,
        0,
        nic_tx_ring_size,
        socket,
        &rxq_conf, pool);
    if (ret < 0) {
        rte_panic("Cannot init TX queue %u for port %u (%d)\n", 0, port, ret);
    }

    //启动端口
    ret = rte_eth_dev_start(port);

    if (ret < 0) {
        rte_panic("Cannot start port %d (%d)\n", port, ret);
    }

    //b0:36:9f:aa:64:9d
    //f8:f2:1e:88:a6:f1
    ethhdr.d_addr.addr_bytes[0] = 0xf8;
    ethhdr.d_addr.addr_bytes[1] = 0xf2;
    ethhdr.d_addr.addr_bytes[2] = 0x1e;
    ethhdr.d_addr.addr_bytes[3] = 0x88;
    ethhdr.d_addr.addr_bytes[4] = 0xa6;
    ethhdr.d_addr.addr_bytes[5] = 0xf1;
    ethhdr.ether_type = 0x8;

	iphdr.version_ihl = 0x45;
	iphdr.type_of_service = 0;
	iphdr.packet_id = 0;
	iphdr.fragment_offset = 0;
	iphdr.total_length = 0x8000;
    iphdr.time_to_live = 50;
    iphdr.next_proto_id = 0x11;
    iphdr.hdr_checksum = 0x1a05;
    iphdr.src_addr = 0x0201a8c0;
    iphdr.dst_addr = 0x0101a8c0;

	udphdr.src_port = htons(8888);
    udphdr.dst_port = htons(60000);
    udphdr.dgram_len = htons(108);
    udphdr.dgram_cksum = 0;

    //构造mbuf
    for (i = 0; i < 256; i++) {
        send_pkt = rte_mbuf_raw_alloc(pool);
        if (send_pkt == NULL) {
            printf("on %s:%d no free mem\n", __func__, __LINE__);
            exit(0);
        }

        rte_pktmbuf_reset_headroom(send_pkt);
        send_pkt->data_len = (uint16_t)(0 + sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr))+100;
        send_pkt->next     = NULL;
        rte_memcpy(rte_pktmbuf_mtod_offset(send_pkt, char *, 0), &ethhdr, sizeof(struct ether_hdr));
        rte_memcpy(rte_pktmbuf_mtod_offset(send_pkt, char *, sizeof(struct ether_hdr)), &iphdr, sizeof(struct ipv4_hdr));
        rte_memcpy(rte_pktmbuf_mtod_offset(send_pkt, char *, sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr)), &udphdr, sizeof(struct udp_hdr));
        rte_memcpy(rte_pktmbuf_mtod_offset(send_pkt, char *, sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr)), t, sizeof(t));
        send_pkt->nb_segs        = 0;
        send_pkt->pkt_len        = (uint16_t)(0 + sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) + sizeof(struct udp_hdr))+100;
        send_pkt->ol_flags       = 0;
        send_pkt->vlan_tci       = 0;
        send_pkt->vlan_tci_outer = 0;
        send_pkt->ol_flags |= PKT_TX_IPV4 | PKT_TX_IP_CKSUM;
        send_array[i]            = send_pkt;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int ret  = 0;
    int port = 0;

    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_panic("Cannot init EAL\n");

    port = 0;
    send_init(port);
    send_loop(port);

    return 0;
}
