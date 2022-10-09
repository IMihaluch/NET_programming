

#include <address.hpp>

namespace imp {

Address::Address()
{
    memset(&_addr, 0, sizeof(_addr));
    _len = sizeof(_addr);
}

Address::Address(const Address &address)
{
    _addr = address._addr;
    _len = address._len;
}

Address::Address(const unsigned short int family, const unsigned int addr, const unsigned short int port)
{
    memset(&_addr, 0, sizeof(_addr));
    _len = sizeof(_addr);

    _addr.sin_family = family;
    _addr.sin_addr.s_addr = htonl(addr);
    _addr.sin_port = htons(port);
}

Address::Address(const unsigned short int family, const std::string &addr, const unsigned short int port)
{
    memset(&_addr, 0, sizeof(_addr));
    _len = sizeof(_addr);

    _host = std::make_unique<const struct hostent *>(gethostbyname(addr.c_str()));

    _addr.sin_family = family;
    _addr.sin_addr.s_addr = *reinterpret_cast<const in_addr_t*>((*_host)->h_addr);
    _addr.sin_port = htons(port);
}

unsigned int& Address::size()
{
    return _len;
}

struct sockaddr_in& Address::operator*()
{
    return _addr;
}

}
