//
// Created by Дима Шестаков on 27.07.2022.
//

#pragma once

#include <boost/asio.hpp>
#include <queue>

namespace chat
{
	namespace io		 = boost::asio;
	using MessageHandler = std::function<void(std::string)>;

	class TCPClient
	{
	public:
		TCPClient(const std::string& address, int port);

		void Run();
		void Stop();
		void SendMessage(const std::string& message);

	private:
		void AsyncRead();
		void OnRead(boost::system::error_code error, size_t bytes_transffered);
		void AsyncWrite();
		void OnWrite(boost::system::error_code error, size_t bytes_transffered);

	public:
		MessageHandler on_message_;

	private:
		io::io_context io_context_{};
		io::ip::tcp::socket socket_;

		io::ip::tcp::resolver::results_type endpoints_;

		io::streambuf stream_buf_{ 65536 };
		std::queue<std::string> outgoing_messages_{};
	};
} // namespace chat
