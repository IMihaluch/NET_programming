

#include <server.hpp>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <algorithm>

namespace imp
{

ServerTCP::Hook::Hook(std::string command, hook handler) : _command(command), _handler(handler)
{
}

void ServerTCP::Hook::execute(std::string buffer, ServerTCP &s)
{
    if (_command == "")
    {
        _handler(buffer, s);
    }
    else
    {
        if (buffer == _command)
        {
            _handler(buffer, s);
        }
    }
}

void ServerTCP::registerHook(std::string command, hook handler)
{
    _hooks.push_back({ command, handler });
}

ServerTCP::ServerTCP(const unsigned short int port, const unsigned short int sizeBuffer) : Server(port, sizeBuffer), _bind(false), _connfd(-1), _listenLoop(true)
{
    _socket = std::make_unique<Socket>(AF_INET, SOCK_STREAM, IPPROTO_IP);
    _address = std::make_unique<Address>(AF_INET, INADDR_ANY, port);
    _sizeClient = sizeof(_client);
}

void ServerTCP::bind()
{
    if (_bind)
    {
        std::cerr << "Адрес уже связан с дескриптором слушающего сокета!" << std::endl;
        return;
    }

    if (::bind(*_socket, reinterpret_cast<const sockaddr*>(&(*_address)), (*_address).size()) != 0)
        std::cerr << "Не удаётся связать адрес с дескриптором слушающего сокета!" << std::endl;
    else
        _bind = true;
}

void ServerTCP::listen()
{
    if (::listen(*_socket, 0) != 0)
    {
        std::cerr << "Не удаётся создать очередь соединений для сокета!" << std::endl;
        return;
    }

    while (_bind)
    {
        if ((_connfd = ::accept(*_socket, reinterpret_cast<sockaddr*>(&(*_client)), &_sizeClient)) < 0)
        {
            std::cerr << "Не удаётся верифицировать и принять пакеты от клиента!" << std::endl;
            return;
        }
        else
            _listenLoop = true;

        // Обработка соединения с клиентом
        chat();
    }
}

void ServerTCP::chat()
{
	Buffer *bf = &(*_buffer);
    while (_listenLoop)
    {
        bf->clear();
        int size = read(_connfd, *bf, bf->getSize());
        if (size > 0)
        {
            std::for_each(_hooks.begin(), _hooks.end(), [&](Hook &hook)
            {
                hook.execute(bf->getString(), *this);
            });
            if (!_listenLoop)
            	write(_connfd, "exit", 4);
            else
            	write(_connfd, "OK", 2);
        }
        else if (size < 0)
            break;
    }
    close(_connfd);
}

void ServerTCP::stop()
{
    _listenLoop = false;
    _bind = 0;
}

void ServerTCP::disconnect()
{
    _listenLoop = false;
}

}
