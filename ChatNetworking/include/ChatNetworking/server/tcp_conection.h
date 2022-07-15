#pragma once
#include <boost/asio.hpp>
#include <memory>

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
    private:
        explicit TCPConnection(tcp::socket&& socket);
    private:
        tcp::socket socket_;
        std::string username_;
    };
}