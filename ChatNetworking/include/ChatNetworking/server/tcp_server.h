#pragma once

#include "tcp_conection.h"

#include <boost/asio.hpp>

#include <string>
#include <unordered_set>
#include <optional>

namespace Chat {
using boost::asio::ip::tcp;

namespace io = boost::asio;

enum class IPV { V4, V6 };

class TCPServer {
 public:
  TCPServer(int port, IPV version);

  int Run();

  void Broadcast(const std::string& message);

 private:
  void StartAccept();

 private:
  int port_;
  IPV ip_version_;

  io::io_context io_context_;
  tcp::acceptor acceptor_;
  std::optional<tcp::socket> socket_;
  std::unordered_set<TCPConnection::pointer> connections_{};
};
}  // namespace Chat
