#include <ting/debug.h>
#include <ting/dns.h>
#include <stdio.h>

static const ting_dns_host ting_dns_hosts[] = {
        (ting_dns_host){.host="google.com", .addr=TING_OCTETS(192, 168, 1, 10)},
};

static unsigned char ting_dns_name[256];

static const size_t ting_dns_hosts_count = sizeof(ting_dns_hosts)/sizeof(ting_dns_host);

bool ting_feature_dns_init(void)
{
    debugf("%s\n", "Starting DNS feature");
    memset((void*)ting_buf_dns, 0, sizeof(ting_buf_dns));
    return true;
}

void ting_feature_dns_process(char *buffer, uint16_t size)
{
    unsigned char cnt, *dns_iter; // maximum name size
    const unsigned char *end = (unsigned char*)buffer + size;
    size_t name_size, i;

    ting_hdr_eth *eth;
    ting_hdr_ip *ip, *ip_res;
    ting_hdr_udp *udp, *udp_res;
    ting_hdr_dns *dns, *dns_res;

    if(size > TING_DNS_MAX_SIZE + sizeof(ting_hdr_eth))
    {
        return;
    }

    eth = (ting_hdr_eth*)buffer;
    if(eth->h_proto != ting_be16(ETH_P_IP))
    {
        return;
    }

    ip = (ting_hdr_ip*)(buffer + sizeof(ting_hdr_eth));
    if(ip->protocol != IPPROTO_UDP)
    {
        return;
    }

    udp = (ting_hdr_udp*)(buffer + sizeof(ting_hdr_eth) + (ip->ihl * 4));
    if(udp->dest != ting_be16(53))
    {
        return;
    }

    dns = (ting_hdr_dns*)((char*)udp + sizeof(ting_hdr_udp));

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
            ting_dns_name[name_size++] = *dns_iter++;
        }
        ting_dns_name[name_size++] = '.';
    }
    ting_dns_name[name_size - 1] = '\0';

    if(dns_iter == end || dns_iter+1 == end)
    {
        return;
    }

    uint16_t dns_type = ting_be16(*((uint16_t*)dns_iter));

    debugf("Query Type Value: %u\n", dns_type);

    switch(dns_type)
    {
        case TING_DNS_TYPE_A:
            break;
        default: return;
    }

    debugf("Handling DNS A Query: %s\n", ting_dns_name);


    ip_res = (ting_hdr_ip*)ting_buf_dns;
    ip_res->frag_off = ting_be16(0x4000);
    ip_res->protocol = IPPROTO_UDP;
    ip_res->ihl = 5;
    ip_res->version = ip->version;
    ip_res->ttl = 64;
    ip_res->check = 0;

    ip_res->saddr = ip->daddr;
    ip_res->daddr = ip->saddr;

    udp_res = (ting_hdr_udp*)(ting_buf_dns + (ip_res->ihl * 4));
    udp_res->check = 0;
    udp_res->source = udp->dest;
    udp_res->dest = udp->source;

    dns_res = (ting_hdr_dns*)((char*)udp_res + sizeof(ting_hdr_udp));

    dns->response = TING_DNS_RESPONSE_ANSWER; // create an answer packet
    dns_res->opcode = 0;
    dns_res->authoritative_answer = false;
    dns_res->truncated = false;
    dns_res->recursion_desired = true;
    dns_res->recursion_available = true;
    dns_res->unused = false;
    dns_res->authentic_data = false;
    dns_res->response_code = 0;
    dns_res->question_count = 1;
    dns_res->answer_count = 1;

    udp_res->len = 0;
}

