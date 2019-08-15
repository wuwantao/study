# -*- coding: utf-8 -*-

"""
/* DHCP message types. */
#define DHCPDISCOVER            1
#define DHCPOFFER               2
#define DHCPREQUEST             3
#define DHCPDECLINE             4
#define DHCPACK                 5
#define DHCPNAK                 6
#define DHCPRELEASE             7
#define DHCPINFORM              8
#define DHCPLEASEQUERY          10
#define DHCPLEASEUNASSIGNED     11
#define DHCPLEASEUNKNOWN        12
#define DHCPLEASEACTIVE         13
"""
import subprocess
import os
def pcap_dir(dirname):

    for maindir,subdir,file_name_list in os.walk(dirname):

        for filename in file_name_list:
            exec_name = os.path.join(maindir, filename)

            cmd = "tshark -r " + exec_name + " -Y \"bootp.option.dhcp == 4\""
            a = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
            lines = a.stdout.readlines()
            if lines:
                print(cmd)
                var = lines[0].decode("utf-8")
                print(var)




pcap_dir("D:\项目\DHCP报文\pcap")            

