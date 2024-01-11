#ifndef TRACER_ROUTE_H
#define TRACER_ROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/sem.h>
#include <poll.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
class Tracer_route
{
public:
    Tracer_route(const char *hostname);
    int createICMPsocket();
    int ICMPHop();
    ~Tracer_route()
    {
        close(this->icmpsocket);
    }
    unsigned short csum(unsigned short *buf, int nwords)
    {
        unsigned long sum;
        for (sum = 0; nwords > 0; nwords--)
            sum += *buf++;
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        return ~sum;
    }

private:
    int destport = 33435;
    const char *hostname;
    int icmpsocket;
    struct ip *ip_hdr;
    int hop = 0;
    char buf[4096];
    struct sockaddr_in addr;
};

#endif
