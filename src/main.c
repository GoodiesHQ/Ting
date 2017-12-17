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
    size_t i;


    if((sockfd = socket(AF_PACKET, SOCK_RAW, htons(TING_CAPTURE_TYPE))) < 0)
    {
        perror("socket");
        return 1;
    }

    while((pkt_size = recvfrom(sockfd, (void*)ting_buf_pkt, sizeof(ting_buf_pkt), 0, (struct sockaddr*)&saddr, &saddr_size)) >= 0)
    {
        if (!pkt_size)
        {
            continue;
        }

        switch(saddr.sll_pkttype)
        {
            case PACKET_OUTGOING:
            case PACKET_OTHERHOST:
            case PACKET_LOOPBACK:
                continue;
            default: break;
        }

        for(i = 0; i < TING_FEATURE_COUNT; ++i)
        {
            if(ting_features[i].process != NULL)
            {
                ting_features[i].process(ting_buf_pkt, (uint16_t)pkt_size);
            }
        }
    }
}

int main()
{
    size_t i;

    for(i = 0; i < TING_FEATURE_COUNT; ++i)
    {
        if(ting_features[i].init() == false)
        {
            ting_features[i].process = NULL;
        }
    }

    sniff();
    return 0;
}