#include <iostream>
#include "tracer_route.cpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <arg1> <arg2>\n";
        return 1;
    }

    std::string hostname = argv[1];

    std::cout << "hostname is  " << hostname << std::endl;
    Tracer_route traceobj(hostname);
    traceobj.createUDPsocket(1);
    traceobj.pingUDP();
   

    return 0;
}
