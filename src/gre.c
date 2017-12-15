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
    ting_gre_seq = 0;
    memset((void*)&ting_gre_sa, 0, sizeof(ting_gre_sa));
    memset((void*)ting_gre_buf, 0, sizeof(ting_gre_buf));

    if((ting_gre_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_GRE)) < 0)
    {
        return false;
    }

    ting_gre_sa.sin_family = AF_INET;
    ting_gre_sa.sin_addr.s_addr = inet_addr(TING_GRE_DESTINATION);

    return true;
}

void ting_feature_gre_process(char *buffer, uint16_t size)
{
    /*
     * Process each packet and forward them over a GRE tunnel
     *
     * RFC 2637 - PPTP GRE
     */
    size_t total_size = (size_t)size + sizeof(ting_hdr_gre);
    memset((void*)ting_gre_buf, 0, sizeof(ting_gre_buf));
    ting_hdr_gre *gre = (ting_hdr_gre*)ting_gre_buf;

    gre->has_cksum = false;
    gre->has_route = false;
    gre->has_key = true;
    gre->has_seq = true;
    gre->recur = 0;
    gre->has_ack = false;
    gre->flags = 0;
    gre->version = TING_GRE_VERSION;
    gre->proto = ting_be16(0x880B);
    gre->key_payload_len = size;
    gre->key_call_id = 1;
    gre->seq_num = ting_gre_seq++;

    memmove((void*)(ting_gre_buf + sizeof(ting_hdr_gre)), buffer, size);

    sendto(ting_gre_sockfd, (void*)ting_gre_buf, total_size, 0, (struct sockaddr*)&ting_gre_sa, sizeof(struct sockaddr));
}
