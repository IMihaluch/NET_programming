

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

namespace imp {

class Socket
{
private:
    int _sockfd;
public:
    Socket(const int domain, const int type, const int protocol);
    operator int() const;
    ~Socket();
};

}
