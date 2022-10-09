
#pragma once

#include <memory>
#include <socket.hpp>
#include <address.hpp>
#include <buffer.hpp>

namespace imp
{

class Server 
{
protected:
    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Address> _address;
    std::unique_ptr<Buffer> _buffer;
    const unsigned short int _port;
public:
    Server(const unsigned short int port, const unsigned short int sizeBuffer);
    virtual void bind() {}
    virtual void listen() {}
};

}
