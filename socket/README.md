bind(sockfd, addr, len)
sendto( sockfd, "HELLO", strlen ( "HELLO" ), 0, (struct sockaddr*)&peeraddr, socklen )

为什么addr是struct sockaddr类型?
是因为
bind sendto,要兼容ipv4, ipv6等不同协议

ipv4的addr 类型是struct sockaddr_in
ipv6的addr 类型是 struct sockaddr_in6

