#include <connection/tcp_connection.hpp>
#include <server/tcp_server.hpp>
#include <iostream>

namespace imp
{
    using boost::asio::ip::tcp;

    TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv), _port(port), _acceptor(_ioContext,
            tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port))
    {
    }

    int TCPServer::run()
    {
        try
        {
            startAccept();
            _ioContext.run();
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        return 0;
    }

    void TCPServer::broadcast(const std::string &message)
    {
        for (auto &connection : _connections)
        {
            connection->post(message);
        }
    }

    void TCPServer::startAccept()
    {
        _socket.emplace(_ioContext);

        _acceptor.async_accept(*_socket, [this](const boost::system::error_code &error)
        {
           auto connection = TCPConnection::create(std::move(*_socket));

            if (onJoin)
            onJoin(connection);

            _connections.insert(connection);

            if (!error)
                connection->start(
                    [this](const std::string& message)
                    {
                        if (onClientMessage) onClientMessage(message);
                    },
                    [&, weak = std::weak_ptr(connection)]
                    {
                        if (auto shared = weak.lock(); shared && _connections.erase(shared))
                        if (onLeave) onLeave(shared);
                    }
                );

            startAccept();
        });
    }
