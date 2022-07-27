#include <iostream>
#include "ChatNetworking/server/tcp_server.h"

int main()
{
	chat::TCPServer server{ 1337, chat::IPV::V4 };

	server.on_join_ = [](chat::TCPConnection::Pointer server) {
		std::cout << "User has joined the server: " << server->GetUsername() << std::endl;
	};

	server.on_leave_ = [](chat::TCPConnection::Pointer server) {
		std::cout << "User has left the server: " << server->GetUsername() << std::endl;
	};

	server.on_client_message = [&server](const std::string& message) { server.Broadcast(message); };

	server.Run();

	return 0;
}