#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <queue>

namespace Chat {
    using boost::asio::ip::tcp;
    namespace io = boost::asio;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>{
    public:
        using pointer = std::shared_ptr<TCPConnection>;

        static pointer Create(tcp::socket&& socket) {
            return pointer(new TCPConnection(std::move(socket)));
        }
        tcp::socket& get_socket() {
            return socket_;
        }

        void Start();
        void SendMessage(const std::string& message);

    private:
        explicit TCPConnection(tcp::socket&& socket);

        void AsyncRead();
        void OnRead(boost::system::error_code error, size_t bytes_transferred);

        void AsyncWrite();
        void OnWrite(boost::system::error_code error, size_t bytes_transferred);
    private:
        tcp::socket socket_;
        std::string username_;

        std::queue<std::string> outgoing_messages_;
        io::streambuf stream_buf_ {65536};
    };
}