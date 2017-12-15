#include <ting.h>
#include <stdio.h>

#define PACKET_OTHERHOST	3		/* To someone else 	*/
#define PACKET_OUTGOING		4		/* Outgoing of any type */
#define PACKET_LOOPBACK		5		/* MC/BRD frame looped back */

struct sockaddr_ll {
    unsigned short sll_family;   /* Always AF_PACKET */
    unsigned short sll_protocol; /* Physical-layer protocol */
    int            sll_ifindex;  /* Interface number */
    unsigned short sll_hatype;   /* ARP hardware type */
    unsigned char  sll_pkttype;  /* Packet type */
    unsigned char  sll_halen;    /* Length of address */
    unsigned char  sll_addr[8];  /* Physical-layer address */
};

int sniff()
{
    ssize_t pkt_size;
    struct sockaddr_ll saddr;
    socklen_t saddr_size = sizeof(saddr);
    int sockfd;

    if(ting_feature_gre_init() == false)
    {
        perror("GRE init");
    }

    if((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0)
    {
        perror("socket");
        return 1;
    }

    while((pkt_size = (uint16_t)recvfrom(sockfd, (void*)ting_pkt_buf, sizeof(ting_pkt_buf), 0, (struct sockaddr*)&saddr, &saddr_size)) >= 0)
    {
        if (!pkt_size || saddr.sll_pkttype == PACKET_OTHERHOST || saddr.sll_pkttype == PACKET_OUTGOING || saddr.sll_pkttype == PACKET_LOOPBACK)
        {
            continue;
        }

        ting_feature_gre_process(ting_pkt_buf, (uint16_t)pkt_size);
    }
}

int main()
{
    sniff();
    return 0;
}