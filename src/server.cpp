#include "../include/server.h"
#include "../include/dithering.h"
#include <stdlib.h>
#include <cstring>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>




Server::Server(int domain, int type, int protocol, 
                    int port) : 
                    BaseSocket(domain, type, protocol, port) 
{
    soc_bind(BaseSocket::get_sock(), BaseSocket::get_adress());
    test_connection(BaseSocket::get_sock(), "binding");
    listener_soc = listen(BaseSocket::get_sock(), 5);
    test_connection(listener_soc, "listening");
    
}

int Server::soc_bind(int sock, struct sockaddr_in address)
{
    return bind(sock, (struct sockaddr*)&address, sizeof(address));
}

int Server::make_stream(int socket, struct sockaddr_in address)
{
    bool recievedFull = false;
    char buf[10000] = {0};
    int addrlen = sizeof(address);
    int contentLenght = 0;
    int curPos = 0;
    long dataline;
    std::string header;
    std::vector<char> body;
    std::vector<char> content;
    body.reserve(2000);
    content.reserve(10000);

    Dithering dither;
    
    while(1)
    {
        std::cout<<"Server is up\n";
        datastream_sock = accept(socket, (struct sockaddr*)&address,
                            (socklen_t*)&addrlen);
        test_connection(datastream_sock, "accepting");

        while(!recievedFull && (dataline = read(datastream_sock, buf, sizeof(buf))) > 0)
        {
            
            std::string s_buf = buf;
            
            std::string::size_type index = s_buf.find("Content-Length: ");
            
            
            if(std::string::npos != index)
            {
                std::cout<<"First\n";
                auto inStrContentLenght = s_buf.substr(index + strlen("Content-Length: "),
                                            s_buf.find("\r\n"));
                contentLenght = atoi(inStrContentLenght.c_str());
                header = s_buf;
            }
            else
            {
                std::cout<<"Second\n";
                for(int i = 0; i<dataline;i++, curPos++)
                {
                    body.push_back(buf[i]);
                }
            }
            if (curPos == contentLenght)
            {
                recievedFull = true;
                curPos = 0;
                std::vector<char> tmp;
                tmp = parseRequsest(header, body);
                for(auto it = tmp.begin(); it!=tmp.end(); it++)
                {
                    content.push_back(*it);
                }
                memset(buf, 0, 10000);
            }
        }
        std::cout<<header;
        SDL_Surface* imgSurface = IMG_LoadJPG_RW(SDL_RWFromMem(content.data(), 
                                                content.size()));
        IMG_SaveJPG(imgSurface, "./images/negr.jpg", 100);
        dither.setWidth(imgSurface->w);
        dither.setHeight(imgSurface->h);
        dither.setSurface(imgSurface);
        dither.apply();

        
    }
    shutdown(datastream_sock, SHUT_RDWR);
    close(datastream_sock);
}

std::vector<char> Server::parseRequsest(const std::string& header,const std::vector<char>& body)
{
    const char ending[] = {13,10};
    const char blank_line[] = {10,13,10};
    method = header.substr(0, header.find(" "));
    auto target_it = std::search(body.begin(), body.end(),
                            "filename=", "filename=" + strlen("filename="));
    auto target_after = std::search(target_it, body.end(), 
                            ending, ending + sizeof(ending));
    for(auto i = target_it + strlen("filename=") + 1- body.begin(); i<target_after-body.begin()-1;i++)
    {
        target += body[i];
    }
    std::vector<char> content;
    auto content_it = std::search(body.begin(), body.end(), 
                                    blank_line, blank_line + sizeof(blank_line));
    auto content_after = std::search(content_it+sizeof(blank_line), body.end(), 
                                    blank_line, blank_line + sizeof(blank_line));
    for(auto i = content_it+sizeof(blank_line); i<content_after; i++)
    {
        content.push_back(*i);
    }
    return content;
}


