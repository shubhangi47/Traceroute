#include <iostream>

#include "tracer_route.cpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <arg1> <arg2>\n";
        return 1;
    }

    const char *hostname = argv[1];

    std::cout << "hostname is  " << hostname << std::endl;
    std::unique_ptr<Tracer_route> traceobj = std::make_unique<Tracer_route>(hostname);
    traceobj->createICMPsocket();
    traceobj->ICMPHop();

    return 0;
}

/* g++ main.cpp -o output
  sudo ./output 8.8.8.8

Output :
hostname is  8.8.8.8
 hop limit 0 Address 172.27.144.1
 hop limit 1 Address 172.27.144.1
 hop limit 2 Address 192.168.1.1
 hop limit 3 Address 103.252.169.230
 hop limit 4 Address 103.77.108.82
 hop limit 5 Address 74.125.37.7
 hop limit 6 Address 142.250.227.73
 Reached destination 8.8.8.8  hop limit 7*/
