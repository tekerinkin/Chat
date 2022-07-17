#include "ChatNetworking/server/tcp_conection.h"
#include <iostream>

namespace Chat {
TCPConnection::TCPConnection(tcp::socket&& socket)
    : socket_(std::move(socket)) {
  boost::system::error_code error;
  std::stringstream name;
  name << socket_.remote_endpoint();

  username_ = name.str();
}

void TCPConnection::Start() {
  AsyncRead();
}

void TCPConnection::SendMessage(const std::string& message) {
}

void TCPConnection::AsyncRead() {
  io::async_read_until(
      socket_, stream_buf_, "\n",
      [self = shared_from_this()](boost::system::error_code error,
                                  size_t bytes_transferred) {
        self->OnRead(error, bytes_transferred);
      });
}

void TCPConnection::OnRead(boost::system::error_code error,
                           size_t bytes_transferred) {
  if (error) {
    socket_.close();

    return;
  }

  std::stringstream msg;
  msg << username_ << ": " << std::istream(&stream_buf_).rdbuf();

  stream_buf_.consume(bytes_transferred);
  std::cout << msg.str() << std::endl;

  AsyncRead();
}
}  // namespace Chat
