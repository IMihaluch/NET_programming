

#pragma once

#include <stock.hpp>

namespace imp
{

class ClientTCP: public Server 
{
private:
    bool _connect;

    void chat();
public:
    ClientTCP(const std::string &address, const unsigned short int port, const unsigned short int sizeBuffer = 1024);
    void connect();
};

}
