#include "../include/basesocket.h"
#include <arpa/inet.h>
BaseSocket::BaseSocket(int domain, int type, int protocol, int port)
{
    adress.sin_family = domain;
    adress.sin_port = htons(port);
    adress.sin_addr.s_addr = inet_addr("127.0.0.1");

    sock = socket(domain, type, protocol);
    test_connection(sock, "socket");
}

int BaseSocket::test_connection(int value, const std::string& func_name)
{
    if(value < 0)
    {
        std::cerr<<"Failed in "+func_name+"\n";
        exit(EXIT_FAILURE);
    }
    return 1;
}

int BaseSocket::get_sock() const 
{
    return sock;
}

int BaseSocket::get_connection() const
{
    return connection;
}

struct sockaddr_in BaseSocket::get_adress() const
{
    return adress;
}