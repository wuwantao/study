#include <arpa/inet.h>
#include <asm/types.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/route.h>
#include <linux/rtnetlink.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFLEN 20480
#define FORMAT "%-35s"

#define FORMAT_CMD "%-18s"

#define COLOR_NONE        "\033[0m"
#define FONT_COLOR_RED    "\033[0;31m"
#define FONT_COLOR_BLUE   "\033[1;34m"
#define FONT_COLOR_YELLOW "\033[1;33m"

#define BACKGROUND_COLOR_RED "\033[41m"

#define BG_RED_FONT_YELLOW "\033[41;33m"

#define t_assert(x)          \
    {                        \
        if (!(x)) {          \
            err = -__LINE__; \
            goto error;      \
        }                    \
    }

/*Ctrl + C 退出*/
static volatile int keepRunning = 1;

void intHandler(int dummy) 
{ 
    keepRunning = 0; 
    exit(0);
}

/**
 *  * 解析RTA,并存入tb
 *   */
void parse_rtattr(struct rtattr **tb, int max, struct rtattr *attr, int len)
{
    for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len)) {
        if (attr->rta_type <= max) {
            tb[attr->rta_type] = attr;
        }
    }
}

/**
 *  * 显示连接信息
 *   * 当网卡变动的时候触发这个信息,例如插/拔网线,增/减网卡设备,启用/禁用接口等.
 *    */
void print_ifinfomsg(struct nlmsghdr *h)
{
    int               len;
    struct rtattr *   tb[IFLA_MAX + 1];
    struct ifinfomsg *ifinfo;
    char              ifname[256];
    char              tmp[256];
    uint8_t *mac = NULL;

    printf(FONT_COLOR_RED);

    bzero(tb, sizeof(tb));
    ifinfo = NLMSG_DATA(h);
    len    = h->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));
    parse_rtattr(tb, IFLA_MAX, IFLA_RTA(ifinfo), len);
    if (h->nlmsg_type == RTM_NEWLINK) {
        printf(FORMAT_CMD, "RTM_NEWLINK");
    }

    if (h->nlmsg_type == RTM_DELLINK) {
        printf(FORMAT_CMD, "RTM_DELLINK");
    }

    if (h->nlmsg_type == RTM_GETLINK) {
        printf(FORMAT_CMD, "RTM_GETLINK");
    }

    if (tb[IFLA_IFNAME]) {
        sprintf(tmp, "ifname: %s", RTA_DATA(tb[IFLA_IFNAME]));
        printf(FORMAT, tmp);
    }

    sprintf(tmp, "link state: %s ", (ifinfo->ifi_flags & IFF_UP) && !(ifinfo->ifi_flags & IFF_RUNNING) ? "NO-CARRIER" : "running");
    printf(FORMAT, tmp);

    sprintf(tmp, "link state: %s ", (ifinfo->ifi_flags & IFF_UP) ? "up" : "down");
    printf(FORMAT, tmp);

    if (tb[IFLA_ADDRESS]) {
    	mac = (uint8_t *)(RTA_DATA(tb[IFLA_ADDRESS]));
        sprintf(tmp, "mac: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        printf(FORMAT, tmp);
    }

    if (tb[IFLA_MTU]) {
        sprintf(tmp, "mtu: %u", *(uint32_t *)(RTA_DATA(tb[IFLA_MTU])));
        printf(FORMAT, tmp);
    }

    if (tb[IFLA_LINK]) {
        sprintf(tmp, "link: %u", *(uint32_t *)RTA_DATA(tb[IFLA_LINK]));
        printf(FORMAT, tmp);
    }

    if (tb[IFLA_MASTER]) {
        if_indextoname(*(uint32_t *)RTA_DATA(tb[IFLA_MASTER]), ifname);
        sprintf(tmp, "master: %s", ifname);
        printf(FORMAT, tmp);
    }

	

    printf("\n");
    printf(COLOR_NONE);
}

struct {
    char str[20];
} ipaddr_flags[0x81] = {
    [0x01] = "IFA_F_SECONDARY",
    [0x01] = "IFA_F_TEMPORARY",
    [0x02] = "IFA_F_NODAD",
    [0x04] = "IFA_F_OPTIMISTIC",
    [0x08] = "IFA_F_DADFAILED",
    [0x10] = "IFA_F_HOMEADDRESS",
    [0x20] = "IFA_F_DEPRECATED",
    [0x40] = "IFA_F_TENTATIVE",
    [0x80] = "IFA_F_PERMANENT"};

struct {
    char str[20];
} ipaddr_scope[256] = {
	[0] = "RT_SCOPE_UNIVERSE",
    [200] = "RT_SCOPE_SITE",
    [253] = "RT_SCOPE_LINK",
    [254] = "RT_SCOPE_HOST",
    [255] = "RT_SCOPE_NOWHERE"};

struct {
    char str[20];
} rt_protocol[5] = {
    "RTPROT_UNSPEC",
    "RTPROT_REDIRECT",
    "RTPROT_KERNEL",
    "RTPROT_BOOT",
    "RTPROT_STATIC"};

struct {
    char str[20];
} rt_scope[256] = {
    [0]   = "RT_SCOPE_UNIVERSE",
    [200] = "RT_SCOPE_SITE",
    [253] = "RT_SCOPE_LINK",
    [254] = "RT_SCOPE_HOST",
    [255] = "RT_SCOPE_NOWHERE"};

struct {
    char str[20];
} rt_type[50] = {
    "RTN_UNSPEC",
    "RTN_UNICAST",
    "RTN_LOCAL",
    "RTN_BROADCAST",
    "RTN_ANYCAST",
    "RTN_MULTICAST",
    "RTN_BLACKHOLE",
    "RTN_UNREACHABLE",
    "RTN_PROHIBIT",
    "RTN_THROW",
    "RTN_NAT",
    "RTN_XRESOLVE"};

struct {
    char str[20];
} rt_table[256] = {
    [0]   = "RT_TABLE_UNSPEC",
    [252] = "RT_TABLE_COMPAT",
    [253] = "RT_TABLE_DEFAULT",
    [254] = "RT_TABLE_MAIN",
    [255] = "RT_TABLE_LOCAL"};
struct {
    char str[20];
} neigh_state[0xff] = {
    [0x01] = "NUD_INCOMPLETE",
    [0x02] = "NUD_REACHABLE",
    [0x04] = "NUD_STALE",
    [0x08] = "NUD_DELAY",
    [0x10] = "NUD_PROBE",
    [0x20] = "NUD_FAILED",
    [0x40] = "NUD_NOARP",
    [0x80] = "NUD_PERMANENT"};

struct {
    char str[20];
} neigh_flag[0xff] = {
    [0x01] = "NTF_USE",
    [0x08] = "NTF_ROUTER",
    [0x80] = "NTF_PROXY"};    

/**
 *  * 显示地址信息
 *   * 当地址变动的时候触发这个信息,例如通过DHCP获取到地址后
 *    */
void print_ifaddrmsg(struct nlmsghdr *nh)
{
    int               len;
    struct rtattr *   tb[IFA_MAX + 1];
    struct ifaddrmsg *ifaddr;
    char              tmp[256];
    char              tmp1[256];
    char              ifname[256];
    struct ifa_cacheinfo *ifa_cacheinfo = NULL;
    
    bzero(tb, sizeof(tb));
    ifaddr = NLMSG_DATA(nh);
    len    = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifaddr));
    parse_rtattr(tb, IFA_MAX, IFA_RTA(ifaddr), len);

    printf(FONT_COLOR_BLUE);

    if (nh->nlmsg_type == RTM_NEWADDR) {
        printf(FORMAT_CMD, "RTM_NEWADDR");
    }

    if (nh->nlmsg_type == RTM_DELADDR) {
        printf(FORMAT_CMD, "RTM_DELADDR");
    }

    if (nh->nlmsg_type == RTM_GETADDR) {
        printf(FORMAT_CMD, "RTM_GETADDR");
    }

    if_indextoname(ifaddr->ifa_index, ifname);
    sprintf(tmp, "ifname: %s", ifname);
    printf(FORMAT, tmp);

    sprintf(tmp, "scope id:%d str:%s", ifaddr->ifa_scope, ipaddr_scope[ifaddr->ifa_scope].str);
    printf(FORMAT, tmp);

    sprintf(tmp, "flags id:%d str:%s", ifaddr->ifa_flags, ipaddr_flags[ifaddr->ifa_flags].str);
    printf(FORMAT, tmp);

    if (tb[IFA_LABEL] != NULL) {
        sprintf(tmp, "label: %s ", RTA_DATA(tb[IFA_LABEL]));
        printf(FORMAT, tmp);
    }
    if (tb[IFA_ADDRESS] != NULL) {
        inet_ntop(ifaddr->ifa_family, RTA_DATA(tb[IFA_ADDRESS]), tmp, sizeof(tmp));
        sprintf(tmp1, "ifaddr: %s/%d ", tmp, ifaddr->ifa_prefixlen);
        printf(FORMAT, tmp1);
    }
    
    if (tb[IFA_LOCAL] != NULL) {
        inet_ntop(ifaddr->ifa_family, RTA_DATA(tb[IFA_LOCAL]), tmp, sizeof(tmp));
        sprintf(tmp1, "local: %s", tmp);
        printf(FORMAT, tmp1);
    }

    if (tb[IFA_BROADCAST] != NULL) {
        inet_ntop(ifaddr->ifa_family, RTA_DATA(tb[IFA_BROADCAST]), tmp, sizeof(tmp));
        sprintf(tmp1, "broadcast: %s", tmp);
        printf(FORMAT, tmp1);
    }    

    if (tb[IFA_ANYCAST] != NULL) {
        inet_ntop(ifaddr->ifa_family, RTA_DATA(tb[IFA_ANYCAST]), tmp, sizeof(tmp));
        sprintf(tmp1, "anycast: %s", tmp);
        printf(FORMAT, tmp1);
    }    


    if (tb[IFA_CACHEINFO] != NULL) {
	    ifa_cacheinfo = (struct ifa_cacheinfo *)RTA_DATA(tb[IFA_CACHEINFO]);
        sprintf(tmp1, "ifa_prefered: %d ifa_valid: %d cstamp: %d tstamp: %d ", ifa_cacheinfo->ifa_prefered, ifa_cacheinfo->ifa_valid, ifa_cacheinfo->cstamp, ifa_cacheinfo->tstamp);
        printf(FORMAT, tmp1);
    }   

    printf("\n");
    printf(COLOR_NONE);
}



/**
 *  * 显示路由信息
 *   * 当路由变动的时候触发这个信息
 *    */
void print_rtmsg(struct nlmsghdr *nh)
{
    int            len;
    struct rtattr *tb[RTA_MAX + 1];
    struct rtmsg * rt;
    char           tmp[256];
    char           tmp1[256];
    char           ifname[256];

    bzero(tb, sizeof(tb));
    rt  = NLMSG_DATA(nh);
    len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*rt));
    parse_rtattr(tb, RTA_MAX, RTM_RTA(rt), len);

    printf(FONT_COLOR_YELLOW);

    if (nh->nlmsg_type == RTM_NEWROUTE) {
        printf(FORMAT_CMD, "RTM_NEWROUTE");
    }

    if (nh->nlmsg_type == RTM_DELROUTE) {
        printf(FORMAT_CMD, "RTM_DELROUTE");
    }

    if (nh->nlmsg_type == RTM_GETROUTE) {
        printf(FORMAT_CMD, "RTM_GETROUTE");
    }

    sprintf(tmp, "table id: %s ", rt_table[rt->rtm_table].str);
    printf(FORMAT, tmp);

    sprintf(tmp, "who  add: %s", rt_protocol[rt->rtm_protocol].str);
    printf(FORMAT, tmp);

    sprintf(tmp, "scope: %s ", rt_scope[rt->rtm_scope].str);
    printf(FORMAT, tmp);

    sprintf(tmp, "type: %s ", rt_type[rt->rtm_type].str);
    printf(FORMAT, tmp);

    if (tb[RTA_DST] != NULL) {
        inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_DST]), tmp, sizeof(tmp));
        sprintf(tmp1, "%s/%d", tmp, rt->rtm_dst_len);
        sprintf(tmp, "RTA_DST: %s ", tmp1);
        printf(FORMAT, tmp);
    }
    if (tb[RTA_SRC] != NULL) {
        inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_SRC]), tmp, sizeof(tmp));
        sprintf(tmp1, "RTA_SRC: %s ", tmp);
        printf(FORMAT, tmp1);
    }
    if (tb[RTA_GATEWAY] != NULL) {
        inet_ntop(rt->rtm_family, RTA_DATA(tb[RTA_GATEWAY]), tmp, sizeof(tmp));
        sprintf(tmp1, "RTA_GATEWAY: %-20s ", tmp);
        printf(FORMAT, tmp1);
    }

    if (tb[RTA_OIF] != NULL) {
        if_indextoname(*(uint32_t *)RTA_DATA(tb[RTA_OIF]), ifname);
        sprintf(tmp, "RTA_OIF: %s ", ifname);
        printf(FORMAT, tmp);
    }

    printf("\n");
    printf(COLOR_NONE);
}


void print_neigh(struct nlmsghdr *nh)
{
    struct ndmsg * ndmsg;
    struct rtattr *attr;
    size_t         len;
    uint8_t        mac[6];
    uint8_t *      ip     = NULL;
    int            ip_len = 0;
    int            i      = 0;
    char           ifname[256];
    char           tmp[256];
    char           tmp1[256];
    ndmsg = NLMSG_DATA(nh);

    if (ndmsg->ndm_family != AF_INET && ndmsg->ndm_family != AF_INET6)
        return;

    attr = RTM_RTA(ndmsg);
    len  = RTM_PAYLOAD(nh);

    if (nh->nlmsg_type == RTM_NEWNEIGH) {
        printf(FORMAT_CMD, "RTM_NEWNEIGH");
    }

    if (nh->nlmsg_type == RTM_DELNEIGH) {
        printf(FORMAT_CMD, "RTM_DELNEIGH");
    }

    if (nh->nlmsg_type == RTM_GETNEIGH) {
        printf(FORMAT_CMD, "RTM_GETNEIGH");
    }

    if_indextoname(ndmsg->ndm_ifindex, ifname);
    sprintf(tmp, "ifname: %s", ifname);
    printf(FORMAT, tmp);

    sprintf(tmp, "state: %s", neigh_state[ndmsg->ndm_state].str);
    printf(FORMAT, tmp);

    sprintf(tmp, "flags: %s", neigh_state[ndmsg->ndm_flags].str);
    printf(FORMAT, tmp);

    for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len)) {
        if (attr->rta_type == NDA_LLADDR) {
            memset(mac, 0, 6);
            memcpy(mac, RTA_DATA(attr), 6);
            sprintf(tmp, "mac:%02x:%02x:%02x:%02x:%02x:%02x ", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            printf(FORMAT, tmp);
        }

        if (attr->rta_type == NDA_DST) {
            ip     = (uint8_t *)RTA_DATA(attr);
            ip_len = RTA_PAYLOAD(attr);
            inet_ntop(ndmsg->ndm_family, ip, tmp, sizeof(tmp));
            sprintf(tmp1, "neigh %s", tmp);
            printf(FORMAT, tmp1);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int                socket_fd;
    int                err = 0;
    fd_set             rd_set;
    struct timeval     timeout;
    int                select_r;
    int                read_r;
    int                send_r;
    struct sockaddr_nl sa;
    struct nlmsghdr *  nh;

    int  len = BUFLEN;
    char buff[BUFLEN];
    signal(SIGINT, intHandler);

    /*打开NetLink Socket*/
    socket_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    t_assert(socket_fd > 0);
    t_assert(!setsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &len, sizeof(len)));

    /*设定接收类型并绑定Socket*/
    bzero(&sa, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV4_ROUTE |
                   RTMGRP_IPV6_IFADDR | RTMGRP_IPV6_ROUTE | RTMGRP_NEIGH;
    sa.nl_pid = getpid();

    t_assert(!bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa)));

#if 1
    struct {
        struct nlmsghdr  nlh;
        struct ifinfomsg ifm;
    } nl_request;

    nl_request.nlh.nlmsg_type  = RTM_GETLINK;
    nl_request.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT | NLM_F_MATCH | NLM_F_DUMP;
    //  nl_request.nlh.nlmsg_len = sizeof(nl_request);
    nl_request.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
    nl_request.nlh.nlmsg_seq = time(NULL);
    //    nl_request.nlh.nlmsg_type = RTM_GETROUTE;
    //  nl_request.nlh.nlmsg_type = RTM_GETNEIGH;
    nl_request.nlh.nlmsg_type = RTM_GETLINK;
    //  nl_request.nlh.nlmsg_type = RTM_GETADDR;

    nl_request.ifm.ifi_family = AF_INET;

    send_r = send(socket_fd, &nl_request, sizeof(nl_request), 0);

    if (send_r < 0) {
        perror("send failed.");
        close(socket_fd);
        return -1;
    }
    while (keepRunning == 1) {
        read_r = read(socket_fd, buff, sizeof(buff));

        for (nh = (struct nlmsghdr *)buff; NLMSG_OK(nh, read_r);
             nh = NLMSG_NEXT(nh, read_r)) {
            switch (nh->nlmsg_type) {
            default:
                /*收到些奇怪的信息*/
                printf("nh->nlmsg_type = %d\n", nh->nlmsg_type);
                break;
            case NLMSG_DONE:
            case NLMSG_ERROR:
                break;
            case RTM_NEWLINK:
            case RTM_DELLINK:
            case RTM_GETLINK:
                print_ifinfomsg(nh);
                break;
            case RTM_NEWADDR:
            case RTM_DELADDR:
            case RTM_GETADDR:
                print_ifaddrmsg(nh);
                break;
            case RTM_NEWROUTE:
            case RTM_DELROUTE:
            case RTM_GETROUTE:
                print_rtmsg(nh);
                break;
            case RTM_NEWNEIGH:
            case RTM_DELNEIGH:
            case RTM_GETNEIGH:
                print_neigh(nh);
            }
        }
        usleep(100);
    }
    printf("end\n");
    return 1;
#endif

    while (keepRunning) {
        FD_ZERO(&rd_set);
        FD_SET(socket_fd, &rd_set);
        timeout.tv_sec  = 5;
        timeout.tv_usec = 0;
        select_r        = select(socket_fd + 1, &rd_set, NULL, NULL, &timeout);
        if (select_r < 0) {
            perror("select");
        } else if (select_r > 0) {
            if (FD_ISSET(socket_fd, &rd_set)) {
                read_r = read(socket_fd, buff, sizeof(buff));
                for (nh = (struct nlmsghdr *)buff; NLMSG_OK(nh, read_r);
                     nh = NLMSG_NEXT(nh, read_r)) {
                    switch (nh->nlmsg_type) {
                    default:
                        /*收到些奇怪的信息*/
                        printf("nh->nlmsg_type = %d\n", nh->nlmsg_type);
                        break;
                    case NLMSG_DONE:
                    case NLMSG_ERROR:
                        break;
                    case RTM_NEWLINK:
                    case RTM_DELLINK:
                    case RTM_GETLINK:
                        print_ifinfomsg(nh);
                        break;
                    case RTM_NEWADDR:
                    case RTM_DELADDR:
                    case RTM_GETADDR:
                        print_ifaddrmsg(nh);
                        break;
                    case RTM_NEWROUTE:
                    case RTM_DELROUTE:
					case RTM_GETROUTE:                    
                        print_rtmsg(nh);
                        break;
                    case RTM_NEWNEIGH:
                    case RTM_DELNEIGH:
                    case RTM_GETNEIGH:                    
                        print_neigh(nh);
                    }
                }
            }
        }
    }

    close(socket_fd);

error:
    if (err < 0) {
        printf("Error at line %d\nErrno=%d\n", -err, errno);
    }
    return err;
}
