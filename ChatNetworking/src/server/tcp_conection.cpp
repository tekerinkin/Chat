#include "ChatNetworking/server/tcp_conection.h"
#include <iostream>

namespace Chat {
    TCPConnection::TCPConnection(tcp::socket&& socket) : socket_(std::move(socket)) {
        boost::system::error_code error;
        std::stringstream name;
        name << socket_.remote_endpoint();

        username_ = name.str();
    }

    void TCPConnection::Start() {

    }
}
