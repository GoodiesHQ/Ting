#include <stdio.h>
#include <ting/gre.h>
#include <ting/packet.h>

bool ting_feature_gre_init(void)
{
    ting_gre_seq = 0;
    memset((void*)&ting_gre_sa, 0, sizeof(ting_gre_sa));
    memset((void*)ting_buf_gre, 0, sizeof(ting_buf_gre));

    if((ting_gre_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_GRE)) < 0)
    {
        return false;
    }

    ting_gre_sa.sin_family = AF_INET;
    ting_gre_sa.sin_addr.s_addr = inet_addr(TING_GRE_DESTINATION);

    return true;
}

void ting_feature_gre_process(char *buffer, uint16_t size) {
    memset((void *) ting_buf_gre, 0, sizeof(ting_buf_gre));
    ting_hdr_gre *gre = (ting_hdr_gre *) ting_buf_gre;

    gre->has_cksum = false;
    gre->has_route = false;
    gre->has_key = true;
    gre->has_seq = true;
    gre->recur = 0;
    gre->has_ack = false;
    gre->flags = 0;
    gre->version = TING_GRE_VERSION;

#ifdef TING_CAPTURE_IP_ONLY
    gre->proto = ting_be16(0x0800); // IP4

    ting_hdr_eth *eth = (ting_hdr_eth*)buffer;
    if(eth->h_proto != ting_be16(ETH_P_IP))
    {
        return;
    }
#else
    gre->proto = ting_be16(0x6558); // Transparent Ethernet Tunnel
#endif

    size_t offset = (size_t)sizeof(struct grehdr);

    if (gre->has_key)
    {
        struct grehdr_key *key = (struct grehdr_key*) (ting_buf_gre + offset);
        key->key = ting_be32(TING_GRE_KEY);
        offset += sizeof(key->key);
    }

    if (gre->has_seq)
    {
        struct grehdr_seq *seq = (struct grehdr_seq*) (ting_buf_gre + offset);
        seq->seq = ting_be32(ting_gre_seq++);
        offset += sizeof(seq->seq);
    }

    /*
     * Unnecessary, never used
     *
    if (gre->has_ack)
    {
        struct grehdr_ack *ack = (struct grehdr_ack*) (ting_buf_gre + offset);
        ack->ack = ting_be32(ting_gre_seq);
        offset += sizeof(ack->ack);
    }
     */

#ifdef TING_CAPTURE_IP_ONLY
    size_t total_size = (size_t)size + offset - sizeof(ting_hdr_eth);
    memmove((void *) (ting_buf_gre + offset), (void*)(buffer + sizeof(ting_hdr_eth)), size - sizeof(ting_hdr_eth));
#else
    size_t total_size = (size_t)size + offset;
    memmove((void *)(ting_buf_gre + offset), (void*)buffer, size);
#endif

    if (ting_gre_sockfd >= 0) {
        sendto(ting_gre_sockfd, (void *) ting_buf_gre, total_size, 0, (struct sockaddr *) &ting_gre_sa,
               sizeof(struct sockaddr));
    }
}
