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
#include <ting/debug.h>

typedef struct ethhdr   ting_hdr_eth;
typedef struct iphdr    ting_hdr_ip;
typedef struct tcphdr   ting_hdr_tcp;
typedef struct udphdr   ting_hdr_udp;
typedef struct dnshdr   ting_hdr_dns;
typedef struct grehdr   ting_hdr_gre; // Note: this is the slight variant of GRE used by PPTP

#ifdef TING_DEBUG
static void print_ip_header(unsigned char* buffer)
{

    ting_hdr_ip *iph = (ting_hdr_ip*)buffer;
    unsigned long iphdrlen =iph->ihl*4;
    struct sockaddr_in source, dest;
    FILE *logfile = stdout;

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    fprintf(logfile,"\n");
    fprintf(logfile,"IP Header\n");
    fprintf(logfile,"   |-IP Version        : %d\n",(unsigned int)iph->version);
    fprintf(logfile,"   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    fprintf(logfile,"   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    fprintf(logfile,"   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    fprintf(logfile,"   |-Identification    : %d\n",ntohs(iph->id));
    //fprintf(logfile,"   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
    //fprintf(logfile,"   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
    //fprintf(logfile,"   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    fprintf(logfile,"   |-TTL      : %d\n",(unsigned int)iph->ttl);
    fprintf(logfile,"   |-Protocol : %d\n",(unsigned int)iph->protocol);
    fprintf(logfile,"   |-Checksum : %d\n",ntohs(iph->check));
    fprintf(logfile,"   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
    fprintf(logfile,"   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
}

static void print_udp_packet(unsigned char* buffer)
{
    print_ip_header(buffer);
    ting_hdr_ip *iph = (ting_hdr_ip*)buffer;
    ting_hdr_udp *udph = (ting_hdr_udp*)(buffer + (iph->ihl * 4));
    FILE *logfile = stdout;
    fprintf(logfile,"\n");
    fprintf(logfile,"UDP Header\n");
    fprintf(logfile,"   |-Source Port       : %d\n", ting_be16(udph->source));
    fprintf(logfile,"   |-Destination Port  : %d\n", ting_be16(udph->dest));
    fprintf(logfile,"   |-UDP Length        : %d\n", ting_be16(udph->len));
    fprintf(logfile,"   |-UDP Checksum      : %X\n", ting_be16(udph->check));
}
#else
#define print_ip_header(...)
#endif

char ting_buf_pkt[TING_PKT_BUF_SIZE];

#ifdef TING_CAPTURE_IP_ONLY
#define TING_CAPTURE_TYPE ETH_P_IP
#else
#define TING_CAPTURE_TYPE ETH_P_ALL
#endif

#endif//TING_PACKET_H
