#ifndef TRACER_ROUTE_H
#define TRACER_ROUTE_H

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <poll.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <linux/errqueue.h>
class Tracer_route
{
public:
Tracer_route(std::string hostname);
int createUDPsocket(int );
int createICMPsocket();
~Tracer_route(){

}
void pingUDP();



private:
int destport = 33435;
std::string hostname;
int udpSocket;
struct addrinfo exp, *connection;
};

#endif
