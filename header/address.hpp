

#pragma once

#include <netinet/in.h>
#include <string.h>
#include <string>
#include <memory>
#include <netdb.h>

namespace imp {

class Address
{
private:
    struct sockaddr_in _addr;
    std::unique_ptr<const struct hostent *> _host;
    unsigned int _len;
public:
    Address();
    Address(const Address &address);
    Address(const unsigned short int family, const unsigned int addr, const unsigned short int port);
    Address(const unsigned short int family, const std::string &addr, const unsigned short int port);
    unsigned int& size();
    struct sockaddr_in& operator*();
};

}
