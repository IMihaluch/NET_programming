#include <ap/ap.hpp>
#include <server/tcp_server.hpp>
#include <connection/tcp_connection.hpp>
#include <iostream>

void server(int port)
{
    using namespace imp;

    TCPServer server(IPV::V4, port);

    server.onJoin = [](TCPConnection::Pointer server)
    {
        std::cout << "User has joined the server " << server->getUsername() << std::endl;
    };

    server.onLeave = [](TCPConnection::Pointer server)
    {
        std::cout << "User has left the server " << server->getUsername() << std::endl;
    };

    server.onClientMessage = [&server](const std::string& message)
    {
        server.broadcast(message);
    };

    server.run();
}

int main(int argc, char *argv[])
{
    ap::Hub hub( { { "port", 'p', ap::REQUIRED }, { "host", 'h', ap::OPTIONAL }, { "server", 's', ap::NO }, { "client", 'c', ap::NO } });
    hub.readArguments(argc, argv);

    auto optionPort = hub.getOption('p');
    if (!optionPort.isSet())
    {
        std::cerr << "Порт не был установлен!" << std::endl;
        exit(EXIT_FAILURE);
    }

    int port = std::stoi(optionPort.getValues()[0]);

    auto optionClient = hub.getOption('c');
    auto optionServer = hub.getOption('s');

    if (optionClient.isSet())
    {
        auto optionHost = hub.getOption('h');
        if (!optionHost.isSet())
        {
            std::cerr << "Адрес не был установлен!" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto host = optionHost.getValues()[0];

//        client(port, host);
    }
    else if (optionServer.isSet())
        server(port);
    else
    {
        std::cerr << "Необходимо установить режим запуска:" << std::endl << "\t-c\t--client\tЗапуск в режиме клиента" << std::endl
                << "\t-s\t--server\tЗапуск в режиме сервера" << std::endl << "\t-h\t--host\t\tУказать IP-адрес сервера" << std::endl
                << "\t-p\t--port\t\tУказать порт" << std::endl;
        exit(EXIT_FAILURE);
    }

 return 0;
