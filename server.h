#pragma once

#include <socket.h>
#include <address.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#define MAXLINE 1024

namespace imp
{

class Server
{
private:
    Socket _socket;
    Address _address;
    Address _client;
    char _buffer[MAXLINE];
    bool _listenLoop;
    long int _recv_len;

    typedef void (*hPtr)(std::string buffer, Server &s);

    class Hook
    {
    private:
        std::string _command;
        hPtr _handler;
    public:
        Hook(std::string command, hPtr handler) :
                _command(command), _handler(handler)
        {
        }

        void execute(std::string buffer, Server &s)
        {
            if (_command == "")
            {
                _handler(buffer, s);
            }
            else
            {
                auto pos = buffer.find_first_of(_command);
                if (pos != std::string::npos && pos == 0)
                {
                    _handler(buffer, s);
                }
            }
        }
    };

    std::vector<Hook> _hooks;

    void print(char *buffer)
        {
            std::string s_buffer(buffer);
            std::cout << s_buffer;
        }
public:
    Server(Socket &socket, Address &address) :
            _socket(socket), _address(address), _listenLoop(true), _recv_len(0)
    {
    }

    Server() :
            _listenLoop(true), _recv_len(0)
    {
    }

    void createUDP(unsigned short int port)
    {
        _socket = { AF_INET, SOCK_DGRAM, IPPROTO_UDP };
        _address = { AF_INET, INADDR_ANY, port };
    }

    void bind()
    {
        if (::bind(_socket, reinterpret_cast<const sockaddr*>(&_address), _address.size()) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }

    void listen()
    {
        while (_listenLoop)
        {
            _recv_len = recvfrom(_socket, _buffer, MAXLINE - 1, MSG_WAITALL, reinterpret_cast<sockaddr*>(&_client), &_client.size());
            if (_recv_len > 0)
            {
                _buffer[_recv_len] = '\0';
                std::for_each(_hooks.begin(), _hooks.end(), [&](Hook &hook)
                {
                    hook.execute(_buffer, *this);
                });

                const char *answer = "Сообщение получено!";

                sendto(_socket, answer, strlen(answer), MSG_CONFIRM, reinterpret_cast<const sockaddr*>(&_client), _client.size());
            }
        }
    }

    void registerHook(std::string command, hPtr handler)
    {
        _hooks.push_back( { command, handler });
    }

    void stop()
    {
        _listenLoop = false;
    }
};

}
