#include "ChatNetworking/tcp_conection.h"
#include <iostream>

namespace Chat {
    TCPConnection::TCPConnection(boost::asio::io_context &io_context) : socket_(io_context) {

    }

    void TCPConnection::Start() {
        auto strong_this = shared_from_this();
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
                                 [strong_this](const boost::system::error_code& error, size_t bytes_transfered){
            if(error) {
                std::cout << "Failed to send message\n";
            } else {
                std::cout << "Sent " << bytes_transfered << " bytes of data\n";
            }
        });


        boost::asio::streambuf buffer;
        socket_.async_receive(buffer.prepare(512),
                              [this](const boost::system::error_code& error, size_t bytes_transfered){
            if(error == boost::asio::error::eof)
                std::cout << "Client disconnected properly!\n";
            else if(error)
                std::cout << "Client disconnected inproperly!\n";
        });
    }
}
