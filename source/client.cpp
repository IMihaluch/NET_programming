

#include <client.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

namespace imp
{

ClientTCP::ClientTCP(const std::string &address, const unsigned short int port, const unsigned short int sizeBuffer) : Server(port, sizeBuffer), _connect(false)
{
    _socket = std::make_unique<Socket>(AF_INET, SOCK_STREAM, IPPROTO_IP);
    _address = std::make_unique<Address>(AF_INET, address, port);
}

void ClientTCP::connect()
{
    if (_connect)
    {
        std::cerr << "Клиентский сокет уже подключён к серверному сокету!" << std::endl;
        return;
    }

    if (::connect(*_socket, reinterpret_cast<const sockaddr*>(&(*_address)), (*_address).size()) != 0)
        std::cerr << "Не удаётся подключить клиентский сокет к серверному сокету!" << std::endl;
    else
        _connect = true;

    chat();
}

void ClientTCP::chat()
{
	Buffer *bf = &(*_buffer);
    while (_connect)
    {
        bf->clear();
        std::cin.getline(*bf, bf->getSize());
        write(*_socket, *bf, bf->getSize());
        bf->clear();
		if (read(*_socket, *bf, bf->getSize()) > 0)
		{
			if (bf->getString() == "exit")
				break;
			else
				std::cout << '\t' << bf->getString() << std::endl;
		}
    }
    close(*_socket);
}

}
