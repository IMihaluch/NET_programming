#include "server_udp.h"
#include "udp.h"

void print(std::string buffer, imp::Server &s)
{
    std::cout << buffer << std::endl;
}

void stop(std::string buffer, imp::Server &s)
{
    s.stop();
}

int serverUDP()
{
    imp::Server server;

    server.createUDP(1991);
    server.bind();

    server.registerHook("", print);
    server.registerHook("exit", stop);

    server.listen();

    return 0;
}
