#ifndef TING_PACKET_H
#define TING_PACKET_H

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

typedef struct ethhdr   ting_hdr_eth;
typedef struct iphdr    ting_hdr_ip;
typedef struct tcphdr   ting_hdr_tcp;
typedef struct udphdr   ting_hdr_udp;
typedef struct grehdr   ting_hdr_gre; // Note: this is the slight variant of GRE used by PPTP

#define TING_PKT_BUF_SIZE UINT16_MAX
char ting_pkt_buf[TING_PKT_BUF_SIZE];

#endif//TING_PACKET_H
