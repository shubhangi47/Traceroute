#include "tracer_route.h"

Tracer_route::Tracer_route(std::string hostname_) : hostname(hostname_)
{

    // struct addrinfo exp, *connection;
    exp.ai_family = AF_INET;
    memset(&exp, 0, sizeof exp);
    int ret = getaddrinfo(this->hostname.c_str(), "33435", &exp, &connection);
    if (ret != 0)
    {
        printf("%s\n", gai_strerror(ret));
    }

    void *addr;
    std::string ipver;

    if (connection->ai_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)connection->ai_addr;
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
    }

    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(connection->ai_family, addr, ipstr, sizeof ipstr);
    std::cout << ipver << " address: " << ipstr << std::endl;
}
int Tracer_route::createUDPsocket(int ttlValue)
{

    // create UDP socket
    // Create a UDP socket
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket < 0)
    {
        std::cerr << "Failed to create UDP socket\n";
        return EXIT_FAILURE;
    }

    // if (setsockopt(udpSocket, IPPROTO_IP, IP_TTL, &ttlValue, sizeof(ttlValue)) < 0)
    // {
    //     std::cerr << "Failed to set TTL for UDP socket\n";
    //     close(udpSocket);
    //     return EXIT_FAILURE;
    // }
    return 0;
}

void Tracer_route::pingUDP()
{
    // Message to send
    char *message_ = "Hello, UDP!";
    // memset(message, ' ', 64);
    bool done = false;
    int ttl = 1, max_ttl = 30; // ttl is no of hops
    while (ttl < max_ttl)
    {

        ttl++;
       
        // Send the UDP message
        ssize_t bytesSent = sendto(udpSocket, message_, strlen(message_), 0,
                                   connection->ai_addr, connection->ai_addrlen);
        if (bytesSent < 0)
        {
            std::cerr << "Failed to send UDP message\n";
            close(udpSocket);
            bool done = false;
        }

        std::cout << "UDP message sent to " << hostname << "\n";
        pollfd readset;
        readset.fd = udpSocket;
        readset.events = 0;
        int retval_ = poll(&readset, 1, 1000);
        // Check results
        if (retval_ < 0)
        {
            perror("poll()");
        }
        else if (retval_ == 0)
        {

            printf("%d  *\n", ttl);
            continue;
        }
        
    }
}

int Tracer_route::createICMPsocket()
{

    int icmpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmpSocket < 0)
    {
        std::cerr << "Failed to create ICMP socket\n";
        return EXIT_FAILURE;
    }
    int ttlValue = 1;

    if (setsockopt(icmpSocket, IPPROTO_IP, IP_TTL, &ttlValue, sizeof(ttlValue)) < 0)
    {
        std::cerr << "Failed to set TTL for ICMP socket\n";
        close(icmpSocket);
        return EXIT_FAILURE;
    }

    return 0;
}