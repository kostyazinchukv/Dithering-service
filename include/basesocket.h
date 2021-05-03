#ifndef BASESOCKET_H
#define BASESOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>

class BaseSocket
{
    public:
        BaseSocket(int domain, int type, int protocol, int port);
        //virtual int soc_bind(int sock, struct sockaddr_in adress) = 0;
        int test_connection(int value, const std::string& func_name);
        int get_sock() const;
        int get_connection() const;
        struct sockaddr_in get_adress() const; 

    private:
        int sock;
        int connection;
        struct sockaddr_in adress;

};
#endif