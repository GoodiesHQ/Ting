#ifndef TING_GRE_H
#define TING_GRE_H

/*
 * This defines the PPTP variant of GRE
 *
 * Refer to RFC 2637
 */

#include <ting/endian.h>
#include <ting/packet.h>
#include <stdbool.h>
#include <stdint.h>

#define TING_GRE_VERSION 1
#define TING_GRE_DESTINATION "127.0.0.1"

#define TING_GRE_BUF_SIZE TING_PKT_BUF_SIZE + sizeof(struct grehdr) + 4 + 4 + 4;
char ting_gre_buf[TING_GRE_BUF_SIZE];
int ting_gre_sockfd;
uint32_t ting_gre_seq;

enum
{
    TING_OPT_GRE_STRICT     = 0b00001,
    TING_OPT_GRE_SEQUENCE   = 0b00010,
    TING_OPT_GRE_KEY        = 0b00100,
    TING_OPT_GRE_ROUTING    = 0b01000,
    TING_OPT_GRE_CHECKSUM   = 0b10000,
};

struct grehdr
{
    union {
        uint16_t base_hdr;
#if defined(TING_BYTEORDER_LE)
        uint16_t    recur:      3,
                    strict:     1,
                    has_seq:    1,
                    has_key:    1,
                    has_route:  1,
                    has_cksum:  1,
                    version:    3,
                    flags:      4,
                    ack:        1;
#elif defined(TING_BYTEORDER_BE)
        uint16_t    has_cksum:  1,
                    has_route:  1,
                    has_key:    1,
                    has_seq:    1,
                    strict:     1,
                    recur:      3,
                    ack:        1,
                    flags:      4,
                    version:    3;
#endif
    };
    uint16_t proto;
    uint16_t key_payload_len;
    uint16_t key_call_id;
    union {
        uint32_t seq_num;
        uint32_t ack_num;
    };
};

bool ting_feature_gre_init(void);
void ting_feature_gre_process(char *buffer, size_t size);

#endif//TING_GRE_H
