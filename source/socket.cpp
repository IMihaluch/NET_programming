

#include <socket.hpp>
#include <unistd.h>

namespace imp {

Socket::Socket(const int domain, const int type, const int protocol)
{
    if ((_sockfd = socket(domain, type, protocol)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
}

Socket::operator int() const
{
    return _sockfd;
}

Socket::~Socket()
{
    ::close(_sockfd);
}

}
