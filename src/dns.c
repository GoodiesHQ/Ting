#include <ting/dns.h>
#include <stdio.h>

bool ting_feature_dns_init(void)
{
    memset((void*)ting_buf_dns, 0, sizeof(ting_buf_dns));
    return true;
}

void ting_feature_dns_process(char *buffer, uint16_t size)
{
    ting_hdr_eth *eth = (ting_hdr_eth*)buffer;
    if(eth->h_proto != ting_be16(ETH_P_IP))
    {
        return;
    }

    ting_hdr_ip *ip = (ting_hdr_ip*)(buffer + sizeof(ting_hdr_eth));
    if(ip->protocol != IPPROTO_UDP)
    {
        return;
    }

    struct in_addr saddr, daddr;
    saddr.s_addr = ip->saddr;
    daddr.s_addr = ip->daddr;

    ting_hdr_udp *udp = (ting_hdr_udp*)(buffer + sizeof(ting_hdr_eth) + (ip->ihl * 4));
    printf("%s:%u -> %s:%u\n", inet_ntoa(saddr), ntohs(udp->source), inet_ntoa(daddr), ntohs(udp->dest));
    //puts("DNS Packet\n");
}
