#pragma once

#include <boost/asio.hpp>
#include <connection/tcp_connection.hpp>
#include <vector>
#include <optional>
#include <unordered_set>

namespace imp
{
    namespace io = boost::asio;

    enum class IPV
    {
        V4, V6
    };

    class TCPServer
    {
        using OnJoinHandler = std::function<void(TCPConnection::Pointer)>;
        using OnLeaveHandler = std::function<void(TCPConnection::Pointer)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        TCPServer(IPV ipv, int port);

        int run();

        void broadcast(const std::string& message);
    public:
        OnJoinHandler onJoin;
        OnLeaveHandler onLeave;
        OnClientMessageHandler onClientMessage;
    private:
        IPV _ipVersion;
        int _port;

        io::io_context _ioContext;
        io::ip::tcp::acceptor _acceptor;
        std::optional<io::ip::tcp::socket> _socket;
        std::unordered_set<TCPConnection::Pointer> _connections;
    private:
        void startAccept();
    };
