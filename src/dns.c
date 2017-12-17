#include <ting/debug.h>
#include <ting/dns.h>
#include <stdio.h>

bool ting_feature_dns_init(void)
{
    debugf("%s\n", "Starting DNS feature");
    memset((void*)ting_buf_dns, 0, sizeof(ting_buf_dns));
    return true;
}

void ting_feature_dns_process(char *buffer, uint16_t size)
{
    if(size > TING_DNS_MAX_SIZE + sizeof(ting_hdr_eth))
    {
        return;
    }

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
    if(udp->dest != ting_be16(53))
    {
        return;
    }

    ting_hdr_dns *dns = (ting_hdr_dns*)((char*)udp + sizeof(ting_hdr_udp));

    if(dns->response != TING_DNS_RESPONSE_QUERY)
    {
        return;
    }

    if(dns->question_count != ting_be16(1))
    {
        return;
    }

    debugf("%s\n", "Handling DNS packet");
}

