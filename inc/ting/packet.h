#ifndef TING_PACKET_H
#define TING_PACKET_H

#define TING_PKT_BUF_SIZE UINT16_MAX
#define TING_OCTETS(a, b, c, d) \
    (uint32_t)((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))


#include <ting/endian.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <ting/gre.h>
#include <ting/dns.h>

typedef struct ethhdr   ting_hdr_eth;
typedef struct iphdr    ting_hdr_ip;
typedef struct tcphdr   ting_hdr_tcp;
typedef struct udphdr   ting_hdr_udp;
typedef struct dnshdr   ting_hdr_dns;
typedef struct grehdr   ting_hdr_gre; // Note: this is the slight variant of GRE used by PPTP

char ting_buf_pkt[TING_PKT_BUF_SIZE];

#ifdef TING_CAPTURE_IP_ONLY
#define TING_CAPTURE_TYPE ETH_P_IP
#else
#define TING_CAPTURE_TYPE ETH_P_ALL
#endif

#endif//TING_PACKET_H
