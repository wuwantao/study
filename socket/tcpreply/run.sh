#!/bin/bash
#tcpreplay -i eth3 -l 0 -p 1 broadcast.pcap
while true
do
    tcpreplay -i eth3 -l 1 -p 1 broadcast.pcap
    sleep 1
done
