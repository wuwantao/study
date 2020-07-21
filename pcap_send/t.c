#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include <pcap.h>
#include <unistd.h>


int main() 
{
    char *output_dev = "eno1";
    char *infile_str = "./taobao.pcap";
    int promisc = 1;
    int timeout = 0;
    int snaplen = 2048;
    pcap_t *out_dev = NULL;
    pcap_t *in_dev = NULL;    
    struct pcap_pkthdr packet;
    const uint8_t *pktStr = NULL;
    char errBuf[PCAP_ERRBUF_SIZE];

    in_dev = pcap_open_offline(infile_str, errBuf);

    if (in_dev == NULL) {
        printf("error: pcap_open_offline:%s\n", errBuf);
        return -1;
    }


    while (1) {
        pktStr = pcap_next(in_dev, &packet);
        
        if (pktStr == NULL) {
            printf("did not capture a packet!\n");
            return -1;
        }
        break;
    }

    out_dev = pcap_open_live("eno1", snaplen, promisc, timeout, errBuf);
    
    if (out_dev == NULL) {
        printf("error: pcap_open_live(): %s\n", errBuf);
        return -1;
    }    

    while (1) {
        pcap_sendpacket(out_dev, pktStr, packet.caplen);
        sleep(1);
    }

    pcap_close(in_dev);
    pcap_close(out_dev);    
	return 1;
}
