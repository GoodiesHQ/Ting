#include <ting/gre.h>
#include <ting/packet.h>


static inline size_t ting_gre_packet_size(ting_hdr_gre *gre_hdr)
{
    size_t size = sizeof(ting_hdr_gre);
    if(gre_hdr->has_cksum) size += 4;   // key exists
    if(gre_hdr->has_key) size += 4;     // sequence exists
    if(gre_hdr->has_seq) size += 4;     // checksum exists
    return size;
}

bool ting_feature_gre_init(void)
{
    memset((void*)ting_gre_buf, 0, sizeof(ting_gre_buf));
    if((ting_gre_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_GRE)) == -1)
    {
        return false;
    }

    return true;
}

ting_hdr_ip* ting_gre_encapsulate(void *packet, size_t size, uint32_t sequence)
{
    ting_hdr_ip *ip, *gre_ip;
    ting_hdr_gre *gre;
    void *ptr;

    ip = (ting_hdr_ip*)packet;
    gre = (ting_hdr_gre*)(ip + 1);

    // RFC 2637
    gre->proto = 0x880B;

    gre->has_cksum = false;
    gre->has_route = false;
    gre->has_key = true;
    gre->has_seq = true; // all data packets should have a sequence number
    gre->strict = false;
    gre->recur = 0;
    gre->ack = 0;
    gre->flags = 0;
    gre->version = TING_GRE_VERSION;

    return NULL;
}
