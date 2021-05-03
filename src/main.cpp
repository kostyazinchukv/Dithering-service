#include "../include/server.h"

int main(int argc, char* argv[])
{
    Server server(AF_INET, SOCK_STREAM, 0, 9998);
    server.make_stream(server.get_sock(), server.get_adress());
    return 0;
}