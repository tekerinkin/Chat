#pragma once

#include "tcp_conection.h"

#include <boost/asio.hpp>

#include <string>
#include <unordered_set>
#include <optional>
#include <functional>
#include <memory>

namespace chat
{
	using boost::asio::ip::tcp;

	namespace io = boost::asio;

	enum class IPV
	{
		V4,
		V6
	};

	class TCPServer
	{
		using OnJoinHandler			 = std::function<void(TCPConnection::Pointer)>;
		using OnLeaveHandler		 = std::function<void(TCPConnection::Pointer)>;
		using OnClientMessageHandler = std::function<void(std::string)>;

	public:
		TCPServer(int port, IPV version);

		int Run();

		void Broadcast(const std::string& message);

	private:
		void StartAccept();

	public:
		OnJoinHandler on_join_;
		OnLeaveHandler on_leave_;
		OnClientMessageHandler on_client_message;

	private:
		int port_;
		IPV ip_version_;

		io::io_context io_context_;
		tcp::acceptor acceptor_;
		std::optional<tcp::socket> socket_;
		std::unordered_set<TCPConnection::Pointer> connections_{};
	};
}  // namespace Chat
