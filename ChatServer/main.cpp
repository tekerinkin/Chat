#include <iostream>
#include "ChatNetworking/server/tcp_server.h"

int main() {
  Chat::TCPServer server{1337, Chat::IPV::V4};

  server.Run();

  return 0;
}