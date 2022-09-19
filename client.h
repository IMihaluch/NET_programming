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

class Client
{
private:
    Socket _socket;
    Address _address;
    char _buffer[MAXLINE];
    long int _recv_len;
public:
    Client(Socket &socket, Address &address) :
            _socket(socket), _address(address), _recv_len(0)
    {
    }

    Client() :
            _recv_len(0)
    {
    }

    void createUDP(unsigned short int port)
    {
        _socket = { AF_INET, SOCK_DGRAM, IPPROTO_UDP };
        _address = { AF_INET, INADDR_ANY, port };
    }

    void start()
    {
        while (true)
        {
            char message[MAXLINE];
            message[MAXLINE] = '\0';
            std::cin.getline(message, MAXLINE);

            sendto(_socket, message, strlen(message), MSG_CONFIRM, reinterpret_cast<const sockaddr*>(&_address), _address.size());

            _recv_len = recvfrom(_socket, _buffer, MAXLINE - 1, MSG_WAITALL, reinterpret_cast<sockaddr*>(&_address), &_address.size());
            if (_recv_len > 0)
            {
                _buffer[_recv_len] = '\0';
                std::cout << _buffer << std::endl;
            }
        }
    }
};

}
