#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

namespace imp
{

class Socket
{
private:
    int _sockfd;
public:
    Socket(int domain, int type, int protocol)
    {
        if ((_sockfd = socket(domain, type, protocol)) < 0)
        {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
    }

    Socket(Socket &socket)
    {
        _sockfd = socket._sockfd;
    }

    Socket()
    {
        _sockfd = -1;
    }

    operator int()
    {
        return _sockfd;
    }
};

}
