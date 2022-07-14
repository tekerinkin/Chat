#pragma once

#include <../include/ChatNetworking/tcp_conection.h>

#include <boost/asio.hpp>

#include <string>
#include <vector>


namespace Chat {
    using boost::asio::ip::tcp;

    enum class IPV {
        V4,
        V6
    };

    class TCPServer {
    public:
        TCPServer(int port, IPV version);

        int Run();

    private:
        void StartAccept();

    private:
        int port_;
        IPV ip_version_;

        boost::asio::io_context io_context_;
        tcp::acceptor acceptor_;

        std::vector<TCPConnection::pointer> connections_ {};
    };
}


