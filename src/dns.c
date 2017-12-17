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

    ting_hdr_udp *udp = (ting_hdr_udp*)(buffer + sizeof(ting_hdr_eth) + (ip->ihl * 4));
    printf("%s:%u", inet_ntoa((struct in_addr){.s_addr=ip->saddr}), udp->uh_sport);
    puts("-> ");
    printf("%s:%u", inet_ntoa((struct in_addr){.s_addr=ip->daddr}), udp->uh_dport);
    fputc('\n', stdout);
}
