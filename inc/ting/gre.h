#ifndef TING_GRE_H
#define TING_GRE_H

/*
 * This defines the "Standard" variant of GRE (version 0)
 *
 * Refer to RFC 2890 (and 2784)
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <ting/endian.h>
#include <stdbool.h>
#include <stdint.h>

#define TING_GRE_KEY            0x11223344
#define TING_GRE_VERSION        0
#define TING_GRE_DESTINATION    "7.7.7.7"

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
#if defined(TING_BYTEORDER_LE)
    uint16_t    recur:      3,
                strict:     1,
                has_seq:    1,
                has_key:    1,
                has_route:  1,
                has_cksum:  1,
                version:    3,
                flags:      4,
                has_ack:    1;
#elif defined(TING_BYTEORDER_BE)
    uint16_t    has_cksum:  1,
                has_route:  1,
                has_key:    1,
                has_seq:    1,
                strict:     1,
                recur:      3,
                has_ack:    1,
                flags:      4,
                version:    3;
#endif
    uint16_t proto;
};

struct grehdr_key
{
    uint32_t key;
};

struct grehdr_seq
{
    uint32_t seq;
};

struct grehdr_ack
{
    uint32_t ack;
};

#define TING_GRE_BUF_SIZE (size_t)(UINT16_MAX + sizeof(struct grehdr) + 12) // max packet size with optional features
char ting_gre_buf[TING_GRE_BUF_SIZE];
int ting_gre_sockfd;
struct sockaddr_in ting_gre_sa;
uint32_t ting_gre_seq;

bool ting_feature_gre_init(void);
void ting_feature_gre_process(char *buffer, uint16_t size);

#endif//TING_GRE_H
