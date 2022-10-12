#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace imp
{
    using boost::asio::ip::tcp;
    namespace io = boost::asio;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>
    {
    public:
        using Pointer = std::shared_ptr<TCPConnection>;

        static Pointer create(io::ip::tcp::socket&& socket);

        inline const std::string& getUsername() const
        {
            return _username;
        }

        tcp::socket& socket();
        void start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);

        void post(const std::string& message);
    private:
        tcp::socket _socket;
        std::string _username;

        std::queue<std::string> _outgoingMessages;
        io::streambuf _streamBuf {65536};

        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    private:
        explicit TCPConnection(io::ip::tcp::socket&& socket);

        void asyncRead();
        void onRead(boost::system::error_code &ec, size_t bytesTransferred);

        void asyncWrite();
        void onWrite(boost::system::error_code &ec, size_t bytesTransferred);
    };

}
