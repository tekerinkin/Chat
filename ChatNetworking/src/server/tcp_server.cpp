#include "ChatNetworking/server/tcp_server.h"

#include <iostream>
#include <memory>

namespace chat
{
	TCPServer::TCPServer(int port, IPV version)
		: port_(port)
		, ip_version_(version)
		, acceptor_(io_context_, tcp::endpoint(version == IPV::V4 ? tcp::v4() : tcp::v6(), port_))
	{
	}

	int TCPServer::Run()
	{
		try
		{
			StartAccept();
			io_context_.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return -1;
		}
		return 0;
	}

	void TCPServer::Broadcast(const std::string& message)
	{
		for (auto& connection : connections_)
		{
			connection->SendMessage(message);
		}
	}

	void TCPServer::StartAccept()
	{
		socket_.emplace(io_context_);

		acceptor_.async_accept(*socket_, [this](const boost::system::error_code& error) {
			auto connection = TCPConnection::Create(std::move(*socket_));

			if (on_join_)
			{
				on_join_(connection);
			}

			connections_.insert(connection);

			if (!error)
			{
				connection->Start(
					[this](const std::string& message) {
						if (on_client_message)
							on_client_message(message);
					},
					[&, weak = std::weak_ptr<chat::TCPConnection>(connection)] {
						if (auto shared = weak.lock(); shared && (connections_.erase(shared) != 0u))
						{
							if (on_leave_)
								on_leave_(shared);
						}
					});
			}

			StartAccept();
		});
	}
}  // namespace Chat