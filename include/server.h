#ifndef SERVER_H
#define SERVER_H
#include "../include/basesocket.h"

#include <unistd.h>
#include <vector>

// struct Request{

//     int version;
//     std::string header;
//     std::vector<char> body;
//     Request(int ver, std::string h, std::vector<char> bd);
//     //getters
//     int getVersion() const;
//     std::string getHeader() const;
//     std::vector<char> getBody() const;
//     //setters
//     void setVersion(int ver);
//     void setHeader(const std::string& h);
//     void setHeader(std::vector<char>& bd);

// };

class Server : public BaseSocket
{
    public:
        Server(int domain, int type, int protocol,int port);
        int soc_bind(int sock, struct sockaddr_in address);
        int make_stream(int socket, struct sockaddr_in address);
        std::vector<char> parseRequsest(const std::string& header, const std::vector<char>& body);
    private:
        int listener_soc;
        int datastream_sock;
        std::string target;
        std::string method;
};

#endif