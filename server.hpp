

#pragma once

//#include <algorithm>

#include <stock.hpp>
#include <vector>
#include <string>

namespace imp
{

class ServerTCP;

typedef void (*hook)(std::string buffer, ServerTCP &s);

class ServerTCP: public Server 
{
private:
    bool _bind;
    int _connfd;
    bool _listenLoop;
    unsigned int _sizeClient;
    std::unique_ptr<Address> _client;

    class Hook
    {
    private:
        std::string _command;
        hook _handler;
    public:
        Hook(std::string command, hook handler);
        void execute(std::string buffer, ServerTCP &s);
    };

    std::vector<Hook> _hooks;

    void chat();
public:
    ServerTCP(const unsigned short int port, const unsigned short int sizeBuffer = 1024);
    void registerHook(std::string command, hook handler);
    void bind();
    void listen();
    void disconnect();
    void stop();
};

}

