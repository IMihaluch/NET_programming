

#include <stock.hpp>

namespace imp
{

Server::Server(const unsigned short int port, const unsigned short int sizeBuffer) : _port(port)
{
    _buffer = std::make_unique<Buffer>(sizeBuffer);
}

}
