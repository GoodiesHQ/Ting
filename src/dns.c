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
    unsigned char name[256], cnt, *dns_iter; // maximum name size
    const unsigned char *end = (unsigned char*)buffer + size;
    size_t name_size, i;

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

    if(dns->question_count != ting_be16(1) || dns->answer_count != 0)
    {
        return;
    }

    dns_iter = (unsigned char*)dns + sizeof(ting_hdr_dns);
    name_size = 0;

    while((cnt = *dns_iter++) != 0)
    {
        for(i = 0; i < cnt && dns_iter != end; ++i)
        {
            name[name_size++] = *dns_iter++;
        }
        name[name_size++] = '.';
    }
    name[name_size - 1] = '\0';

    if(dns_iter == end)
    {
        return;
    }

    debugf("Query Type Value: %u\n", *dns_iter);

    switch(*dns_iter)
    {
        case TING_DNS_TYPE_A:
            break;
        default: return;
    }

    debugf("Handling DNS Query: %s\n", name);
}

