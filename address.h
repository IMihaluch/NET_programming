#pragma once

#include <string.h>
#include <netinet/in.h>

namespace imp
{

class Address
{
private:
    struct sockaddr_in _addr;
    unsigned int _len;
public:
    Address(unsigned short int family, unsigned int addr, unsigned short int port)
    {
        memset(&_addr, 0, sizeof(_addr));
        _len = sizeof(_addr);

        _addr.sin_family = family;
        _addr.sin_addr.s_addr = addr;
        _addr.sin_port = htons(port);
    }

    Address()
    {
        memset(&_addr, 0, sizeof(_addr));
        _len = sizeof(_addr);
    }

    Address(Address &address)
    {
        _addr = address._addr;
        _len = address._len;
    }

    unsigned int& size()
    {
        return _len;
    }

    struct sockaddr_in& operator*()
    {
        return _addr;
    }
};

}
