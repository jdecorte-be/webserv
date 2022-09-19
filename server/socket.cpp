#include "socket.hpp"

void Socket::setup(std::string port, std::string ip) {
    int port_int = stoi(port);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port_int);

    if(ip.empty())
        ip = "0.0.0.0";

    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("serverSocket : ");
        exit(-1);
    }

    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

    int value = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
    {
        perror("setsockopt :");
        exit(-1);
    }

    address.sin_addr.s_addr = inet_addr(ip.c_str());
    if((bind(serverSocket, (struct sockaddr *)&address, sizeof(address))) < 0)
    {
        perror("bind");
        exit(-1);
    }
    if (listen(serverSocket, 42) != 0)
    {
        perror("Listenning");
        exit(-1);
    }
    else
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        std::cout << colors::bright_yellow << "[" << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "]" << "[" << port << "] listening ..." << std::endl;
    }
}


