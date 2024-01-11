#include "tracer_route.h"

Tracer_route::Tracer_route(const char *hostname_) : hostname(hostname_)
{

    this->hostname = hostname_;
}

int Tracer_route::createICMPsocket()
{
    /* Creates a raw socket (SOCK_RAW) to work directly with IP packets, specifically ICMP in this case.*/

    icmpsocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    buf[4096] = {0};
    ip_hdr = (struct ip *)buf; /* Ip header */
    hop = 0;

    int one = 1;
    const int *val = &one;

    /* Sets the IP_HDRINCL socket option to include the IP header when sending packets. */

    if (setsockopt(icmpsocket, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
        std::cout << "cannot set ttl for socket" << std::endl;

    addr.sin_port = htons(this->destport);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, this->hostname, &(addr.sin_addr));

    return 0;
}

int Tracer_route::ICMPHop()
{
    while (hop <= 30)
    {
        ip_hdr->ip_hl = 5;
        ip_hdr->ip_v = 4;
        ip_hdr->ip_tos = 0;
        ip_hdr->ip_len = 20 + 8;
        ip_hdr->ip_id = 10000;
        ip_hdr->ip_off = 0;
        ip_hdr->ip_ttl = hop;
        ip_hdr->ip_p = IPPROTO_ICMP;
        inet_pton(AF_INET, "172.27.152.253", &(ip_hdr->ip_src)); /*  172.27.152.253 - USE your IP */
        inet_pton(AF_INET, this->hostname, &(ip_hdr->ip_dst));
        ip_hdr->ip_sum = csum((unsigned short *)buf, 9);

        struct icmphdr *icmphd = (struct icmphdr *)(buf + 20); /* ICMP header */
        icmphd->type = ICMP_ECHO;
        icmphd->code = 0;
        icmphd->checksum = 0;
        icmphd->un.echo.id = 0;
        icmphd->un.echo.sequence = hop + 1;
        icmphd->checksum = csum((unsigned short *)(buf + 20), 4);
        /* Sending ICMP Echo Request */
        sendto(icmpsocket, buf, sizeof(struct ip) + sizeof(struct icmphdr), 0, (struct sockaddr *)&addr, sizeof addr);
        char buff[4096] = {0};
        struct sockaddr_in addr2;
        socklen_t len = sizeof(struct sockaddr_in);
        /* Receiving ICMP Echo Reply */
        recvfrom(icmpsocket, buff, sizeof(buff), 0, (struct sockaddr *)&addr2, &len);
        struct icmphdr *icmphd2 = (struct icmphdr *)(buff + 20);
        if (icmphd2->type != 0)
            std::cout << " hop limit " << hop << " Address " << inet_ntoa(addr2.sin_addr) << std::endl;

        else
        {
            std::cout << " Reached destination " << inet_ntoa(addr2.sin_addr) << "  hop limit " << hop << std::endl;

            exit(0);
        }

        hop++;
    }

    return 0;
}
