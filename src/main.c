#include <ting.h>
#include <stdio.h>

int sniff()
{
    bool done = false;
    uint16_t pkt_size;
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);
    int sockfd;

    if(ting_feature_gre_init() == false)
    {
        perror("GRE init");
    }

    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
    {
        perror("socket");
        return 1;
    }

    while(!done)
    {
        pkt_size = (uint16_t)recvfrom(sockfd, (void*)ting_pkt_buf, sizeof(ting_pkt_buf), 0, &saddr, &saddr_size);
        fprintf(stdout, "Packet Size: %u\n", pkt_size);
        ting_feature_gre_process(ting_pkt_buf, pkt_size);
    }
}

int main()
{
    sniff();
    return 0;
}