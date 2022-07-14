#pragma once
#include <boost/asio.hpp>
#include <memory>

namespace Chat {
    using boost::asio::ip::tcp;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>{
    public:
        using pointer = std::shared_ptr<TCPConnection>;

        static pointer Create(boost::asio::io_context& io_context) {
            return pointer(new TCPConnection(io_context));
        }
        tcp::socket& get_socket() {
            return socket_;
        }

        void Start();
    private:
        explicit TCPConnection(boost::asio::io_context& io_context);
    private:
        tcp::socket socket_;
        std::string message_ {"Hello beautiful client!\n"};
    };
}