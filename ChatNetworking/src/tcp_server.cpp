#include "ChatNetworking/tcp_server.h"

#include <iostream>

namespace Chat {
    TCPServer::TCPServer(int port, IPV version) : port_(port), ip_version_(version),
        acceptor_ (io_context_, tcp::endpoint(version == IPV::V4 ? tcp::v4() : tcp::v6(),
                                              port_))
    {

    }

    int TCPServer::Run() {

        try {
            StartAccept();
            io_context_.run();
        } catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return 0;
    }

    void TCPServer::StartAccept() {
        auto connection = TCPConnection::Create(io_context_);

        connections_.push_back(connection);

        acceptor_.async_accept(connection->get_socket(),
                               [connection, this]
                               (const boost::system::error_code& error){
            if(!error) {
                connection->Start();
            }

            StartAccept();
        });
    }
}