#!/bin/bash
#tcpreplay -l 0 -p 90000 -i eth0 dns250.pcap
#tcpreplay -l 0 -t -i eth0 taobao.pcap
tcpreplay -l 0 -p 90000 -i eth0 ptr.pcap 
#tcpreplay -l 0 -p 90000 -i eth0 error.pcap 
#tcpreplay -l 0 -p 90000 -i eth0 soa_query.pcap 
#tcpreplay -l 0 -p 90000 -i eth0 edns_subnet.pcap 
#tcpreplay -l 0 -p 90000 -i eth0 edns_not_subnet.pcap 
